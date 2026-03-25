import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import output
from esphome.const import CONF_ID, CONF_FREQUENCY
from . import ln882h_pwm_ns

LN882HPWM = ln882h_pwm_ns.class_("LN882HPWM", output.FloatOutput, cg.Component)

CONF_PORT = "port"
CONF_PIN_NUM = "pin_num"
CONF_TIMER_CHANNEL = "timer_channel"

CONFIG_SCHEMA = output.FLOAT_OUTPUT_SCHEMA.extend(
    {
        cv.Required(CONF_ID): cv.declare_id(LN882HPWM),
        cv.Required(CONF_PORT): cv.int_range(min=0, max=1),
        cv.Required(CONF_PIN_NUM): cv.int_range(min=0, max=15),
        cv.Optional(CONF_TIMER_CHANNEL, default=0): cv.int_range(min=0, max=5),
        cv.Optional(CONF_FREQUENCY, default=1000): cv.positive_int,
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await output.register_output(var, config)
    cg.add(var.set_port(config[CONF_PORT]))
    cg.add(var.set_pin_num(config[CONF_PIN_NUM]))
    cg.add(var.set_timer_channel(config[CONF_TIMER_CHANNEL]))
    cg.add(var.set_frequency(config[CONF_FREQUENCY]))
