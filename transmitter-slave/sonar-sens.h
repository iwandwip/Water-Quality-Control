/*
 *  sonar-sens.h
 *
 *  sonar sensor lib
 *  Created on: 2023. 4. 3
 */

#pragma once

#ifndef SONAR_SENS_H
#define SONAR_SENS_H

#include "Arduino.h"
#include "sensor-module.h"
#include "stdint.h"
#include "NewPing.h"

class SonarSens : public BaseSens {
      private:
        NewPing* sensorClass;
        float arrSonarValue[6];
        float sonarValue;
        uint32_t sensTimer[3];

        bool isCalibrate;
        uint8_t echoPin;
        uint8_t triggerPin;

        void (*thisCallbackFunc)(void) = nullptr;
        float getSensorAverage(float sensorValue, int numReadings = 10);
        float lowPassFilter(float input, float output, float alpha);
      public:
        SonarSens();
        SonarSens(uint8_t _echoPin, uint8_t _triggerPin, bool enableCalibrate = false);
        ~SonarSens();

        void init() override;
        void update() override;
        void debug() override;
        void calibrate() override;

        void getValue(float* output) override;
        void getValue(int* output) override;
        void getValue(char* output) override;

        void setCallBack(void (*callbackFunc)(void)) override;
        void count() override;
        void reset() override;

        float getValue(sens_ret_index_t c = SENS_RET_ACT_DATA);
        void setPins(uint8_t _echoPin, uint8_t _triggerPin);
};

#endif  // SONAR_SENS_H