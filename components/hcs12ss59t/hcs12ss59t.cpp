#include "hcs12ss59t.h"
#include "esphome/core/log.h"

#include <cstdarg>
#include <cstring>

namespace esphome::hcs12ss59t {

static const char *const TAG = "hcs12ss59t";

static const uint8_t COMMAND_DCRAM_WRITE = 0x10;
static const uint8_t COMMAND_DUTY = 0x50;
static const uint8_t COMMAND_DIGIT_COUNT = 0x60;
static const uint8_t COMMAND_LIGHTS = 0x70;
static const uint8_t LIGHTS_NORMAL = 0x00;

// Minimum gap between bytes (tDOFF) and before CS goes high (tCSH) per the datasheet.
static const uint32_t BYTE_GAP_US = 8;

// Maps ASCII to the character ROM of the controller. The ROM has no lowercase, so those show as uppercase.
static uint8_t rom_code(char character) {
  if (character >= '@' && character <= '_')
    return character - 48;
  if (character >= ' ' && character <= '?')
    return character + 16;
  if (character >= 'a' && character <= 'z')
    return character - 80;
  return '?' + 16;
}

void HCS12SS59TComponent::setup() {
  memset(this->buffer_, ' ', sizeof(this->buffer_));
  this->spi_setup();

  // VDON and RESET are both active low.
  if (this->vdon_pin_ != nullptr) {
    this->vdon_pin_->setup();
    this->vdon_pin_->digital_write(false);
    delay(1);
  }
  if (this->reset_pin_ != nullptr) {
    this->reset_pin_->setup();
    this->reset_pin_->digital_write(false);
    delayMicroseconds(1);
    this->reset_pin_->digital_write(true);
    delayMicroseconds(1);
  }

  this->send_command_(COMMAND_DIGIT_COUNT, HCS12SS59T_NUM_DIGITS);
  this->send_command_(COMMAND_DUTY, this->intensity_);
  this->send_command_(COMMAND_LIGHTS, LIGHTS_NORMAL);
  this->display_();
}

void HCS12SS59TComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "HCS-12SS59T:\n  Intensity: %u", this->intensity_);
  LOG_PIN("  CS Pin: ", this->cs_);
  LOG_PIN("  Reset Pin: ", this->reset_pin_);
  LOG_PIN("  VDON Pin: ", this->vdon_pin_);
  LOG_UPDATE_INTERVAL(this);
}

void HCS12SS59TComponent::update() {
  memset(this->buffer_, ' ', sizeof(this->buffer_));
  if (this->writer_.has_value())
    (*this->writer_)(*this);

  if (this->intensity_changed_) {
    this->send_command_(COMMAND_DUTY, this->intensity_);
    this->intensity_changed_ = false;
  }
  this->display_();
}

void HCS12SS59TComponent::set_intensity(uint8_t intensity) {
  intensity = std::min<uint8_t>(intensity, 15);
  this->intensity_changed_ |= intensity != this->intensity_;
  this->intensity_ = intensity;
}

// DCRAM address 0 is the rightmost digit, so the buffer goes out back to front.
void HCS12SS59TComponent::display_() {
  this->enable();
  delayMicroseconds(1);
  this->write_byte(COMMAND_DCRAM_WRITE);
  delayMicroseconds(BYTE_GAP_US);
  for (int digit = HCS12SS59T_NUM_DIGITS - 1; digit >= 0; digit--) {
    this->write_byte(rom_code(this->buffer_[digit]));
    delayMicroseconds(BYTE_GAP_US);
  }
  this->disable();
}

void HCS12SS59TComponent::send_command_(uint8_t command, uint8_t argument) {
  this->enable();
  delayMicroseconds(1);
  this->write_byte(command | argument);
  delayMicroseconds(BYTE_GAP_US);
  this->disable();
}

uint8_t HCS12SS59TComponent::print(uint8_t pos, const char *str) {
  uint8_t written = 0;
  for (; *str != '\0' && pos + written < HCS12SS59T_NUM_DIGITS; str++, written++)
    this->buffer_[pos + written] = *str;
  if (*str != '\0')
    ESP_LOGW(TAG, "Text is longer than %u digits, the rest is cut off", HCS12SS59T_NUM_DIGITS);
  return written;
}

uint8_t HCS12SS59TComponent::printf(uint8_t pos, const char *format, ...) {
  char text[64];
  va_list args;
  va_start(args, format);
  int length = vsnprintf(text, sizeof(text), format, args);
  va_end(args);
  return length > 0 ? this->print(pos, text) : 0;
}

uint8_t HCS12SS59TComponent::printf(const char *format, ...) {
  char text[64];
  va_list args;
  va_start(args, format);
  int length = vsnprintf(text, sizeof(text), format, args);
  va_end(args);
  return length > 0 ? this->print(0, text) : 0;
}

uint8_t HCS12SS59TComponent::strftime(uint8_t pos, const char *format, ESPTime time) {
  char text[64];
  size_t length = time.strftime(text, sizeof(text), format);
  return length > 0 ? this->print(pos, text) : 0;
}

uint8_t HCS12SS59TComponent::strftime(const char *format, ESPTime time) { return this->strftime(0, format, time); }

}  // namespace esphome::hcs12ss59t
