import esphome.codegen as cg
from esphome.components import climate_ir, select
import esphome.config_validation as cv
from esphome.const import CONF_ID

AUTO_LOAD = ["climate_ir", "select"]

climate_ir_lg_ns = cg.esphome_ns.namespace("climate_ir_lg")
LgIrClimate = climate_ir_lg_ns.class_("LgIrClimate", climate_ir.ClimateIR)
LgIrLouverVerticalSelect = climate_ir_lg_ns.class_(
    "LgIrLouverVerticalSelect", select.Select
)

CONF_HEADER_HIGH = "header_high"
CONF_HEADER_LOW = "header_low"
CONF_BIT_HIGH = "bit_high"
CONF_BIT_ONE_LOW = "bit_one_low"
CONF_BIT_ZERO_LOW = "bit_zero_low"
CONF_ALTERNATIVE_MODE = "alternative_mode"
CONF_LOUVER_FIXED_VERTICAL_POSITION = "louver_fixed_vertical_position"

SWING_POSITION_OPTIONS = [
    "Lowest",
    "Low",
    "Middle-Low",
    "Middle-High",
    "High",
    "Highest",
]

CONFIG_SCHEMA = climate_ir.climate_ir_with_receiver_schema(LgIrClimate).extend(
    {
        cv.Optional(CONF_ALTERNATIVE_MODE, default=False): cv.boolean,
        cv.Optional(
            CONF_HEADER_HIGH, default="8000us"
        ): cv.positive_time_period_microseconds,
        cv.Optional(
            CONF_HEADER_LOW, default="4000us"
        ): cv.positive_time_period_microseconds,
        cv.Optional(
            CONF_BIT_HIGH, default="600us"
        ): cv.positive_time_period_microseconds,
        cv.Optional(
            CONF_BIT_ONE_LOW, default="1600us"
        ): cv.positive_time_period_microseconds,
        cv.Optional(
            CONF_BIT_ZERO_LOW, default="550us"
        ): cv.positive_time_period_microseconds,
        cv.Optional(CONF_LOUVER_FIXED_VERTICAL_POSITION): select.select_schema(
            LgIrLouverVerticalSelect,
            icon="mdi:arrow-expand-vertical",
        ),
    }
)


async def to_code(config):
    var = await climate_ir.new_climate_ir(config)

    cg.add(var.set_alternative_mode(config[CONF_ALTERNATIVE_MODE]))
    cg.add(var.set_header_high(config[CONF_HEADER_HIGH]))
    cg.add(var.set_header_low(config[CONF_HEADER_LOW]))
    cg.add(var.set_bit_high(config[CONF_BIT_HIGH]))
    cg.add(var.set_bit_one_low(config[CONF_BIT_ONE_LOW]))
    cg.add(var.set_bit_zero_low(config[CONF_BIT_ZERO_LOW]))

    if swing_pos_config := config.get(CONF_LOUVER_FIXED_VERTICAL_POSITION):
        sel = await select.new_select(
            swing_pos_config,
            options=SWING_POSITION_OPTIONS,
        )
        await cg.register_parented(sel, config[CONF_ID])
        cg.add(var.set_louver_fixed_vertical_position_select(sel))
