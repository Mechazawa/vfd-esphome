// VFD.h, vacuum fluroescent display driver for Samsung HCS-12SS59T, V0.9 171112 qrt@qland.de

#ifndef _VFD_h
#define _VFD_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include <SPI.h>							// 50 MIOSO, 51 MOSI, 52 SCK, 53 SS

#define SPIPARS					2000000, LSBFIRST, SPI_MODE3

#define VFD_DCRAM_WR		0x10		// ccccaaaa dddddddd dddddddd ..
#define VFD_CGRAM_WR		0x20		// "
#define VFD_ADRAM_WR		0x30		// ccccaaaa ******dd ******dd ..
#define VFD_DUTY				0x50		// ccccdddd
#define VFD_NUMDIGIT		0x60		// "
#define VFD_LIGHTS			0x70		// cccc**dd

#define LINORM				  0x00		// normal display
#define LIOFF					  0x01		// lights OFF
#define LION					  0x02		//        ON

#define NUMDIGITS			  12			// display digit width
#define BUFSIZE				  100			// display/scroll buffer

class VFD
{
public:
	VFD(int pinReset = 5, int pinVdon = 4, int pinCs = 15);

	void setPins(int pinReset, int pinVdon, int pinCs);
	void init();
	void supplyOn();
	void supplyOff();
	void write(const char* text);
	void scroll(int16_t mode);
	void display();
	void setBrightness(uint8_t brightness);

protected:

private:
	char buf[BUFSIZE];
	int pin_reset;
	int pin_vdon;
	int pin_cs;
	int16_t scrLen;
	int16_t scrPos;
	int16_t scrMode;

	void select(int pin);
	void deSelect(int pin);
	void sendCmd(char cmd, char arg);
	void sendCmdSeq(char cmd, char arg);
	void sendChar(char c);
	char getCode(char c);
};



#endif
