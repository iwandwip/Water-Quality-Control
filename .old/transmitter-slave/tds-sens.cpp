/*
 *  tds-sens.cpp
 *
 *  tds sensor c
 *  Created on: 2023. 4. 3
 */

#include "tds-sens.h"
#include "Arduino.h"
#include "sensor-filter.h"

#define SENSOR_FILTER_KF 8

TdsSens::TdsSens()
  : sensorPin(A0), isCalibrate(false) {
}

TdsSens::TdsSens(uint8_t __pin, bool enableCalibrate) {
        this->sensorPin = __pin;
        isCalibrate = enableCalibrate;
}

TdsSens::~TdsSens() {
        delete this;
}

void TdsSens::init() {
        pinMode(sensorPin, INPUT);
}

void TdsSens::update() {
        if (millis() - sensTimer[3] >= 40U) {
                buff[buffIndex] = analogRead(sensorPin);
                buffIndex++;
                if (buffIndex == 30) buffIndex = 0;
                sensTimer[3] = millis();
        }
        if (millis() - sensTimer[0] >= 800) {
                if (!isCalibrate) {
                        for (int i = 0; i < 30; i++) {
                                temp[i] = analogRead(sensorPin);
                        }
                        averageVolt = getMedianNum(temp, 30) * (float)5.0 / 1024.0;
                        float coeff = 1.0 + 0.02 * (temperature - 25.0);
                        float volt = averageVolt / coeff;

                        tdsValue = (133.42 * volt * volt * volt - 255.86 * volt * volt + 857.39 * volt) * 0.5;
                        tdsValue = tdsValue + (tdsValue * SENSOR_FILTER_KF);
                        tdsValue /= SENSOR_FILTER_KF + 1;
                } else {
                        KalmanFilter* sensorKf = new KalmanFilter(2, 2, 0.01);
                        arrTdsValue[SENS_RET_RAW_DATA] = analogRead(sensorPin);
                        arrTdsValue[SENS_RET_ACT_DATA] = arrTdsValue[SENS_RET_RAW_DATA] + (arrTdsValue[SENS_RET_RAW_DATA] * SENSOR_FILTER_KF);
                        arrTdsValue[SENS_RET_ACT_DATA] /= SENSOR_FILTER_KF + 1;

                        arrTdsValue[SENS_RET_AVG_DATA] = getSensorAverage(arrTdsValue[SENS_RET_ACT_DATA]);
                        // arrTdsValue[SENS_RET_FILTERED_DATA] = lowPassFilter(arrTdsValue[SENS_RET_ACT_DATA], arrTdsValue[SENS_RET_FILTERED_DATA], 0.1);
                        arrTdsValue[SENS_RET_FILTERED_DATA] = sensorKf->updateEstimate(arrTdsValue[SENS_RET_ACT_DATA]);
                        delete sensorKf;
                }
                sensTimer[0] = millis();
        }
}

void TdsSens::debug() {
        if (millis() - sensTimer[1] >= 500) {
                if (isCalibrate) return;
                Serial.print("| tdsValueRaw: ");
                Serial.print(tdsValue);
                Serial.println();
                sensTimer[1] = millis();
        }
}

void TdsSens::calibrate() {
        if (millis() - sensTimer[2] >= 500) {
                if (!isCalibrate) return;
                Serial.print("| arrTdsValueRaw: ");
                Serial.print(arrTdsValue[SENS_RET_RAW_DATA]);
                Serial.print("| arrTdsValueAct: ");
                Serial.print(arrTdsValue[SENS_RET_ACT_DATA]);
                Serial.print("| arrTdsValueAvg: ");
                Serial.print(arrTdsValue[SENS_RET_AVG_DATA]);
                Serial.print("| arrTdsValueFiltered: ");
                Serial.print(arrTdsValue[SENS_RET_FILTERED_DATA]);
                Serial.println();
                sensTimer[2] = millis();
        }
}

void TdsSens::getValue(float* output) {
        *output = tdsValue;
}

void TdsSens::getValue(int* output) {
}

void TdsSens::getValue(char* output) {
}

void TdsSens::setCallBack(void (*callbackFunc)(void)) {
        thisCallbackFunc = callbackFunc;
}

void TdsSens::count() {
}

void TdsSens::reset() {
}

float TdsSens::getValue(sens_ret_index_t c) {
        if (!isCalibrate) return tdsValue;
        return arrTdsValue[c];
}

void TdsSens::setPins(uint8_t __pin) {
        this->sensorPin = __pin;
}

float TdsSens::getSensorAverage(float sensorValue, int numReadings) {
        float sensorSum = 0;
        for (int i = 0; i < numReadings; i++)
                sensorSum += sensorValue;
        return sensorSum / (float)numReadings;
}

float TdsSens::lowPassFilter(float input, float output, float alpha) {
        output = (alpha * input) + ((1.0 - alpha) * output);
        return output;
}

int TdsSens::getMedianNum(int bArray[], int iFilterLen) {
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