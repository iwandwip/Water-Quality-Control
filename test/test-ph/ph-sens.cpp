/*
 *  ph-sens.cpp
 *
 *  ph sensor c
 *  Created on: 2023. 4. 3
 */

#include "ph-sens.h"
#include "Arduino.h"
#include "sensor-filter.h"

#define SENSOR_FILTER_KF 8

pHSens::pHSens()
  : sensorPin(A0), isCalibrate(false) {
}

pHSens::pHSens(uint8_t __pin, bool enableCalibrate) {
        this->sensorPin = __pin;
        isCalibrate = enableCalibrate;
}

pHSens::~pHSens() {
        delete this;
}

void pHSens::init() {
        pinMode(sensorPin, INPUT);
}

void pHSens::update() {
        if (millis() - sensTimer[0] >= 500) {
                for (int i = 0; i < 10; i++) buff[i] = analogRead(sensorPin);
                for (int i = 0; i < 9; i++) {
                        for (int j = i + 1; j < 10; j++) {
                                if (buff[i] > buff[j]) {
                                        temp = buff[i];
                                        buff[i] = buff[j];
                                        buff[j] = temp;
                                }
                        }
                }
                average = 0;
                for (int i = 2; i < 8; i++) average += buff[i];
                if (!isCalibrate) {
                        phValue = (float)average * 5.0 / 1024.0 / 6.0;
                        phValue = phValue * 3.5;
                        phValue = phValue + (phValue * SENSOR_FILTER_KF);
                        phValue /= SENSOR_FILTER_KF + 1;
                } else {
                        KalmanFilter* sensorKf = new KalmanFilter(2, 2, 0.01);
                        arrPhValue[SENS_RET_RAW_DATA] = analogRead(sensorPin);
                        arrPhValue[SENS_RET_ACT_DATA] = arrPhValue[SENS_RET_RAW_DATA] + (arrPhValue[SENS_RET_RAW_DATA] * SENSOR_FILTER_KF);
                        arrPhValue[SENS_RET_ACT_DATA] /= SENSOR_FILTER_KF + 1;

                        arrPhValue[SENS_RET_AVG_DATA] = getSensorAverage(arrPhValue[SENS_RET_ACT_DATA]);
                        // arrPhValue[SENS_RET_FILTERED_DATA] = lowPassFilter(arrPhValue[SENS_RET_ACT_DATA], arrPhValue[SENS_RET_FILTERED_DATA], 0.1);
                        arrPhValue[SENS_RET_FILTERED_DATA] = sensorKf->updateEstimate(arrPhValue[SENS_RET_ACT_DATA]);
                        delete sensorKf;
                }
                sensTimer[0] = millis();
        }
}

void pHSens::debug() {
        if (millis() - sensTimer[1] >= 500) {
                if (isCalibrate) return;
                Serial.print("| phValueRaw: ");
                Serial.print(phValue);
                Serial.println();
                sensTimer[1] = millis();
        }
}

void pHSens::calibrate() {
        if (millis() - sensTimer[2] >= 500) {
                if (!isCalibrate) return;
                Serial.print("| arrPhValueRaw: ");
                Serial.print(arrPhValue[SENS_RET_RAW_DATA]);
                Serial.print("| arrPhValueAct: ");
                Serial.print(arrPhValue[SENS_RET_ACT_DATA]);
                Serial.print("| arrPhValueAvg: ");
                Serial.print(arrPhValue[SENS_RET_AVG_DATA]);
                Serial.print("| arrPhValueFiltered: ");
                Serial.print(arrPhValue[SENS_RET_FILTERED_DATA]);
                Serial.println();
                sensTimer[2] = millis();
        }
}

void pHSens::getValue(float* output) {
        *output = phValue;
}

void pHSens::getValue(int* output) {
}

void pHSens::getValue(char* output) {
}

void pHSens::setCallBack(void (*callbackFunc)(void)) {
        thisCallbackFunc = callbackFunc;
}

void pHSens::count() {
}

void pHSens::reset() {
}

float pHSens::getValue(sens_ret_index_t c) {
        if (!isCalibrate) return phValue;
        return arrPhValue[c];
}

void pHSens::setPins(uint8_t __pin) {
        this->sensorPin = __pin;
}

float pHSens::getSensorAverage(float sensorValue, int numReadings) {
        float sensorSum = 0;
        for (int i = 0; i < numReadings; i++)
                sensorSum += sensorValue;
        return sensorSum / (float)numReadings;
}

float pHSens::lowPassFilter(float input, float output, float alpha) {
        output = (alpha * input) + ((1.0 - alpha) * output);
        return output;
}