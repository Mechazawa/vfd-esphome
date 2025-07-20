# vfd_hcs_12ss59t

ESPHome custom component for Samsung HCS-12SS59T VFD display.

## Files
- `vfd_hcs_12ss59t.h` / `vfd_hcs_12ss59t.cpp`: C++ implementation
- `vfd_hcs_12ss59t.py`: Python interface for ESPHome

## Usage
Add to your ESPHome `custom_component` as described in the ESPHome docs.

## Example device configuration

```yaml
esphome:
  name: vfd_clock
  platform: ESP32
  board: esp32dev

external_components:
  - source: github://your-github-username/vfd_hcs_12ss59t
    components: [vfd_hcs_12ss59t]

spi:
  clk_pin: GPIO18
  mosi_pin: GPIO23

time:
  - platform: sntp
    id: sntp_time

display:
  - platform: vfd_hcs_12ss59t
    id: vfd_display
    pin_reset: 5
    pin_vdon: 4
    pin_cs: 15
    brightness: 80%
    lambda: |-
      char buf[13];
      auto t = id(sntp_time).now();
      snprintf(buf, sizeof(buf), "%02d:%02d:%02d", t.hour, t.minute, t.second);
      it.printf(0, 0, "%s", buf);
```
