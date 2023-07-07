/*
 *  ds18b20.h
 *
 *  ds18b20 sensor lib
 *  Created on: 2023. 4. 3
 */

#pragma once

#ifndef DS18B20_H
#define DS18B20_H

#include <DallasTemperature.h>
#include <OneWire.h>
#include <stdint.h>

#include "Arduino.h"
#include "sensor-module.h"

class DS18b20 : public BaseSens {
       private:
        OneWire* oneWirePtr;
        DallasTemperature* sensorsPtr;

        uint8_t ds18b20Pin;
        float* temperature;
        float threshold;
        bool inRange;
        bool isCalibrate;
        uint32_t up_tmr;
        uint32_t debug_tm;

        float offsetValue;

        float movingAverageFilter(float newValue);
        float gaussianFilter(float sensorValue);

       public:
        DS18b20(uint8_t __pin, bool enableCalibrate = false);
        ~DS18b20();

        void init() override;
        void update() override;
        void debug() override;
        void calibrate() override;
        void getSensorValue(float* value) override;
        void getSensorValue(int* value) override;
        void count() override;
        void reset() override;

        float getTemperature(sens_ret_index_t __d = SENS_RET_ACT_DATA);
        void setPins(uint8_t __pin = 2);
        void setThreshold(float __temp);
        bool isInRange();
};

#endif  // DS18B20_H