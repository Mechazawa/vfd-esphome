import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import display
from esphome.const import CONF_ID

CODEOWNERS = ["@your-github-username"]
DEPENDENCIES = ["spi"]

vfd_ns = cg.esphome_ns.namespace("vfd_hcs_12ss59t")
VFDHCS12SS59T = vfd_ns.class_("VFDHCS12SS59T", display.DisplayBuffer)

CONF_PIN_RESET = "pin_reset"
CONF_PIN_VDON = "pin_vdon"
CONF_PIN_CS = "pin_cs"
CONF_BRIGHTNESS = "brightness"

CONFIG_SCHEMA = display.BASIC_DISPLAY_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(VFDHCS12SS59T),
    cv.Required(CONF_PIN_RESET): cv.int_,
    cv.Required(CONF_PIN_VDON): cv.int_,
    cv.Required(CONF_PIN_CS): cv.int_,
    cv.Optional(CONF_BRIGHTNESS, default=1.0): cv.percentage,
})

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID], config[CONF_PIN_RESET], config[CONF_PIN_VDON], config[CONF_PIN_CS])
    yield display.register_display(var, config)
    cg.add(var.set_brightness(config[CONF_BRIGHTNESS]))
