import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import spi
from esphome.const import CONF_ID

CODEOWNERS = ["@your-github-username"]
DEPENDENCIES = ["spi"]

vfd_ns = cg.esphome_ns.namespace("vfd_hcs_12ss59t")
VFDHCS12SS59T = vfd_ns.class_("VFDHCS12SS59T", cg.Component)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(VFDHCS12SS59T),
    cv.Required("pin_reset"): cv.int_,
    cv.Required("pin_vdon"): cv.int_,
    cv.Required("pin_cs"): cv.int_,
}).extend(cv.COMPONENT_SCHEMA)

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID], config["pin_reset"], config["pin_vdon"], config["pin_cs"])
    yield cg.register_component(var, config)
