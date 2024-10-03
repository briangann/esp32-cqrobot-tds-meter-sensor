"""Module providing a CQRobot TDS (Total Dissolved Solids) Meter Sensor support SEN0244."""

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import sensor
from esphome.const import (
    UNIT_PARTS_PER_MILLION,
    STATE_CLASS_MEASUREMENT,
    CONF_PIN,
    DEVICE_CLASS_VOLATILE_ORGANIC_COMPOUNDS_PARTS,
)
from esphome.cpp_helpers import gpio_pin_expression

#DEPENDENCIES = ['dallas_temp']

CONF_ADC_RANGE = "adc_range"
CONF_ANALOG_REFERENCE_VOLTAGE = "analog_reference_voltage"

cqrobot_tds_meter_sensor_ns = cg.esphome_ns.namespace("cqrobot_tds_meter_sensor")
CQRobotTDSMeterSensor = cqrobot_tds_meter_sensor_ns.class_(
    "CQRobotTDSMeterSensor", cg.PollingComponent
)

CONFIG_SCHEMA = (
    sensor.sensor_schema(
        CQRobotTDSMeterSensor,
        unit_of_measurement=UNIT_PARTS_PER_MILLION,
        icon="hass:water-opacity",
        accuracy_decimals=0,
        device_class=DEVICE_CLASS_VOLATILE_ORGANIC_COMPOUNDS_PARTS,
        state_class=STATE_CLASS_MEASUREMENT,
    )
    .extend(
        {
            cv.Required(CONF_PIN): pins.gpio_input_pin_schema,
            cv.Optional(CONF_ADC_RANGE, default="1024"): cv.int_,
            cv.Optional(CONF_ANALOG_REFERENCE_VOLTAGE, default="5.0"): cv.float_,
        }
    )
    .extend(cv.polling_component_schema("5s"))
)


async def to_code(config):
    """Create and register CQRobotTDSMeterSensor"""
    var = await sensor.new_sensor(config)
    await cg.register_component(var, config)
    pin = await gpio_pin_expression(config[CONF_PIN])
    cg.add(var.set_pin(pin))
    cg.add(var.set_adc_range(config[CONF_ADC_RANGE]))
    cg.add(var.set_analog_reference_voltage(config[CONF_ANALOG_REFERENCE_VOLTAGE]))
