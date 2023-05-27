#include "Arduino.h"
#include "ds18b20.h"
#include "input-module.h"
#include "output-module.h"
#include "ph-sens.h"
#include "sensor-module.h"
#include "sonar.h"
#include "tds-sens.h"
#include "turbidity-sens.h"

void sensorRoutine();

SensorModule sensor;
float value[5];

void setup() {
        Serial.begin(115200);
        sensor.addModule(new Sonar(2, 3));
        sensor.addModule(new DS18b20(4));
        sensor.addModule(new PHsens(5));
        sensor.addModule(new Turbidity(6));
        sensor.addModule(new TDS(7));
        sensor.init();
}

void loop() {
        sensor.update(sensorRoutine);
}

void sensorRoutine() {
        for (uint8_t i = 0; i < 5; i++) {
                sensor.getModule(i)->getSensorValue(&value[i]);
        }
}
