#pragma once

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/core/time.h"

#include "esphome/components/display/display.h"
#include "esphome/components/spi/spi.h"

namespace esphome::hcs12ss59t {

static const uint8_t HCS12SS59T_NUM_DIGITS = 12;

class HCS12SS59TComponent;

using hcs12ss59t_writer_t = display::DisplayWriter<HCS12SS59TComponent>;

class HCS12SS59TComponent final : public PollingComponent,
                                  public spi::SPIDevice<spi::BIT_ORDER_LSB_FIRST, spi::CLOCK_POLARITY_HIGH,
                                                        spi::CLOCK_PHASE_TRAILING, spi::DATA_RATE_1MHZ> {
 public:
  void setup() override;
  void dump_config() override;
  void update() override;
  float get_setup_priority() const override { return setup_priority::PROCESSOR; }

  void set_writer(hcs12ss59t_writer_t &&writer) { this->writer_ = writer; }
  void set_reset_pin(GPIOPin *pin) { this->reset_pin_ = pin; }
  void set_vdon_pin(GPIOPin *pin) { this->vdon_pin_ = pin; }
  void set_intensity(uint8_t intensity);

  uint8_t print(uint8_t pos, const char *str);
  uint8_t print(const char *str) { return this->print(0, str); }
  uint8_t printf(uint8_t pos, const char *format, ...) __attribute__((format(printf, 3, 4)));
  uint8_t printf(const char *format, ...) __attribute__((format(printf, 2, 3)));
  uint8_t strftime(uint8_t pos, const char *format, ESPTime time) __attribute__((format(strftime, 3, 0)));
  uint8_t strftime(const char *format, ESPTime time) __attribute__((format(strftime, 2, 0)));

 protected:
  void display_();
  void send_command_(uint8_t command, uint8_t argument);

  GPIOPin *reset_pin_{nullptr};
  GPIOPin *vdon_pin_{nullptr};
  uint8_t intensity_{15};
  bool intensity_changed_{false};
  char buffer_[HCS12SS59T_NUM_DIGITS];
  hcs12ss59t_writer_t writer_{};
};

}  // namespace esphome::hcs12ss59t
