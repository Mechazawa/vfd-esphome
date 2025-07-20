import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID
import voluptuous as vol

vfd_ns = cg.esphome_ns.namespace("vfd_hcs_12ss59t")
VFDHCS12SS59T = vfd_ns.class_("VFDHCS12SS59T")

VFD_WRITE_ACTION_SCHEMA = cv.Schema({
    cv.Required(CONF_ID): cv.use_id(VFDHCS12SS59T),
    vol.Required("text"): cv.string,
})

async def to_code_write_action(config, action_id):
    var = await cg.get_variable(config[CONF_ID])
    return cg.new_Pvariable(action_id, var, config["text"])

VFD_CLEAR_ACTION_SCHEMA = cv.Schema({
    cv.Required(CONF_ID): cv.use_id(VFDHCS12SS59T),
})

async def to_code_clear_action(config, action_id):
    var = await cg.get_variable(config[CONF_ID])
    return cg.new_Pvariable(action_id, var)
