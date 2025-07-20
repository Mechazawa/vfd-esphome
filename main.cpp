#include <Arduino.h>
#include <VFD.h>

void setup () {}
	Vfd.init();

	Vfd.write("Hello World!");
	Vfd.setBrightness(10);
}