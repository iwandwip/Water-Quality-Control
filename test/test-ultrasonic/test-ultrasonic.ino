#include "sonar-sens.h"

SonarSens sonar(12, 11);

void setup() {
        Serial.begin(9600);
        sonar.init();
}

void loop() {
        sonar.update();
        sonar.debug();
}
