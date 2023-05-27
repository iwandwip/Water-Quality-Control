/*
 *  ds18b20.cpp
 *
 *  ds18b20 sensor c
 *  Created on: 2023. 4. 3
 */

#include "ds18b20.h"

#include "Arduino.h"

#define SENS_FILTER_KF 8

#define WINDOW_SIZE 5
#define NO_OFFSET 0

float DS18b20::movingAverageFilter(float newValue) {
        static float window[WINDOW_SIZE];
        static int index = 0;
        static float sum = 0;

        sum -= window[index];
        sum += newValue;
        window[index] = newValue;
        index = (index + 1) % WINDOW_SIZE;

        return sum / WINDOW_SIZE;
}

float DS18b20::gaussianFilter(float sensorValue) {
        const float filterWeight[7] = {0.06136, 0.24477, 0.38774, 0.24477, 0.06136};
        const int filterLength = 5;

        static float sensorBuffer[5];
        static int index = 0;

        float filteredValue = 0.0;
        sensorBuffer[index] = sensorValue;
        index++;

        if (index >= filterLength) index = 0;
        for (int i = 0; i < filterLength; i++) {
                int bufferIndex = (index + i) % filterLength;
                filteredValue += filterWeight[i] * sensorBuffer[bufferIndex];
        }
        return filteredValue;
}

DS18b20::DS18b20(uint8_t __pin, bool enableCalibrate)
    : threshold(25.0) {
        ds18b20Pin = __pin;
        isCalibrate = enableCalibrate;
        temperature = (enableCalibrate) ? new float[SENS_RET_TOTAL_DATA] : new float;
        offsetValue = 0.0;
}

DS18b20::~DS18b20() {
        delete this;
}

void DS18b20::init() {
        oneWirePtr = new OneWire(ds18b20Pin);
        sensorsPtr = new DallasTemperature(oneWirePtr);
        sensorsPtr->begin();
}

void DS18b20::update() {
        if (millis() - up_tmr >= 3000) {
                sensorsPtr->requestTemperatures();
                if (!isCalibrate) {
                        float temp = sensorsPtr->getTempCByIndex(0);
                        if (temp > 0) *temperature = temp;
                        // *temperature = (*temperature * 9.0) / 5.0 + 32.0;

                        *temperature = *temperature + (*temperature * SENS_FILTER_KF);
                        *temperature /= SENS_FILTER_KF + 1;
                        *temperature += offsetValue;
                } else {
                        temperature[SENS_RET_RAW_DATA] = sensorsPtr->getTempCByIndex(0);
                        temperature[SENS_RET_RAW_DATA] = (temperature[SENS_RET_RAW_DATA] * 9.0) / 5.0 + 32.0;

                        temperature[SENS_RET_ACT_DATA] = temperature[SENS_RET_RAW_DATA] + (temperature[SENS_RET_RAW_DATA] * SENS_FILTER_KF);
                        temperature[SENS_RET_ACT_DATA] /= SENS_FILTER_KF + 1;

                        temperature[SENS_RET_AVG_DATA] = movingAverageFilter(temperature[SENS_RET_ACT_DATA]);
                        temperature[SENS_RET_FILTERED_DATA] = gaussianFilter(temperature[SENS_RET_ACT_DATA]);

                        inRange = (temperature[SENS_RET_ACT_DATA] < threshold) ? 1 : 0;
                }

                up_tmr = millis();
        }
}

void DS18b20::debug() {
        if (millis() - debug_tm >= 500) {
                Serial.print("| tempCRaw: ");
                Serial.print(*temperature);
                Serial.println();
                debug_tm = millis();
        }
}

void DS18b20::calibrate() {
        if (millis() - debug_tm >= 500) {
                Serial.print("| tempCRaw: ");
                Serial.print(temperature[SENS_RET_RAW_DATA]);
                Serial.print("| tempFRaw: ");
                Serial.print(temperature[SENS_RET_RAW_DATA]);
                Serial.print("| tempCAct: ");
                Serial.print(temperature[SENS_RET_ACT_DATA]);
                Serial.print("| tempFact: ");
                Serial.print(temperature[SENS_RET_ACT_DATA]);
                Serial.println();
                debug_tm = millis();
        }
}

void DS18b20::getSensorValue(float* value) {
        *value = *temperature;
}

void DS18b20::getSensorValue(int* value) {
}

void DS18b20::count() {
}

void DS18b20::reset() {
}

void DS18b20::setPins(uint8_t __pin) {
        this->ds18b20Pin = __pin;
}

void DS18b20::setThreshold(float __temp) {
        threshold = __temp;
}

bool DS18b20::isInRange() {
        return inRange;
}