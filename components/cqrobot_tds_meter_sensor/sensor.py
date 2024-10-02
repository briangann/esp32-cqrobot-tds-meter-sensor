"""Module providing a CQRobot TDS (Total Dissolved Solids) Meter Sensor support SEN0244."""

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import uart, sensor
from esphome.const import ICON_WATER, UNIT_PARTS_PER_MILLION, STATE_CLASS_MEASUREMENT, CONF_PIN
from esphome.cpp_helpers import gpio_pin_expression

DEPENDENCIES = ["uart"]

cqrobot_tds_meter_sensor_ns = cg.esphome_ns.namespace("cqrobot_tds_meter_sensor")
CQRobotTDSMeterSensor = cqrobot_tds_meter_sensor_ns.class_(
    "CQRobotTDSMeterSensor", cg.PollingComponent, uart.UARTDevice
)

CONFIG_SCHEMA = (
    sensor.sensor_schema(
        CQRobotTDSMeterSensor,
        state_class=STATE_CLASS_MEASUREMENT,
        unit_of_measurement=UNIT_PARTS_PER_MILLION,
        icon=ICON_WATER,
        accuracy_decimals=1,
    )
    .extend(
        {
            cv.Required(CONF_PIN): pins.gpio_input_pin_schema,
        }
    )
    .extend(cv.polling_component_schema("5s"))
    .extend(uart.UART_DEVICE_SCHEMA)
)


async def to_code(config):
    """Create and register CQRobotTDSMeterSensor"""
    var = await sensor.new_sensor(config)
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)
    pin = await gpio_pin_expression(config[CONF_PIN])
    cg.add(var.set_pin(pin))
