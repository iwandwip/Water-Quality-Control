/*
 *  tds-sens.h
 *
 *  tds sensor lib
 *  Created on: 2023. 4. 3
 */

#pragma once

#ifndef TDS_SENS_H
#define TDS_SENS_H

#include "Arduino.h"
#include "sensor-module.h"
#include "stdint.h"

class TdsSens : public BaseSens {
      private:
        float arrTdsValue[6];
        float tdsValue;
        uint32_t sensTimer[4];

        int buff[30], temp[30];
        int buffIndex = 0;
        float averageVolt = 0;
        float temperature = 25;

        bool isCalibrate;
        uint8_t sensorPin;

        void (*thisCallbackFunc)(void) = nullptr;
        float getSensorAverage(float sensorValue, int numReadings = 10);
        float lowPassFilter(float input, float output, float alpha);
        int getMedianNum(int bArray[], int iFilterLen);
      public:
        TdsSens();
        TdsSens(uint8_t __pin, bool enableCalibrate = false);
        ~TdsSens();

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
        void setPins(uint8_t __pin);
};

#endif  // TDS_SENS_H