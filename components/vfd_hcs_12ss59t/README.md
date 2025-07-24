# VFD HCS-12SS59T ESPHome Component

## Overview

This ESPHome custom component enables control of the Samsung HCS-12SS59T Vacuum Fluorescent Display (VFD) via SPI. It supports text display, brightness control, and basic runtime operations. The component is suitable for projects requiring a 12-digit VFD display with adjustable brightness.

### Supported Hardware

- Samsung HCS-12SS59T VFD (12-digit)
- Compatible with ESPHome devices supporting SPI

## YAML Configuration

Add the component to your ESPHome YAML configuration as follows:

```yaml
external_components:
  - source: github://your-repo/components

spi:
  clk_pin: GPIO18
  mosi_pin: GPIO23

vfd_hcs_12ss59t:
  id: vfd1
  cs_pin: GPIO5
  reset_pin: GPIO17         # Optional, recommended for reliable startup
  vdon_pin: GPIO16          # Optional, controls VFD power supply
  brightness: 8             # Optional, 0 (dim) to 15 (bright), default: 4
  update_interval: 500ms    # Optional, display refresh interval, default: 1s
```

### Configuration Options

| Key               | Required | Type    | Description                                         |
|-------------------|----------|---------|-----------------------------------------------------|
| `id`              | Yes      | string  | Component ID                                        |
| `cs_pin`          | Yes      | pin     | Chip select pin for SPI                             |
| `reset_pin`       | No       | pin     | Reset pin for VFD (recommended)                     |
| `vdon_pin`        | No       | pin     | VFD power supply control pin                        |
| `brightness`      | No       | int     | Initial brightness (0-15), default: 4               |
| `update_interval` | No       | time    | Display update interval, default: 1s                |

## Example: Time Display with Brightness Control

```yaml
time:
  - platform: sntp
    id: my_time

number:
  - platform: template
    name: "VFD Brightness"
    id: vfd_brightness
    min_value: 0
    max_value: 15
    step: 1
    initial_value: 8
    set_action:
      then:
        - lambda: |-
            id(vfd1).set_brightness(x);

vfd_hcs_12ss59t:
  id: vfd1
  cs_pin: GPIO5
  brightness: 8

interval:
  - interval: 1s
    then:
      - lambda: |-
          char buf[13];
          snprintf(buf, sizeof(buf), "%02d:%02d:%02d", id(my_time).now().hour, id(my_time).now().minute, id(my_time).now().second);
          id(vfd1).write_text(buf);
```

## Runtime Control

- **Brightness:**  
  The component exposes a number entity for brightness control (0-15). You can adjust brightness at runtime via Home Assistant or ESPHome dashboard.

- **Text Display:**  
  Use the `write_text` method in a lambda or automation to update the display content.

  ```yaml
  - lambda: |-
      id(vfd1).write_text("Hello VFD!");
  ```

## Extensibility & Future Features

- The component is designed for easy extension, such as adding scrolling, custom characters, or additional display effects.
- Future improvements may include:
  - Timer-based scrolling support
  - Custom font or symbol support
  - More advanced Home Assistant service integration

---

For questions or contributions, see the [source code](.) or open an issue in the repository.