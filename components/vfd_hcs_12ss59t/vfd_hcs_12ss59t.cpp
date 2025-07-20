// vfd_hcs_12ss59t.cpp - ESPHome component for Samsung HCS-12SS59T VFD
#include "vfd_hcs_12ss59t.h"

namespace esphome {
namespace vfd_hcs_12ss59t {

SPISettings settingsA(SPIPARS);

VFDHCS12SS59T::VFDHCS12SS59T(int pinReset, int pinVdon, int pinCs)
    : pin_reset(pinReset), pin_vdon(pinVdon), pin_cs(pinCs) {
  scrLen = NUMDIGITS;
  scrPos = 0;
  scrMode = 0;
  for (int16_t i = 0; i < NUMDIGITS; i++)
    buf[i] = ' ';
  buf[NUMDIGITS] = '\0';
}

void VFDHCS12SS59T::set_pins(int pinReset, int pinVdon, int pinCs) {
  pin_reset = pinReset;
  pin_vdon = pinVdon;
  pin_cs = pinCs;
}

void VFDHCS12SS59T::setup() {
  pinMode(pin_reset, OUTPUT);
  pinMode(pin_vdon, OUTPUT);
  pinMode(pin_cs, OUTPUT);
  digitalWrite(pin_reset, HIGH);
  digitalWrite(pin_vdon, HIGH);
  digitalWrite(pin_cs, HIGH);
  SPI.begin();
}

void VFDHCS12SS59T::loop() {
  // Optionally implement periodic display updates here
}

void VFDHCS12SS59T::display() {
  SPI.beginTransaction(settingsA);
  select(pin_cs);
  sendCmdSeq(VFD_DCRAM_WR, 0);
  int16_t p = scrPos;
  for (int16_t i = 0; i < NUMDIGITS; i++) {
    sendChar(buf[p--]);
    if (p < 0)
      p = scrLen - 1;
  }
  deSelect(pin_cs);
  SPI.endTransaction();
}

void VFDHCS12SS59T::select(int pin) {
  digitalWrite(pin, LOW);
  delayMicroseconds(1);
}

void VFDHCS12SS59T::deSelect(int pin) {
  delayMicroseconds(8);
  digitalWrite(pin, HIGH);
}

void VFDHCS12SS59T::sendCmd(char cmd, char arg) {
  select(pin_cs);
  SPI.transfer(cmd | arg);
  delayMicroseconds(8);
  deSelect(pin_cs);
}

void VFDHCS12SS59T::sendCmdSeq(char cmd, char arg) {
  SPI.transfer(cmd | arg);
  delayMicroseconds(8);
}

void VFDHCS12SS59T::sendChar(char c) {
  SPI.transfer(getCode(c));
  delayMicroseconds(8);
}

char VFDHCS12SS59T::getCode(char c) {
  if (c >= '@' && c <= '_')
    c -= 48;
  else if (c >= ' ' && c <= '?')
    c += 16;
  else if (c >= 'a' && c <= 'z')
    c -= 80;
  else
    c = this->getCode('?');
  return c;
}

}  // namespace vfd_hcs_12ss59t
}  // namespace esphome
