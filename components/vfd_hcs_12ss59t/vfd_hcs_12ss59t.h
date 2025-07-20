
/**
 * @file vfd_hcs_12ss59t.h
 * @brief ESPHome component for Samsung HCS-12SS59T VFD
 * @author (your name)
 * @license MIT
 */

#pragma once


#include "esphome/components/display/display_buffer.h"
#include "esphome/core/automation.h"
#include <SPI.h>
#include "vfd_hcs_12ss59t_action.h"



constexpr uint32_t SPIPARS = 2000000;
constexpr uint8_t NUMDIGITS = 12;
constexpr uint8_t BUFSIZE = 100;

/// VFD command set
enum class VFDCommand : uint8_t {
  DCRAM_WR = 0x10,
  CGRAM_WR = 0x20,
  ADRAM_WR = 0x30,
  DUTY = 0x50,
  NUMDIGIT = 0x60,
  LIGHTS = 0x70
};

/// VFD light states
enum class VFDLight : uint8_t {
  NORM = 0x00,
  OFF = 0x01,
  ON = 0x02
};




/**
 * @class VFDHCS12SS59T
 * @brief Driver for Samsung HCS-12SS59T VFD, ESPHome display component
 */
namespace esphome {
namespace vfd_hcs_12ss59t {

class VFDHCS12SS59T : public esphome::display::DisplayBuffer {
 public:
  /**
   * @brief Construct a new VFDHCS12SS59T object
   * @param pinReset Reset pin
   * @param pinVdon VFD power pin
   * @param pinCs Chip select pin
   */
  VFDHCS12SS59T(uint8_t pinReset, uint8_t pinVdon, uint8_t pinCs);

  void setup() override;
  void dump_config() override;
  void update() override;
  void draw_absolute_pixel_internal(int x, int y, esphome::Color color) override;
  int get_width() const { return NUMDIGITS * 8; }  ///< 8 pixels per char
  int get_height() const { return 16; }            ///< 16 pixels high
  display::DisplayType get_display_type() override { return display::DisplayType(); }
  int get_width_internal() override { return get_width(); }
  int get_height_internal() override { return get_height(); }

  /**
   * @brief Set display brightness (0.0-1.0)
   */
  void set_brightness(float brightness);

  /**
   * @brief Clear the display buffer
   */
  void clear();

  /**
   * @brief Write a string to the display buffer
   * @param text Null-terminated string
   */
  void write(const char *text);

 protected:
  char buf[BUFSIZE] = {};
  uint8_t pin_reset;
  uint8_t pin_vdon;
  uint8_t pin_cs;
  int16_t scrLen = NUMDIGITS;
  int16_t scrPos = 0;
  int16_t scrMode = 0;
  float brightness_ = 1.0f;

  void select(uint8_t pin);
  void deSelect(uint8_t pin);
  void sendCmd(VFDCommand cmd, uint8_t arg);
  void sendCmdSeq(VFDCommand cmd, uint8_t arg);
  void sendChar(char c);
  char getCode(char c) const;
};

}  // namespace vfd_hcs_12ss59t
}  // namespace esphome
