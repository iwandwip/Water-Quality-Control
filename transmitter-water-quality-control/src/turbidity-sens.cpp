/*
 *  turbidity-sens.cpp
 *
 *  turbidity sensor c
 *  Created on: 2023. 4. 3
 */

#include "turbidity-sens.h"

#include "Arduino.h"
#include "SimpleKalmanFilter.h"

#define SENSOR_FILTER_KF 8

Turbidity::Turbidity()
    : sensorPin(A0) {
        isCalibrate = false;
        turbidityValue = (isCalibrate) ? new float[SENS_RET_TOTAL_DATA] : new float;
        if (isCalibrate) cal_tm = new uint32_t;
}

Turbidity::Turbidity(uint8_t __pin, bool enableCalibrate) {
        this->sensorPin = __pin;
        isCalibrate = enableCalibrate;
        turbidityValue = (isCalibrate) ? new float[SENS_RET_TOTAL_DATA] : new float;
        if (isCalibrate) cal_tm = new uint32_t;
}

Turbidity::~Turbidity() {
        delete this;
}

void Turbidity::init() {
        pinMode(sensorPin, INPUT);
}

void Turbidity::update() {
        if (millis() - update_tm >= 500) {
                if (!isCalibrate) {
                        *turbidityValue = analogRead(sensorPin);
                        *turbidityValue = *turbidityValue * (3.3 / 4096.0);

                        *turbidityValue = *turbidityValue + (*turbidityValue * SENSOR_FILTER_KF);
                        *turbidityValue /= SENSOR_FILTER_KF + 1;
                } else {
                        SimpleKalmanFilter* sonarKf = new SimpleKalmanFilter(2, 2, 0.01);
                        turbidityValue[SENS_RET_RAW_DATA] = analogRead(sensorPin);
                        turbidityValue[SENS_RET_ACT_DATA] = turbidityValue[SENS_RET_RAW_DATA] + (turbidityValue[SENS_RET_RAW_DATA] * SENSOR_FILTER_KF);
                        turbidityValue[SENS_RET_ACT_DATA] /= SENSOR_FILTER_KF + 1;

                        turbidityValue[SENS_RET_AVG_DATA] = getSensorAverage(turbidityValue[SENS_RET_ACT_DATA]);
                        // turbidityValue[SENS_RET_FILTERED_DATA] = lowPassFilter(turbidityValue[SENS_RET_ACT_DATA], turbidityValue[SENS_RET_FILTERED_DATA], 0.1);
                        turbidityValue[SENS_RET_FILTERED_DATA] = sonarKf->updateEstimate(turbidityValue[SENS_RET_ACT_DATA]);
                        delete sonarKf;
                }

                update_tm = millis();
        }
}

void Turbidity::debug() {
        if (millis() - debug_tm >= 500) {
                if (isCalibrate) return;
                Serial.print("| turbidityValueRaw: ");
                Serial.print(*turbidityValue);
                Serial.println();
                debug_tm = millis();
        }
}

void Turbidity::calibrate() {
        if (millis() - *cal_tm >= 500) {
                if (!isCalibrate) return;
                Serial.print("| turbidityValueRaw: ");
                Serial.print(turbidityValue[SENS_RET_RAW_DATA]);
                Serial.print("| turbidityValueAct: ");
                Serial.print(turbidityValue[SENS_RET_ACT_DATA]);
                Serial.print("| turbidityValueAvg: ");
                Serial.print(turbidityValue[SENS_RET_AVG_DATA]);
                Serial.print("| turbidityValueFiltered: ");
                Serial.print(turbidityValue[SENS_RET_FILTERED_DATA]);
                Serial.println();
                *cal_tm = millis();
        }
}

void Turbidity::getSensorValue(float* value) {
        *value = *turbidityValue;
}

void Turbidity::getSensorValue(int* value) {
}

void Turbidity::count() {
}

void Turbidity::reset() {
}

float Turbidity::getValue(sens_ret_index_t c) {
        if (!isCalibrate) return *turbidityValue;
        return turbidityValue[c];
}

void Turbidity::setPins(uint8_t __pin) {
        this->sensorPin = __pin;
}

float Turbidity::getSensorAverage(float sensorValue, int numReadings) {
        float sensorSum = 0;
        for (int i = 0; i < numReadings; i++) {
                sensorSum += sensorValue;
        }
        return sensorSum / (float)numReadings;
}

float Turbidity::lowPassFilter(float input, float output, float alpha) {
        output = (alpha * input) + ((1.0 - alpha) * output);
        return output;
}