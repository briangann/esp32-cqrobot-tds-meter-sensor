#include "esphome/core/log.h"
#include "cqrobot_tds_meter_sensor.h"

namespace esphome
{
    namespace cqrobot_tds_meter_sensor
    {
        static const char *TAG = "cqrobot_tds_meter.sensor";
        /**
         * A0	GPIO 36
         * A3	GPIO 39
         * A4	GPIO 32
         * A5	GPIO 33
         * A6	GPIO 34
         * A7	GPIO 35
         */
        unsigned long timeout = 0;

        float temperature = 23;
        float tdsValue = 0;

        void CQRobotTDSMeterSensor::setup()
        {
            ESP_LOGCONFIG(TAG, "Setting up CQRobot TDS Meter Sensor '%s'...", this->get_name().c_str());
            Serial.begin(115200); // Initialize serial port (just writing log info to it)
            ESP_LOGCONFIG(TAG, "PIN '%d'", this->pin_->get_pin());
            ESP_LOGCONFIG(TAG, "ADC RANGE '%d'", this->adc_range_);
            ESP_LOGCONFIG(TAG, "Analog Reference Voltage '%g'", this->analog_reference_voltage_);
            this->tds_ = new CQRobotTDS(this->pin_->get_pin(), this->analog_reference_voltage_);
            this->tds_->setAdcRange(this->adc_range_); // 1024 = 10 bit, 4096 = 12 bit
        }

        void CQRobotTDSMeterSensor::update()
        {
            float checkTDSValue = this->tds_->getTdsValue();
            if (checkTDSValue >= 0.0) {
                tdsValue = checkTDSValue;
                ESP_LOGI(TAG, "UPDATE: %g ppm ", tdsValue);
                this->publish_state(tdsValue);
            } else
            {
                ESP_LOGI(TAG, "UPDATE: (skipped) ");
            }
        }

        void CQRobotTDSMeterSensor::loop()
        {
            if (!this->update_)
                return;
            tdsValue = this->tds_->update(temperature);

            if (timeout < millis())
            {
                Serial.print("TDS value: ");
                Serial.print(tdsValue, 0);
                Serial.println(" ppm");
                timeout = millis() + 10000;
            }
        }

        void CQRobotTDSMeterSensor::dump_config()
        {
            ESP_LOGCONFIG(TAG, "CQRobot TDS Meter Sensor");
            LOG_SENSOR("", "", this);
            LOG_UPDATE_INTERVAL(this);
        }

        void CQRobotTDSMeterSensor::set_pin(InternalGPIOPin *pin)
        {
            ESP_LOGCONFIG(TAG, "CQRobot TDS Meter Sensor: set_pin");
            this->pin_ = pin;
        }
        void CQRobotTDSMeterSensor::set_adc_range(int range)
        {
            ESP_LOGCONFIG(TAG, "CQRobot TDS Meter Sensor: set_adc_range");
            this->adc_range_ = range;
        }
        void CQRobotTDSMeterSensor::set_analog_reference_voltage(float voltage)
        {
            ESP_LOGCONFIG(TAG, "CQRobot TDS Meter Sensor: set_analog_reference_voltage");
            this->analog_reference_voltage_ = voltage;
        }

    } // namespace cqrobot_tds_meter_sensor
} // namespace esphome
