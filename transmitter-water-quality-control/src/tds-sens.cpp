/*
 *  tds-sens.cpp
 *
 *  tds sensor c
 *  Created on: 2023. 4. 3
 */

#include "tds-sens.h"

#include "Arduino.h"
#include "SimpleKalmanFilter.h"

#define SENSOR_FILTER_KF 8

TDS::TDS()
    : sensorPin(A0) {
        isCalibrate = false;
        templateValue = (isCalibrate) ? new float[SENS_RET_TOTAL_DATA] : new float;
        if (isCalibrate) cal_tm = new uint32_t;
}

TDS::TDS(uint8_t __pin, bool enableCalibrate) {
        this->sensorPin = __pin;
        isCalibrate = enableCalibrate;
        templateValue = (isCalibrate) ? new float[SENS_RET_TOTAL_DATA] : new float;
        if (isCalibrate) cal_tm = new uint32_t;
}

TDS::~TDS() {
        delete this;
}

void TDS::init() {
        pinMode(sensorPin, INPUT);
}

void TDS::update() {
        if (millis() - sample_tm >= 40) {
                buff[buffIndex] = analogRead(sensorPin);
                buffIndex++;
                if (buffIndex == 30) buffIndex = 0;
                sample_tm = millis();
        }
        if (millis() - update_tm >= 800) {
                if (!isCalibrate) {
                        for (int i = 0; i < 30; i++) {
                                temp[i] = buff[i];
                        }
                        averageVolt = getMedianNum(temp, 30) * (float)3.3 / 4096.0;
                        float coeff = 1.0 + 0.02 * (temperature - 25.0);
                        float volt = averageVolt / coeff;
                        *templateValue = (133.42 * volt * volt * volt - 255.86 * volt * volt + 857.39 * volt) * 0.5;

                        *templateValue = *templateValue * (5.0 / 1023.0);
                        *templateValue = *templateValue + (*templateValue * SENSOR_FILTER_KF);
                        *templateValue /= SENSOR_FILTER_KF + 1;
                } else {
                        SimpleKalmanFilter* sonarKf = new SimpleKalmanFilter(2, 2, 0.01);
                        templateValue[SENS_RET_RAW_DATA] = analogRead(sensorPin);
                        templateValue[SENS_RET_ACT_DATA] = templateValue[SENS_RET_RAW_DATA] + (templateValue[SENS_RET_RAW_DATA] * SENSOR_FILTER_KF);
                        templateValue[SENS_RET_ACT_DATA] /= SENSOR_FILTER_KF + 1;

                        templateValue[SENS_RET_AVG_DATA] = getSensorAverage(templateValue[SENS_RET_ACT_DATA]);

                        templateValue[SENS_RET_FILTERED_DATA] = sonarKf->updateEstimate(templateValue[SENS_RET_ACT_DATA]);
                        delete sonarKf;
                }

                update_tm = millis();
        }
}

void TDS::debug() {
        if (millis() - debug_tm >= 500) {
                if (isCalibrate) return;
                Serial.print("| templateValueRaw: ");
                Serial.print(*templateValue);
                Serial.println();
                debug_tm = millis();
        }
}

void TDS::calibrate() {
        if (millis() - *cal_tm >= 500) {
                if (!isCalibrate) return;
                Serial.print("| templateValueRaw: ");
                Serial.print(templateValue[SENS_RET_RAW_DATA]);
                Serial.print("| templateValueAct: ");
                Serial.print(templateValue[SENS_RET_ACT_DATA]);
                Serial.print("| templateValueAvg: ");
                Serial.print(templateValue[SENS_RET_AVG_DATA]);
                Serial.print("| templateValueFiltered: ");
                Serial.print(templateValue[SENS_RET_FILTERED_DATA]);
                Serial.println();
                *cal_tm = millis();
        }
}

void TDS::getSensorValue(float* value) {
        *value = *templateValue;
}

void TDS::getSensorValue(int* value) {
}

void TDS::count() {
}

void TDS::reset() {
}

float TDS::getValue(sens_ret_index_t c) {
        if (!isCalibrate) return *templateValue;
        return templateValue[c];
}

void TDS::setPins(uint8_t __pin) {
        this->sensorPin = __pin;
}

float TDS::getSensorAverage(float sensorValue, int numReadings) {
        float sensorSum = 0;
        for (int i = 0; i < numReadings; i++) {
                sensorSum += sensorValue;
        }
        return sensorSum / (float)numReadings;
}

float TDS::lowPassFilter(float input, float output, float alpha) {
        output = (alpha * input) + ((1.0 - alpha) * output);
        return output;
}

int TDS::getMedianNum(int bArray[], int iFilterLen) {
        int bTab[iFilterLen];
        for (byte i = 0; i < iFilterLen; i++)
                bTab[i] = bArray[i];
        int i, j, bTemp;
        for (j = 0; j < iFilterLen - 1; j++) {
                for (i = 0; i < iFilterLen - j - 1; i++) {
                        if (bTab[i] > bTab[i + 1]) {
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