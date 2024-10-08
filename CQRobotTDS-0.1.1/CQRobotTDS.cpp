#include "CQRobotTDS.h"

CQRobotTDS::CQRobotTDS(int pin, float aref)
: pin(pin)
, aref(aref)
, adcRange(1024.0)
{
}

CQRobotTDS::~CQRobotTDS()
{
}

float CQRobotTDS::update(float temp) //read and calculate
{
	setTemperature(temp);
	return update();
}

float CQRobotTDS::update() //read and calculate
{
	static unsigned long analogSampleTimepoint = millis();
	if (millis() - analogSampleTimepoint > 40U)  //every 40 milliseconds,read the analog value from the ADC
	{
		analogSampleTimepoint = millis();
		this->analogBuffer[this->analogBufferIndex] = analogRead(this->pin);    //read the analog value and store into the buffer
		this->analogBufferIndex++;
		if (this->analogBufferIndex == CQROBOT_SCOUNT)
			this->analogBufferIndex = 0;
	}
	static unsigned long printTimepoint = millis();
	if (millis() - printTimepoint > 800U)
	{
		printTimepoint = millis();
		for (copyIndex = 0; copyIndex < CQROBOT_SCOUNT; copyIndex++)
			this->analogBufferTemp[copyIndex] = this->analogBuffer[copyIndex];
		averageVoltage = getMedianNum(analogBufferTemp, CQROBOT_SCOUNT) * this->aref / this->adcRange; // read the analog value more stable by the median filtering algorithm, and convert to voltage value
		float compensationCoefficient = 1.0 + 0.02 * (this->temperature - 25.0); //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
		float compensationVoltage = averageVoltage / compensationCoefficient; //temperature compensation
		tdsValue = (133.42 * compensationVoltage * compensationVoltage * compensationVoltage - 255.86 * compensationVoltage * compensationVoltage + 857.39 * compensationVoltage) * 0.5; //convert voltage value to tds value
	}
	return tdsValue;
}

void CQRobotTDS::setTemperature(float temp)  //set the temperature and execute temperature compensation
{
	this->temperature = temp;
}

void CQRobotTDS::setAdcRange(float range)  //1024 for 10bit ADC;4096 for 12bit ADC
{
	this->adcRange = range;
}

float CQRobotTDS::getTdsValue()
{
	return tdsValue;
}

int CQRobotTDS::getMedianNum(int bArray[], int iFilterLen)
{
	int bTab[iFilterLen];
	for (byte i = 0; i < iFilterLen; i++)
		bTab[i] = bArray[i];
	int i, j, bTemp;
	for (j = 0; j < iFilterLen - 1; j++)
	{
		for (i = 0; i < iFilterLen - j - 1; i++)
		{
			if (bTab[i] > bTab[i + 1])
			{
				bTemp = bTab[i];
				bTab[i] = bTab[i + 1];
				bTab[i + 1] = bTemp;
			}
		}
	}
	if ((iFilterLen & 1) > 0)
		bTemp = bTab[(iFilterLen - 1) / 2];
	else
		bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
	return bTemp;
}
