// vfd_hcs_12ss59t.cpp - ESPHome component for Samsung HCS-12SS59T VFD

#include "vfd_hcs_12ss59t.h"
#include "esphome/core/log.h"

namespace esphome {
namespace vfd_hcs_12ss59t {

using esphome::number::Number;

static const char *const TAG = "vfd_hcs_12ss59t";
static uint8_t clamp_brightness(float value) {
  if (value < 0) return 0;
  if (value > 15) return 15;
  return static_cast<uint8_t>(value);
}

void VFDHCS12SS59TComponent::setup() {
  if (this->reset_pin_ != nullptr) {
    this->reset_pin_->setup();
    this->reset_pin_->digital_write(true);  // RESET OFF
  }
  if (this->cs_ != nullptr) {
    this->cs_->setup();
    this->cs_->digital_write(true);  // CS HIGH
  }
  if (this->vdon_pin_ != nullptr) {
    this->vdon_pin_->setup();
    this->vdon_pin_->digital_write(true);  // Vdisp OFF
  }

  for (int16_t i = 0; i < NUMDIGITS; i++)
    buf_[i] = ' ';
  buf_[NUMDIGITS] = '\0';

  this->supply_on();

  if (this->reset_pin_ != nullptr) {
    this->reset_pin_->digital_write(false);  // RESET LOW
    delayMicroseconds(1);
    this->reset_pin_->digital_write(true);   // RESET HIGH
    delayMicroseconds(1);
  }

  this->enable();
  this->select();
  this->send_cmd(VFD_NUMDIGIT, NUMDIGITS);
  this->send_cmd(VFD_DUTY, 4);
  this->send_cmd(VFD_LIGHTS, LINORM);
  this->deselect();
  this->disable();
}

void VFDHCS12SS59TComponent::loop() {
  static uint8_t last_brightness = 255;
  if (this->brightness_number_ != nullptr) {
    uint8_t new_brightness = clamp_brightness(this->brightness_number_->state);
    if (new_brightness != last_brightness) {
      this->set_brightness(new_brightness);
      last_brightness = new_brightness;
    }
  }
}

void VFDHCS12SS59TComponent::supply_on() {
  if (this->vdon_pin_ != nullptr)
    this->vdon_pin_->digital_write(false);  // Vdisp ON
  delay(1);
}

void VFDHCS12SS59TComponent::supply_off() {
  if (this->vdon_pin_ != nullptr)
    this->vdon_pin_->digital_write(true);   // Vdisp OFF
  delay(1);
}

void VFDHCS12SS59TComponent::write_text(const char *text) {
  scr_pos_ = NUMDIGITS - 1;
  size_t len = strlen(text);
  if (len > BUFSIZE)
    len = BUFSIZE;
  strncpy(buf_, text, len);
  buf_[BUFSIZE - 1] = 0;
  display();
}

void VFDHCS12SS59TComponent::set_brightness(uint8_t brightness) {
  this->enable();
  this->select();
  this->send_cmd(VFD_DUTY, brightness > 15 ? 15 : brightness);
  this->deselect();
  this->disable();
}

void VFDHCS12SS59TComponent::scroll(int16_t mode) {
  scr_mode_ = mode;
  // Timer-based scrolling not implemented in this port
  display();
}

void VFDHCS12SS59TComponent::display() {
  this->enable();
  this->select();
  this->send_cmd_seq(VFD_DCRAM_WR, 0);

  int16_t p = scr_pos_;
  for (int16_t i = 0; i < NUMDIGITS; i++) {
    send_char(buf_[p--]);
    if (p < 0)
      p = scr_len_ - 1;
  }

  this->deselect();
  this->disable();
}

void VFDHCS12SS59TComponent::select() {
  if (this->cs_ != nullptr)
    this->cs_->digital_write(false);
  delayMicroseconds(1);
}

void VFDHCS12SS59TComponent::deselect() {
  delayMicroseconds(8);
  if (this->cs_ != nullptr)
    this->cs_->digital_write(true);
}

void VFDHCS12SS59TComponent::send_cmd(uint8_t cmd, uint8_t arg) {
  this->select();
  this->transfer_byte(cmd | arg);
  delayMicroseconds(8);
  this->deselect();
}

void VFDHCS12SS59TComponent::send_cmd_seq(uint8_t cmd, uint8_t arg) {
  this->transfer_byte(cmd | arg);
  delayMicroseconds(8);
}

void VFDHCS12SS59TComponent::send_char(char c) {
  this->transfer_byte(get_code(c));
  delayMicroseconds(8);
}

char VFDHCS12SS59TComponent::get_code(char c) {
  if (c >= '@' && c <= '_')
    c -= 48;
  else if (c >= ' ' && c <= '?')
    c += 16;
  else if (c >= 'a' && c <= 'z')
    c -= 80;
  else
    c = get_code('?');
  return c;
}

}  // namespace vfd_hcs_12ss59t
}  // namespace esphome