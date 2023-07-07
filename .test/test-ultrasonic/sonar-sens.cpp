/*
 *  sonar-sens.cpp
 *
 *  sonar sensor c
 *  Created on: 2023. 4. 3
 */

#include "sonar-sens.h"
#include "Arduino.h"
#include "sensor-filter.h"

#define SENSOR_FILTER_KF 8

SonarSens::SonarSens()
  : isCalibrate(false) {
}

SonarSens::SonarSens(uint8_t _echoPin, uint8_t _triggerPin, bool enableCalibrate) {
        this->echoPin = _echoPin;
        this->triggerPin = _triggerPin;
        isCalibrate = enableCalibrate;
}

SonarSens::~SonarSens() {
        delete this;
}

void SonarSens::init() {
        sensorClass = new NewPing(triggerPin, echoPin, 200);
}

void SonarSens::update() {
        if (millis() - sensTimer[0] >= 500) {
                if (!isCalibrate) {
                        sonarValue = sensorClass->ping_cm();
                        sonarValue = sonarValue + (sonarValue * SENSOR_FILTER_KF);
                        sonarValue /= SENSOR_FILTER_KF + 1;
                } else {
                        KalmanFilter* sensorKf = new KalmanFilter(2, 2, 0.01);
                        arrSonarValue[SENS_RET_RAW_DATA] = sensorClass->ping_cm();
                        arrSonarValue[SENS_RET_ACT_DATA] = arrSonarValue[SENS_RET_RAW_DATA] + (arrSonarValue[SENS_RET_RAW_DATA] * SENSOR_FILTER_KF);
                        arrSonarValue[SENS_RET_ACT_DATA] /= SENSOR_FILTER_KF + 1;

                        arrSonarValue[SENS_RET_AVG_DATA] = getSensorAverage(arrSonarValue[SENS_RET_ACT_DATA]);
                        // arrSonarValue[SENS_RET_FILTERED_DATA] = lowPassFilter(arrSonarValue[SENS_RET_ACT_DATA], arrSonarValue[SENS_RET_FILTERED_DATA], 0.1);
                        arrSonarValue[SENS_RET_FILTERED_DATA] = sensorKf->updateEstimate(arrSonarValue[SENS_RET_ACT_DATA]);
                        delete sensorKf;
                }
                sensTimer[0] = millis();
        }
        // delete sensorClass;
        // sensorClass = nullptr;
}

void SonarSens::debug() {
        if (millis() - sensTimer[1] >= 500) {
                if (isCalibrate) return;
                Serial.print("| sonarValueRaw: ");
                Serial.print(sonarValue);
                Serial.println();
                sensTimer[1] = millis();
        }
}

void SonarSens::calibrate() {
        if (millis() - sensTimer[2] >= 500) {
                if (!isCalibrate) return;
                Serial.print("| arrSonarValueRaw: ");
                Serial.print(arrSonarValue[SENS_RET_RAW_DATA]);
                Serial.print("| arrSonarValueAct: ");
                Serial.print(arrSonarValue[SENS_RET_ACT_DATA]);
                Serial.print("| arrSonarValueAvg: ");
                Serial.print(arrSonarValue[SENS_RET_AVG_DATA]);
                Serial.print("| arrSonarValueFiltered: ");
                Serial.print(arrSonarValue[SENS_RET_FILTERED_DATA]);
                Serial.println();
                sensTimer[2] = millis();
        }
}

void SonarSens::getValue(float* output) {
        *output = sonarValue;
}

void SonarSens::getValue(int* output) {
}

void SonarSens::getValue(char* output) {
}

void SonarSens::setCallBack(void (*callbackFunc)(void)) {
        thisCallbackFunc = callbackFunc;
}

void SonarSens::count() {
}

void SonarSens::reset() {
}

float SonarSens::getValue(sens_ret_index_t c) {
        if (!isCalibrate) return sonarValue;
        return arrSonarValue[c];
}

void SonarSens::setPins(uint8_t _echoPin, uint8_t _triggerPin) {
        this->echoPin = _echoPin;
        this->triggerPin = _triggerPin;
}

float SonarSens::getSensorAverage(float sensorValue, int numReadings) {
        float sensorSum = 0;
        for (int i = 0; i < numReadings; i++)
                sensorSum += sensorValue;
        return sensorSum / (float)numReadings;
}

float SonarSens::lowPassFilter(float input, float output, float alpha) {
        output = (alpha * input) + ((1.0 - alpha) * output);
        return output;
}