/*
 *  turbidity-sens.cpp
 *
 *  turbidity sensor c
 *  Created on: 2023. 4. 3
 */

#include "turbidity-sens.h"
#include "Arduino.h"
#include "sensor-filter.h"

#define SENSOR_FILTER_KF 8

TurbiditySens::TurbiditySens()
  : sensorPin(A0), isCalibrate(false) {
}

TurbiditySens::TurbiditySens(uint8_t __pin, bool enableCalibrate) {
        this->sensorPin = __pin;
        isCalibrate = enableCalibrate;
}

TurbiditySens::~TurbiditySens() {
        delete this;
}

void TurbiditySens::init() {
        pinMode(sensorPin, INPUT);
}

void TurbiditySens::update() {
        if (millis() - sensTimer[0] >= 500) {
                if (!isCalibrate) {
                        turbidityValue = analogRead(sensorPin);
                        // turbidityValue = turbidityValue * (5.0 / 1023.0);
                        turbidityValue = turbidityValue + (turbidityValue * SENSOR_FILTER_KF);
                        turbidityValue /= SENSOR_FILTER_KF + 1;
                } else {
                        KalmanFilter* sensorKf = new KalmanFilter(2, 2, 0.01);
                        arrTurbidityValue[SENS_RET_RAW_DATA] = analogRead(sensorPin);
                        arrTurbidityValue[SENS_RET_ACT_DATA] = arrTurbidityValue[SENS_RET_RAW_DATA] + (arrTurbidityValue[SENS_RET_RAW_DATA] * SENSOR_FILTER_KF);
                        arrTurbidityValue[SENS_RET_ACT_DATA] /= SENSOR_FILTER_KF + 1;

                        arrTurbidityValue[SENS_RET_AVG_DATA] = getSensorAverage(arrTurbidityValue[SENS_RET_ACT_DATA]);
                        // arrTurbidityValue[SENS_RET_FILTERED_DATA] = lowPassFilter(arrTurbidityValue[SENS_RET_ACT_DATA], arrTurbidityValue[SENS_RET_FILTERED_DATA], 0.1);
                        arrTurbidityValue[SENS_RET_FILTERED_DATA] = sensorKf->updateEstimate(arrTurbidityValue[SENS_RET_ACT_DATA]);
                        delete sensorKf;
                }
                sensTimer[0] = millis();
        }
}

void TurbiditySens::debug() {
        if (millis() - sensTimer[1] >= 500) {
                if (isCalibrate) return;
                Serial.print("| turbidityValueRaw: ");
                Serial.print(turbidityValue);
                Serial.println();
                sensTimer[1] = millis();
        }
}

void TurbiditySens::calibrate() {
        if (millis() - sensTimer[2] >= 500) {
                if (!isCalibrate) return;
                Serial.print("| arrTurbidityValueRaw: ");
                Serial.print(arrTurbidityValue[SENS_RET_RAW_DATA]);
                Serial.print("| arrTurbidityValueAct: ");
                Serial.print(arrTurbidityValue[SENS_RET_ACT_DATA]);
                Serial.print("| arrTurbidityValueAvg: ");
                Serial.print(arrTurbidityValue[SENS_RET_AVG_DATA]);
                Serial.print("| arrTurbidityValueFiltered: ");
                Serial.print(arrTurbidityValue[SENS_RET_FILTERED_DATA]);
                Serial.println();
                sensTimer[2] = millis();
        }
}

void TurbiditySens::getValue(float* output) {
        *output = turbidityValue;
}

void TurbiditySens::getValue(int* output) {
}

void TurbiditySens::getValue(char* output) {
}

void TurbiditySens::setCallBack(void (*callbackFunc)(void)) {
        thisCallbackFunc = callbackFunc;
}

void TurbiditySens::count() {
}

void TurbiditySens::reset() {
}

float TurbiditySens::getValue(sens_ret_index_t c) {
        if (!isCalibrate) return turbidityValue;
        return arrTurbidityValue[c];
}

void TurbiditySens::setPins(uint8_t __pin) {
        this->sensorPin = __pin;
}

float TurbiditySens::getSensorAverage(float sensorValue, int numReadings) {
        float sensorSum = 0;
        for (int i = 0; i < numReadings; i++)
                sensorSum += sensorValue;
        return sensorSum / (float)numReadings;
}

float TurbiditySens::lowPassFilter(float input, float output, float alpha) {
        output = (alpha * input) + ((1.0 - alpha) * output);
        return output;
}