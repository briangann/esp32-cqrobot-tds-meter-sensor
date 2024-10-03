#include "CQRobotTDS.h"

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/core/hal.h"

namespace esphome
{
  namespace cqrobot_tds_meter_sensor
  {
    class CQRobotTDSMeterSensor : public sensor::Sensor, public PollingComponent
    {
    public:
      void set_tds_sensor(sensor::Sensor *value) { tds_sensor_ = value; }
      void set_temperature(float val);
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
      float internal_temperature_{25.0};
      CQRobotTDS *tds_{nullptr};

    };
  } // namespace cqrobot_tds_meter_sensor
} // namespace esphome
