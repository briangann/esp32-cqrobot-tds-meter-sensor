# ESPHome CQRobot TDS Meter Sensor Component

For CQRobot model CQRSENTDS01M, with a dallas temperature sensor input to adjust readings.

Outputs PPM

TODO:

- Output raw voltage reading
- Convert to generic naming


## Example Configuration

```YAML

esphome:
  name: esp32-pondsensors-1
  libraries:
    - CQRobotTDS=https://github.com/briangann/esp32-cqrobot-tds-meter-sensor.git#main

external_components:
  - source:
      type: git
      url: https://github.com/briangann/esp32-cqrobot-tds-meter-sensor
      ref: main
    components: [ cqrobot_tds_meter_sensor ]

one_wire:
  - platform: gpio
    pin: 19
    id: hub_1

sensor:
  - platform: dallas_temp
    one_wire_id: hub_1
    name: "Pond Upper Zone Temperature C"
    id: pond_upper_zone_temperature_c
    address: 0x723c56f6493d0528
    accuracy_decimals: 5
    unit_of_measurement: °C
    on_value:
      then:
        - lambda: |-
            id(tds_meter).set_temperature(id(pond_upper_zone_temperature_c).state);

  - platform: cqrobot_tds_meter_sensor
    name: CQRobot TDS Meter
    id: tds_meter
    update_interval: 15s
    accuracy_decimals: 0
    pin: 34
    adc_range: 4096
    analog_reference_voltage: 1.1
    vendor: CQRobot
    model: CQRSENTDS01M
```
