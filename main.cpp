#include <Arduino.h>
#include <VFD.h>

VFD vfd(10, 8, SS); // Example pins, adjust as needed

void setup() {
	vfd.init();
	vfd.write("Hello World!");
	vfd.setBrightness(10);
}