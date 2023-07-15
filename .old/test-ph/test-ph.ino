#include "ph-sens.h"

pHSens pH(A2);

void setup() {
        Serial.begin(9600);
        pH.init();
}

void loop() {
        pH.update();
        pH.debug();
}
