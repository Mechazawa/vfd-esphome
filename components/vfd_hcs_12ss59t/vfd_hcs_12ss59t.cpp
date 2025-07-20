// vfd_hcs_12ss59t.cpp - ESPHome component for Samsung HCS-12SS59T VFD

#include "vfd_hcs_12ss59t.h"

namespace esphome {
namespace vfd_hcs_12ss59t {

SPISettings settingsA(SPIPARS, LSBFIRST, SPI_MODE3);

VFDHCS12SS59T::VFDHCS12SS59T(uint8_t pinReset, uint8_t pinVdon, uint8_t pinCs)
    : pin_reset(pinReset), pin_vdon(pinVdon), pin_cs(pinCs) {
  clear();
}

void VFDHCS12SS59T::setup() {
  pinMode(pin_reset, OUTPUT);
  pinMode(pin_vdon, OUTPUT);
  pinMode(pin_cs, OUTPUT);
  digitalWrite(pin_reset, HIGH);
  digitalWrite(pin_vdon, HIGH);
  digitalWrite(pin_cs, HIGH);
  SPI.begin();
  clear();
}

void VFDHCS12SS59T::dump_config() {
  ESP_LOGCONFIG("vfd_hcs_12ss59t", "VFD HCS-12SS59T Display:");
  ESP_LOGCONFIG("vfd_hcs_12ss59t", "  Reset Pin: %u", pin_reset);
  ESP_LOGCONFIG("vfd_hcs_12ss59t", "  Vdon Pin: %u", pin_vdon);
  ESP_LOGCONFIG("vfd_hcs_12ss59t", "  CS Pin: %u", pin_cs);
  ESP_LOGCONFIG("vfd_hcs_12ss59t", "  Brightness: %.2f", brightness_);
}

void VFDHCS12SS59T::update() {
  // Called by ESPHome to refresh the display
  this->do_update_();
  SPI.beginTransaction(settingsA);
  select(pin_cs);
  sendCmdSeq(VFDCommand::DCRAM_WR, 0);
  for (uint8_t i = 0; i < NUMDIGITS; i++) {
    sendChar(buf[i]);
  }
  deSelect(pin_cs);
  SPI.endTransaction();
}

void VFDHCS12SS59T::draw_absolute_pixel_internal(int, int, Color) {
  // Not used for character VFD, but required by DisplayBuffer
}

void VFDHCS12SS59T::set_brightness(float brightness) {
  brightness_ = brightness;
  // If the VFD supports brightness via SPI, send the command here
  // sendCmd(VFDCommand::DUTY, static_cast<uint8_t>(brightness_ * 0x0F));
}

void VFDHCS12SS59T::clear() {
  for (uint8_t i = 0; i < NUMDIGITS; i++) {
    buf[i] = ' ';
  }
  buf[NUMDIGITS] = '\0';
  update();
}

void VFDHCS12SS59T::write(const char *text) {
  if (!text) return;
  size_t len = strlen(text);
  for (uint8_t i = 0; i < NUMDIGITS; i++) {
    buf[i] = (i < len) ? text[i] : ' ';
  }
  buf[NUMDIGITS] = '\0';
  update();
}


void VFDHCS12SS59T::select(uint8_t pin) {
  digitalWrite(pin, LOW);
  delayMicroseconds(1);
}

void VFDHCS12SS59T::deSelect(uint8_t pin) {
  delayMicroseconds(8);
  digitalWrite(pin, HIGH);
}

void VFDHCS12SS59T::sendCmd(VFDCommand cmd, uint8_t arg) {
  select(pin_cs);
  SPI.transfer(static_cast<uint8_t>(cmd) | arg);
  delayMicroseconds(8);
  deSelect(pin_cs);
}

void VFDHCS12SS59T::sendCmdSeq(VFDCommand cmd, uint8_t arg) {
  SPI.transfer(static_cast<uint8_t>(cmd) | arg);
  delayMicroseconds(8);
}

void VFDHCS12SS59T::sendChar(char c) {
  SPI.transfer(getCode(c));
  delayMicroseconds(8);
}

char VFDHCS12SS59T::getCode(char c) const {
  if (c >= '@' && c <= '_')
    c -= 48;
  else if (c >= ' ' && c <= '?')
    c += 16;
  else if (c >= 'a' && c <= 'z')
    c -= 80;
  else
    c = '?';
  return c;
}

}  // namespace vfd_hcs_12ss59t
}  // namespace esphome
