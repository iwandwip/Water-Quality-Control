#include "dstemperature-sens.h"

TemperatureSens temperature(8);

void setup() {
        Serial.begin(9600);
        temperature.init();
}

void loop() {
        temperature.update();
        temperature.debug();
}
