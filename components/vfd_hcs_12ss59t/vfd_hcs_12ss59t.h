// vfd_hcs_12ss59t.h - ESPHome component for Samsung HCS-12SS59T VFD

#pragma once

#include "esphome/core/component.h"
#include "esphome/components/spi/spi.h"
#include "esphome/components/number/number.h"

namespace esphome {
namespace vfd_hcs_12ss59t {

static const uint8_t VFD_DCRAM_WR = 0x10;
static const uint8_t VFD_CGRAM_WR = 0x20;
static const uint8_t VFD_ADRAM_WR = 0x30;
static const uint8_t VFD_DUTY     = 0x50;
static const uint8_t VFD_NUMDIGIT = 0x60;
static const uint8_t VFD_LIGHTS   = 0x70;

static const uint8_t LINORM = 0x00;
static const uint8_t LIOFF  = 0x01;
static const uint8_t LION   = 0x02;

static const int NUMDIGITS = 12;
static const int BUFSIZE = 100;

class VFDHCS12SS59TComponent : public Component, public spi::SPIDevice<spi::BIT_ORDER_LSB_FIRST, spi::CLOCK_POLARITY_HIGH, spi::CLOCK_PHASE_2EDGE, 2*1000*1000> {
 public:
  void setup() override;
  void loop() override;

  void set_reset_pin(GPIOPin *pin) { this->reset_pin_ = pin; }
  void set_vdon_pin(GPIOPin *pin) { this->vdon_pin_ = pin; }

  void supply_on();
  void supply_off();
  void write_text(const char *text);
  void scroll(int16_t mode);
  void display();
  void set_brightness(uint8_t brightness);
  void set_brightness_number(esphome::number::Number *number) { this->brightness_number_ = number; }

 protected:
  GPIOPin *reset_pin_{nullptr};
  GPIOPin *vdon_pin_{nullptr};

  esphome::number::Number *brightness_number_{nullptr};

  int16_t scr_len_{NUMDIGITS};
  int16_t scr_pos_{NUMDIGITS-1};
  int16_t scr_mode_{0};
  char buf_[BUFSIZE];

  void select();
  void deselect();
  void send_cmd(uint8_t cmd, uint8_t arg);
  void send_cmd_seq(uint8_t cmd, uint8_t arg);
  void send_char(char c);
  char get_code(char c);
};

}  // namespace vfd_hcs_12ss59t
}  // namespace esphome