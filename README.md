# HCS-12SS59T VFD for ESPHome

External ESPHome component for the Samsung HCS-12SS59T, a 12-digit 16-segment vacuum fluorescent display with an SPI interface.

## Usage

```yaml
external_components:
  - source: github://Mechazawa/vfd-esphome@master
    components: [hcs12ss59t]

spi:
  clk_pin: GPIO14
  mosi_pin: GPIO13

display:
  - platform: hcs12ss59t
    cs_pin: GPIO15
    reset_pin: GPIO5
    vdon_pin: GPIO4
    intensity: 12
    lambda: |-
      it.strftime("%H:%M:%S", id(sntp_time).now());
```

| Option | Default | |
|---|---|---|
| `cs_pin` | required | Chip select (`/CS`). |
| `reset_pin` | none | `/RESET`, active low. Leave out when it is tied high. |
| `vdon_pin` | none | `/VDON`, switches the display supply, active low. Leave out when it is tied low. |
| `intensity` | `15` | Brightness, `0` to `15`. |
| `update_interval` | `1s` | How often the lambda runs. |

The lambda gets `it` with the same text API as the built-in `max7219` display:

- `it.print([pos,] "text")`
- `it.printf([pos,] "format", ...)`
- `it.strftime([pos,] "format", time)`
- `it.set_intensity(0..15)`

`pos` counts digits from the left. Text past digit 12 is cut off. The character ROM has no lowercase, so lowercase letters show as uppercase, and characters outside the ROM show as `?`.

The init sequence and character mapping come from the Arduino driver by qrt@qland.de.

## Testing

```sh
esphome compile tests/test.esp8266.yaml
```
