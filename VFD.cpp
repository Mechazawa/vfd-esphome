// VFD.cpp, vacuum fluorescent display driver for Samsung HCS-12SS59T, V0.9 171112 qrt@qland.de

#include "VFD.h"
// #include "TimerOne.h"

SPISettings settingsA(SPIPARS);

VFD::VFD(int pinReset, int pinVdon, int pinCs)
	: pin_reset(pinReset), pin_vdon(pinVdon), pin_cs(pinCs)
{
	scrLen = NUMDIGITS;
	scrPos = 0;
	scrMode = 0;
	for(int16_t i = 0; i < NUMDIGITS; i++)
		buf[i] = ' ';
	buf[NUMDIGITS] = '\0';
}

void VFD::setPins(int pinReset, int pinVdon, int pinCs)
{
	pin_reset = pinReset;
	pin_vdon = pinVdon;
	pin_cs = pinCs;
}

void VFD::display()
{
	SPI.beginTransaction(settingsA);
	select(pin_cs);

	sendCmdSeq(VFD_DCRAM_WR, 0);

	int16_t p = scrPos;

	for(int16_t i=0; i<NUMDIGITS; i++){
		sendChar(buf[p--]);

		if(p < 0)
			p = scrLen-1;
	}

	deSelect(pin_cs);
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
	select(pin_cs);					// select
	SPI.transfer(cmd | arg);			// send command and argument
	delayMicroseconds(8);				// 1/2 tCSH
	deSelect(pin_cs);				// deselect
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