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

## Sample Output

```SHELL
[19:48:45][C][gpio.one_wire:020]: GPIO 1-wire bus:
[19:48:45][C][gpio.one_wire:021]:   Pin: GPIO19
[19:48:45][C][gpio.one_wire:080]:   Found devices:
[19:48:45][C][gpio.one_wire:082]:     0x723c56f6493d0528 (DS18B20)
[19:48:45][C][cqrobot_tds_meter.sensor:067]: CQRobot-CQRSENTDS01M TDS Meter
[19:48:45][C][cqrobot_tds_meter.sensor:068]:  'CQRobot TDS Meter'
[19:48:45][C][cqrobot_tds_meter.sensor:068]:   Device Class: 'volatile_organic_compounds_parts'
[19:48:45][C][cqrobot_tds_meter.sensor:068]:   State Class: 'measurement'
[19:48:45][C][cqrobot_tds_meter.sensor:068]:   Unit of Measurement: 'ppm'
[19:48:45][C][cqrobot_tds_meter.sensor:068]:   Accuracy Decimals: 0
[19:48:45][C][cqrobot_tds_meter.sensor:068]:   Icon: 'hass:water-opacity'
[19:48:45][C][cqrobot_tds_meter.sensor:069]:   Update Interval: 15.0s
[19:48:45][C][dallas.temp.sensor:029]: Dallas Temperature Sensor:
[19:48:45][C][dallas.temp.sensor:034]:   Address: 0x723c56f6493d0528 (DS18B20)
[19:48:45][C][dallas.temp.sensor:035]:   Resolution: 12 bits
[19:48:45][C][dallas.temp.sensor:036]:   Update Interval: 60.0s
...
[19:48:57][I][cqrobot_tds_meter.sensor:041]: UPDATE: CQRobot-CQRSENTDS01M TDS Meter -> 150 ppm (149.697 raw value) (using 23.6875 Celsius)
[19:48:57][D][sensor:094]: 'CQRobot TDS Meter': Sending state 150.00000 ppm with 0 decimals of accuracy
[19:49:12][I][cqrobot_tds_meter.sensor:041]: UPDATE: CQRobot-CQRSENTDS01M TDS Meter -> 150 ppm (149.697 raw value) (using 23.6875 Celsius)
[19:49:12][D][sensor:094]: 'CQRobot TDS Meter': Sending state 150.00000 ppm with 0 decimals of accuracy
```
