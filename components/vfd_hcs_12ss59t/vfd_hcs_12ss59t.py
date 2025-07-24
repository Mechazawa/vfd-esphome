import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import spi, number
from esphome.const import (
    CONF_ID,
    CONF_CS_PIN,
    CONF_RESET_PIN,
    CONF_UPDATE_INTERVAL,
)

CODEOWNERS = ["@your-github"]
DEPENDENCIES = ["spi"]

vfd_ns = cg.esphome_ns.namespace("vfd_hcs_12ss59t")
VFDHCS12SS59TComponent = vfd_ns.class_("VFDHCS12SS59TComponent", cg.Component, spi.SPIDevice)

CONF_VDON_PIN = "vdon_pin"
CONF_BRIGHTNESS = "brightness"

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(VFDHCS12SS59TComponent),
            cv.Required(CONF_CS_PIN): cv.pin,
            cv.Optional(CONF_RESET_PIN): cv.pin,
            cv.Optional(CONF_VDON_PIN): cv.pin,
            cv.Optional(CONF_BRIGHTNESS, default=4): cv.int_range(min=0, max=15),
            cv.Optional(CONF_UPDATE_INTERVAL, default="1s"): cv.positive_time_period_milliseconds,
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(spi.spi_device_schema())
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await spi.register_spi_device(var, config)

    cg.add(var.set_cs_pin(config[CONF_CS_PIN]))
    if CONF_RESET_PIN in config:
        cg.add(var.set_reset_pin(config[CONF_RESET_PIN]))
    if CONF_VDON_PIN in config:
        cg.add(var.set_vdon_pin(config[CONF_VDON_PIN]))
    cg.add(var.set_brightness(config[CONF_BRIGHTNESS]))
    cg.add(var.set_update_interval(config[CONF_UPDATE_INTERVAL].total_milliseconds))

    # Register brightness number entity for runtime control
    num = await number.new_number(
        {
            CONF_ID: f"{config[CONF_ID]}_brightness",
            number.CONF_MIN_VALUE: 0,
            number.CONF_MAX_VALUE: 15,
            number.CONF_STEP: 1,
            number.CONF_INITIAL_VALUE: config[CONF_BRIGHTNESS],
            number.CONF_MODE: number.MODE_SLIDER,
        }
    )
    cg.add(var.set_brightness_number(num))