// vfd_hcs_12ss59t.h - ESPHome component for Samsung HCS-12SS59T VFD
#pragma once

#include "esphome.h"
#include <SPI.h>
#include "esphome/components/display/display_buffer.h"

#define SPIPARS 2000000, LSBFIRST, SPI_MODE3
#define VFD_DCRAM_WR 0x10
#define VFD_CGRAM_WR 0x20
#define VFD_ADRAM_WR 0x30
#define VFD_DUTY 0x50
#define VFD_NUMDIGIT 0x60
#define VFD_LIGHTS 0x70
#define LINORM 0x00
#define LIOFF 0x01
#define LION 0x02
#define NUMDIGITS 12
#define BUFSIZE 100

namespace esphome {
namespace vfd_hcs_12ss59t {

class VFDHCS12SS59T : public display::DisplayBuffer {
 public:
  VFDHCS12SS59T(int pinReset, int pinVdon, int pinCs);
  void setup() override;
  void dump_config() override;
  void update() override;
  void draw_absolute_pixel_internal(int x, int y, Color color) override;
  int get_width() override { return NUMDIGITS * 8; } // 8 pixels per char (example)
  int get_height() override { return 16; } // 16 pixels high (example)
  void set_brightness(float brightness);

 protected:
  char buf[BUFSIZE];
  int pin_reset;
  int pin_vdon;
  int pin_cs;
  int16_t scrLen;
  int16_t scrPos;
  int16_t scrMode;
  float brightness_ = 1.0f;

  void select(int pin);
  void deSelect(int pin);
  void sendCmd(char cmd, char arg);
  void sendCmdSeq(char cmd, char arg);
  void sendChar(char c);
  char getCode(char c);
};

}  // namespace vfd_hcs_12ss59t
}  // namespace esphome
