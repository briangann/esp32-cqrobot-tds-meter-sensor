#include "CQRobotTDS.h"

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/core/hal.h"

#include "esphome/components/uart/uart.h"

namespace esphome
{
  namespace cqrobot_tds_meter_sensor
  {
    class CQRobotTDSMeterSensor : public sensor::Sensor, public PollingComponent, public uart::UARTDevice
    {
    public:
      void set_tds_sensor(sensor::Sensor *value) { tds_sensor_ = value; }
      void set_temperature_sensor(sensor::Sensor *value) { internal_temperature_ = value; }
      void set_pin(InternalGPIOPin *pin);
      void set_adc_range(int range);
      void set_analog_reference_voltage(float voltage);
      void setup() override;
      void update() override;
      void loop() override;
      void dump_config() override;

    protected:
      bool update_{true};
      InternalGPIOPin *pin_{nullptr};
      int adc_range_{1024};
      float analog_reference_voltage_{5.0};
      sensor::Sensor *tds_sensor_{nullptr};
      sensor::Sensor *internal_temperature_{nullptr};
      CQRobotTDS *tds_{nullptr};

    };
  } // namespace cqrobot_tds_meter_sensor
} // namespace esphome
