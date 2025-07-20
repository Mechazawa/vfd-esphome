import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import display
from esphome.const import CONF_ID
import esphome.components.automation as automation
from . import actions as vfd_actions

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

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID], config[CONF_PIN_RESET], config[CONF_PIN_VDON], config[CONF_PIN_CS])
    await display.register_display(var, config)
    cg.add(var.set_brightness(config[CONF_BRIGHTNESS]))



# --- Actions ---

@automation.register_action(
    "vfd_hcs_12ss59t.write",
    vfd_actions.VFD_WRITE_ACTION_SCHEMA,
    vfd_actions.to_code_write_action,
)
def vfd_write_action_to_code(config, action_id, template_arg, args):
    return vfd_actions.to_code_write_action(config, action_id)

@automation.register_action(
    "vfd_hcs_12ss59t.clear",
    vfd_actions.VFD_CLEAR_ACTION_SCHEMA,
    vfd_actions.to_code_clear_action,
)
def vfd_clear_action_to_code(config, action_id, template_arg, args):
    return vfd_actions.to_code_clear_action(config, action_id)
