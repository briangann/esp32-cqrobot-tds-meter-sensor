#include "esphome/core/log.h"
#include "cqrobot_tds_meter_sensor.h"
#include "CQRobotTDS.h"

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
        float analogReferenceVoltage = 3.3;
        //CQRobotTDS tds(36, analogReferenceVoltage); // A0 = 36
        CQRobotTDS tds(A0); // A0 = 36
        unsigned long timeout = 0;

        float temperature = 25;
        int adcRange = 4096;
        float tdsValue = 0;

        void CQRobotTDSMeterSensor::setup()
        {
            //tds = CQRobotTDS(36, analogReferenceVoltage); // A0 = 36

            ESP_LOGCONFIG(TAG, "Setting up CQRobot TDS Meter Sensor '%s'...", this->get_name().c_str());
            Serial.begin(115200); // Initialize serial port (just writing log info to it)
            // figure out how to get the pin from config
            //LOG_PIN("  Pin: ", this->pin_);
            //this->pin_->setup();
            //tds.setMyPin(this->pin_);
            //ESP_LOGCONFIG(TAG, "PIN '%d'", this->pin_);
            ESP_LOGCONFIG(TAG, "ADC RANGE '%d'", adcRange);
            ESP_LOGCONFIG(TAG, "Analog Reference Voltage '%g'", analogReferenceVoltage);
            tds.setAdcRange(adcRange); // 1024 = 10 bit, 4096 = 12 bit
            //tds.setMyPin(1);
        }

        void CQRobotTDSMeterSensor::update()
        {
            tdsValue = tds.getTdsValue();
            ESP_LOGI(TAG, "UPDATE: %g ppm ", tdsValue); // display value,
            this->publish_state(tdsValue);
        }

        void CQRobotTDSMeterSensor::loop()
        {
            if (!this->update_)
                return;
            tdsValue = tds.update(temperature);

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

        void CQRobotTDSMeterSensor::set_pin(GPIOPin *pin)
        {
            ESP_LOGCONFIG(TAG, "CQRobot TDS Meter Sensor: set_custom_pin");
            this->pin_ = pin;
        }

    } // namespace cqrobot_tds_meter_sensor
} // namespace esphome
