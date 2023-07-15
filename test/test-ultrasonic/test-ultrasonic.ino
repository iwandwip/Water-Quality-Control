#include "NewPing.h"

#define ULTRASONIC_TRIGG_PIN 11
#define ULTRASONIC_ECHO_PIN 12

NewPing sonar(ULTRASONIC_TRIGG_PIN, ULTRASONIC_ECHO_PIN, 200);
float ultrasonicValue;

void setup() {
        Serial.begin(9600);
}

void loop() {
        ultrasonicValue = sonar.ping_cm();
        Serial.print("Ultrasonic: ");
        Serial.println(ultrasonicValue);
        delay(50);
}
