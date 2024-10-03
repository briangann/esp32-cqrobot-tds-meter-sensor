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

        float tdsValue = 0;

        void CQRobotTDSMeterSensor::setup()
        {
            ESP_LOGCONFIG(TAG, "Setting up %s-%s TDS Meter '%s'...", this->vendor_.c_str(), this->model_.c_str(), this->get_name().c_str());
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
            if (checkTDSValue >= 0.0)
            {
                tdsValue = checkTDSValue;
                int roundedValue = (int)round(tdsValue);
                ESP_LOGI(TAG, "UPDATE: %s-%s TDS Meter -> %d ppm (%g raw value) (using %g Celsius)",
                    this->vendor_.c_str(), this->model_.c_str(),
                    roundedValue, tdsValue, this->internal_temperature_);
                this->publish_state(roundedValue);
            }
            else
            {
                ESP_LOGI(TAG, "UPDATE: (skipped) ");
            }
        }

        void CQRobotTDSMeterSensor::loop()
        {
            if (!this->update_)
                return;
            tdsValue = this->tds_->update(this->internal_temperature_);

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
            ESP_LOGCONFIG(TAG, "%s-%s TDS Meter", this->vendor_.c_str(), this->model_.c_str());
            LOG_SENSOR("", "", this);
            LOG_UPDATE_INTERVAL(this);
        }

        void CQRobotTDSMeterSensor::set_vendor(const std::string &value)
        {
            this->vendor_ = value;
            ESP_LOGI(TAG, "TDS Meter: vendor set to %s", this->vendor_.c_str());
        }
        void CQRobotTDSMeterSensor::set_model(const std::string &value)
        {
            this->model_ = value;
            ESP_LOGI(TAG, "TDS Meter: model set to %s", this->model_.c_str());
        }

        void CQRobotTDSMeterSensor::set_temperature(float val)
        {
            ESP_LOGI(TAG, "%s-%s TDS Meter: received temperature update %g", this->vendor_.c_str(), this->model_.c_str(), val);
            this->internal_temperature_ = val;
        }

        void CQRobotTDSMeterSensor::set_pin(InternalGPIOPin *pin)
        {
            ESP_LOGCONFIG(TAG, "TDS Meter: set_pin");
            this->pin_ = pin;
        }
        void CQRobotTDSMeterSensor::set_adc_range(int range)
        {
            ESP_LOGCONFIG(TAG, "TDS Meter: set_adc_range");
            this->adc_range_ = range;
        }
        void CQRobotTDSMeterSensor::set_analog_reference_voltage(float voltage)
        {
            ESP_LOGCONFIG(TAG, "TDS Meter: set_analog_reference_voltage");
            this->analog_reference_voltage_ = voltage;
        }

    } // namespace cqrobot_tds_meter_sensor
} // namespace esphome
