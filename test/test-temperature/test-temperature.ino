#include "OneWire.h"
#include "DallasTemperature.h"

#define TEMPERATURE_SENSOR_PIN 8

OneWire oneWire(TEMPERATURE_SENSOR_PIN);
DallasTemperature temperature(&oneWire);

float temperatureValue;

void setup() {
        Serial.begin(9600);
        temperature.begin();
}

void loop() {
        temperature.requestTemperatures();
        temperatureValue = temperature.getTempCByIndex(0);

        Serial.print("Suhu: ");
        Serial.println(temperatureValue);
        delay(3000);
}
