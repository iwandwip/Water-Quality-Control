/*
 *  dstemperature-sens.cpp
 *
 *  dstemperature sensor c
 *  Created on: 2023. 4. 3
 */

#include "dstemperature-sens.h"
#include "Arduino.h"
#include "sensor-filter.h"

#define SENSOR_FILTER_KF 8

TemperatureSens::TemperatureSens()
  : sensorPin(A0), isCalibrate(false) {
}

TemperatureSens::TemperatureSens(uint8_t __pin, bool enableCalibrate) {
        this->sensorPin = __pin;
        isCalibrate = enableCalibrate;
}

TemperatureSens::~TemperatureSens() {
        delete this;
}

void TemperatureSens::init() {
        oneWireClass = new OneWire(sensorPin);
        sensorClass = new DallasTemperature(oneWireClass);
        sensorClass->begin();
}

void TemperatureSens::update() {
        if (millis() - sensTimer[0] >= 3000) {
                sensorClass->requestTemperatures();
                if (!isCalibrate) {
                        temperatureValue = sensorClass->getTempCByIndex(0);
                        temperatureValue = temperatureValue + (temperatureValue * SENSOR_FILTER_KF);
                        temperatureValue /= SENSOR_FILTER_KF + 1;
                } else {
                        KalmanFilter* sensorKf = new KalmanFilter(2, 2, 0.01);
                        arrTemperatureValue[SENS_RET_RAW_DATA] = sensorClass->getTempCByIndex(0);
                        arrTemperatureValue[SENS_RET_ACT_DATA] = arrTemperatureValue[SENS_RET_RAW_DATA] + (arrTemperatureValue[SENS_RET_RAW_DATA] * SENSOR_FILTER_KF);
                        arrTemperatureValue[SENS_RET_ACT_DATA] /= SENSOR_FILTER_KF + 1;

                        arrTemperatureValue[SENS_RET_AVG_DATA] = getSensorAverage(arrTemperatureValue[SENS_RET_ACT_DATA]);
                        // arrTemperatureValue[SENS_RET_FILTERED_DATA] = lowPassFilter(arrTemperatureValue[SENS_RET_ACT_DATA], arrTemperatureValue[SENS_RET_FILTERED_DATA], 0.1);
                        arrTemperatureValue[SENS_RET_FILTERED_DATA] = sensorKf->updateEstimate(arrTemperatureValue[SENS_RET_ACT_DATA]);
                        delete sensorKf;
                }
                sensTimer[0] = millis();
        }
        // delete oneWireClass;
        // delete sensorClass;
        // oneWireClass = nullptr;
        // sensorClass = nullptr;
}

void TemperatureSens::debug() {
        if (millis() - sensTimer[1] >= 500) {
                if (isCalibrate) return;
                Serial.print("| temperatureValueRaw: ");
                Serial.print(temperatureValue);
                Serial.println();
                sensTimer[1] = millis();
        }
}

void TemperatureSens::calibrate() {
        if (millis() - sensTimer[2] >= 500) {
                if (!isCalibrate) return;
                Serial.print("| arrTemperatureValueRaw: ");
                Serial.print(arrTemperatureValue[SENS_RET_RAW_DATA]);
                Serial.print("| arrTemperatureValueAct: ");
                Serial.print(arrTemperatureValue[SENS_RET_ACT_DATA]);
                Serial.print("| arrTemperatureValueAvg: ");
                Serial.print(arrTemperatureValue[SENS_RET_AVG_DATA]);
                Serial.print("| arrTemperatureValueFiltered: ");
                Serial.print(arrTemperatureValue[SENS_RET_FILTERED_DATA]);
                Serial.println();
                sensTimer[2] = millis();
        }
}

void TemperatureSens::getValue(float* output) {
        *output = temperatureValue;
}

void TemperatureSens::getValue(int* output) {
}

void TemperatureSens::getValue(char* output) {
}

void TemperatureSens::setCallBack(void (*callbackFunc)(void)) {
        thisCallbackFunc = callbackFunc;
}

void TemperatureSens::count() {
}

void TemperatureSens::reset() {
}

float TemperatureSens::getValue(sens_ret_index_t c) {
        if (!isCalibrate) return temperatureValue;
        return arrTemperatureValue[c];
}

void TemperatureSens::setPins(uint8_t __pin) {
        this->sensorPin = __pin;
}

float TemperatureSens::getSensorAverage(float sensorValue, int numReadings) {
        float sensorSum = 0;
        for (int i = 0; i < numReadings; i++)
                sensorSum += sensorValue;
        return sensorSum / (float)numReadings;
}

float TemperatureSens::lowPassFilter(float input, float output, float alpha) {
        output = (alpha * input) + ((1.0 - alpha) * output);
        return output;
}