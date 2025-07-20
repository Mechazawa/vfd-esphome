// VFD.cpp, vacuum fluorescent display driver for Samsung HCS-12SS59T, V0.9 171112 qrt@qland.de

#include "VFD.h"
// #include "TimerOne.h"

VFD Vfd;

SPISettings settingsA(SPIPARS);

void VFD::init()
{
	pinMode(Pin_VFD_RESET, OUTPUT);
	pinMode(Pin_VFD_CS, OUTPUT);

	digitalWrite(Pin_VFD_RESET, HIGH);		// VFD _RESET OFF
	digitalWrite(Pin_VFD_CS, HIGH);			//     _CS

	for(int16_t i = 0; i < NUMDIGITS; i++)		// preset display buffer
		buf[i] = ' ';						// with unused char

	buf[NUMDIGITS] = '\0';					// terminate buffer

	pinMode(Pin_VFD_VDON, OUTPUT);			// _VDON output
	digitalWrite(Pin_VFD_VDON, HIGH);		// Vdisp OFF

	supplyOn();								// supply on
	SPI.begin();

	digitalWrite(Pin_VFD_RESET, LOW);		// reset
	delayMicroseconds(1);					// tWRES
	digitalWrite(Pin_VFD_RESET, HIGH);
	delayMicroseconds(1);					// tRSOFF

	SPI.beginTransaction(settingsA);

	sendCmd(VFD_NUMDIGIT, NUMDIGITS);		// number of digits
	sendCmd(VFD_DUTY, 4);					// brightness 1..15
	sendCmd(VFD_LIGHTS, LINORM);			// lights normal

	SPI.endTransaction();
}

void VFD::supplyOn()
{
	digitalWrite(Pin_VFD_VDON, LOW);				// Vdisp ON

	delay(1);
}

void VFD::supplyOff()
{
	digitalWrite(Pin_VFD_VDON, HIGH);					// Vdisp OFF

	delay(1);
}

void VFD::write(const char* text)
{
	scrPos = NUMDIGITS-1;
	size_t len = strlen(text);
	strlcpy(buf, text, len = len > BUFSIZE ? BUFSIZE : len);
	buf[BUFSIZE-1] = 0;
	display();
}

void VFD::setBrightness(uint8_t brightness)
{
	SPI.beginTransaction(settingsA);
	sendCmd(VFD_DUTY, brightness > 15 ? 15 : brightness);	// set brightness 0..15
	SPI.endTransaction();
}

void doScroll()
{
	Vfd.display();

	if(Vfd.scrMode > 0){
		if(++Vfd.scrPos >= Vfd.scrLen)
			Vfd.scrPos = 0;
	}
	else{
		if(--Vfd.scrPos < 0)
			Vfd.scrPos = Vfd.scrLen-1;
	}
}

void VFD::scroll(int16_t mode)
{
	static bool tirat = false;

	scrMode = mode;

	doScroll();

	// if(mode == 0){
	// 	Timer1.detachInterrupt();
	// }
	// else{
	// 	Timer1.initialize((uint32_t)abs(mode) * 10000);

	// 	if(!tirat){
	// 		Timer1.attachInterrupt(doScroll);
	// 		tirat = true;
	// 	}
	// }
}

void VFD::display()
{
	SPI.beginTransaction(settingsA);
	select(Pin_VFD_CS);

	sendCmdSeq(VFD_DCRAM_WR, 0);

	int16_t p = scrPos;

	for(int16_t i=0; i<NUMDIGITS; i++){
		sendChar(buf[p--]);

		if(p < 0)
			p = scrLen-1;
	}

	deSelect(Pin_VFD_CS);
	SPI.endTransaction();
}

void VFD::select(int pin)
{
	digitalWrite(pin, LOW);				// select
	delayMicroseconds(1);				// tCSS
}

void VFD::deSelect(int pin)
{
	delayMicroseconds(8);				// 1/2 tCSH
	digitalWrite(pin, HIGH);			// deselect
}

void VFD::sendCmd(char cmd, char arg)
{
	select(Pin_VFD_CS);					// select
	SPI.transfer(cmd | arg);			// send command and argument
	delayMicroseconds(8);				// 1/2 tCSH
	deSelect(Pin_VFD_CS);				// deselect
}

void VFD::sendCmdSeq(char cmd, char arg)
{
	SPI.transfer(cmd | arg);			// send command and argument
	delayMicroseconds(8);				// tDOFF
}

void VFD::sendChar(char c)
{
	SPI.transfer(getCode(c));			// send data
	delayMicroseconds(8);				// tDOFF and 1/2 tCSH for last data
}

char VFD::getCode(char c)
{
	if(c>='@' && c<='_')				// 64.. -> 16..
		c -= 48;
	else if(c>=' ' && c<='?')			// 32.. -> 48..
		c += 16;
	else if(c>='a' && c<='z')			// 97.. -> 17..
		c -= 80;
	else								// unvalid -> ?
		c = this->getCode('?');

	return c;
}