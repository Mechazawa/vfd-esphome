from esphome import pins
import esphome.codegen as cg
from esphome.components import display, spi
import esphome.config_validation as cv
from esphome.const import CONF_ID, CONF_INTENSITY, CONF_RESET_PIN

DEPENDENCIES = ["spi"]

CONF_VDON_PIN = "vdon_pin"

hcs12ss59t_ns = cg.esphome_ns.namespace("hcs12ss59t")
HCS12SS59TComponent = hcs12ss59t_ns.class_(
    "HCS12SS59TComponent", cg.PollingComponent, spi.SPIDevice
)
HCS12SS59TComponentRef = HCS12SS59TComponent.operator("ref")

CONFIG_SCHEMA = (
    display.BASIC_DISPLAY_SCHEMA.extend(
        {
            cv.GenerateID(): cv.declare_id(HCS12SS59TComponent),
            cv.Optional(CONF_RESET_PIN): pins.gpio_output_pin_schema,
            cv.Optional(CONF_VDON_PIN): pins.gpio_output_pin_schema,
            cv.Optional(CONF_INTENSITY, default=15): cv.int_range(min=0, max=15),
        }
    )
    .extend(cv.polling_component_schema("1s"))
    .extend(spi.spi_device_schema())
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await spi.register_spi_device(var, config, write_only=True)
    await display.register_display(var, config)

    cg.add(var.set_intensity(config[CONF_INTENSITY]))

    if CONF_RESET_PIN in config:
        cg.add(var.set_reset_pin(await cg.gpio_pin_expression(config[CONF_RESET_PIN])))
    if CONF_VDON_PIN in config:
        cg.add(var.set_vdon_pin(await cg.gpio_pin_expression(config[CONF_VDON_PIN])))

    await display.add_writer(var, config, HCS12SS59TComponentRef)
