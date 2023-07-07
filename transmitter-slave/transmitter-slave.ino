#include "ph-sens.h"
#include "tds-sens.h"
#include "MemoryFree.h"
#include "sonar-sens.h"
#include "serial-com.h"
#include "timer-ticks.h"
#include "input-module.h"
#include "output-module.h"
#include "sensor-module.h"
#include "turbidity-sens.h"
#include "SoftwareSerial.h"
#include "dstemperature-sens.h"

#define NUM_SENSOR 5

SensorModule sensor;
SoftwareSerial SerialEsp(3, 2);
SerialCom com;

float sensorValue[NUM_SENSOR];
float outputValue[5];

TimerTicks sendTime(500);

void setup() {
        Serial.begin(9600);
        SerialEsp.begin(9600);
}

void loop() {
        sensor.update(subRoutine);
        onSendData();
        debug();
}

void subRoutine() {
        sensor.addModule(new SonarSens(4, 5));
        sensor.addModule(new TurbiditySens(A1));
        sensor.addModule(new TemperatureSens(6));
        sensor.addModule(new pHSens(A0));
        sensor.addModule(new TdsSens(A2));

        sensor.init();
        sensor.update();

        for (uint8_t i = 0; i < NUM_SENSOR; i++) {
                sensor.getModule(i)->getValue(&sensorValue[i]);
                sensorValue[i] = (sensorValue[i] < 0) ? 0 : sensorValue[i];
        }

        // sensorValue[0] = 110;
        // sensorValue[1] = 150;
        // sensorValue[2] = 27;
        // sensorValue[3] = 7;
        // sensorValue[4] = 700;

        sensorValue[0] = (sensorValue[0] > 200) ? 200 : sensorValue[0];
        sensorValue[1] = (sensorValue[1] > 300) ? 300 : sensorValue[1];
        sensorValue[2] = (sensorValue[2] > 40) ? 40 : sensorValue[2];
        sensorValue[3] = (sensorValue[3] > 14) ? 14 : sensorValue[3];
        sensorValue[4] = (sensorValue[4] > 1500) ? 1500 : sensorValue[4];

        sensor.clearModules();
}

void onReceive(String data) {
        for (uint8_t i = 0; i < 5; i++) {
                outputValue[i] = com.getData(data, i);
                outputValue[i] = (outputValue[i] > 0.5) ? 1.0 : 0.0;
        }
}

template<typename T>
void sendData(T& serial) {
        for (uint8_t i = 0; i < NUM_SENSOR; i++) {
                serial.print(sensorValue[i]);
                serial.print(";");
        }
        serial.print(freeMemory());
        serial.println();
}

void onSendData() {
        if (sendTime.tick()) {
                // sendData(Serial);
                sendData(SerialEsp);
        }
        if (SerialEsp.available() > 0) {
                String data;
                while (SerialEsp.available()) data += (char)SerialEsp.read();
                onReceive(data);
        }
}

void debug() {
        Serial.print("| sonar: ");
        Serial.print(sensorValue[0]);
        Serial.print("| turb: ");
        Serial.print(sensorValue[1]);
        Serial.print("| temp: ");
        Serial.print(sensorValue[2]);
        Serial.print("| pH: ");
        Serial.print(sensorValue[3]);
        Serial.print("| tds: ");
        Serial.print(sensorValue[4]);
        Serial.print("| mem: ");
        Serial.print(freeMemory());
        Serial.print("| isi: ");
        Serial.print(outputValue[0]);
        Serial.print("| kuras: ");
        Serial.print(outputValue[1]);
        Serial.print("| heater: ");
        Serial.print(outputValue[2]);
        Serial.print("| up: ");
        Serial.print(outputValue[3]);
        Serial.print("| down: ");
        Serial.print(outputValue[4]);
        Serial.println();
}
