#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include "Arduino.h"
#include "LoRa.h"
#include "SPI.h"
#include "Wire.h"
#include "ds18b20.h"
#include "input-module.h"
#include "output-module.h"
#include "ph-sens.h"
#include "sensor-module.h"
#include "serial-com.h"
#include "sonar.h"
#include "tds-sens.h"
#include "turbidity-sens.h"

void sensorRoutine(void);
void sensorDebug(void);

SensorModule sensor;
float value[5];

Adafruit_SSD1306 display(128, 64, &Wire, 16);
DigitalOut relaySatu(12);
DigitalOut relayDua(13);
DigitalOut relayTiga(32);
DigitalOut relayEmpat(33);
DigitalOut led(2);
DigitalOut buzzer(21);

SerialCom com;

void setup() {
        Serial.begin(115200);
        sensor.addModule(new Sonar(22, 23));
        sensor.addModule(new DS18b20(36));
        sensor.addModule(new PHsens(37));
        sensor.addModule(new Turbidity(38));
        sensor.addModule(new TDS(39));
        sensor.init();

        SPI.begin(5, 19, 27, 18);
        LoRa.setPins(18, 14, 26);
        if (!LoRa.begin(866E6))
                while (1) Serial.println("Failed");

        pinMode(16, OUTPUT);
        digitalWrite(16, LOW);
        delay(20);
        digitalWrite(16, HIGH);
        Wire.begin(4, 15);
        if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false)) {
                for (;;) Serial.println(F("SSD1306 allocation failed"));
        }

        display.clearDisplay();
        display.setTextColor(WHITE);
        display.setTextSize(1);
        display.setCursor(0, 0);
        display.print("INITIALIZE ");
        display.display();
}

void loop() {
        sensor.update(sensorRoutine);

        display.clearDisplay();
        display.setCursor(20, 0);
        display.println("SENSORS MONITOR");
        display.setTextSize(1);
        display.setCursor(0, 10);
        display.print("SONAR : ");
        display.println(value[0]);
        display.setCursor(0, 20);
        display.print("TEMP  : ");
        display.println(value[1]);
        display.setCursor(0, 30);
        display.print("PH    : ");
        display.println(value[2]);
        display.setCursor(0, 40);
        display.print("TURB  : ");
        display.println(value[3]);
        display.setCursor(0, 50);
        display.print("TDS   : ");
        display.println(value[4]);
        display.display();

        com.clearData();
        for (uint8_t i = 0; i < 5; i++) {
                com.addData(value[i]);
        }
        static uint32_t loraTimer;
        if (millis() - loraTimer >= 1000) {
                led.on();
                LoRa.beginPacket();
                LoRa.print(com.getAllData());
                LoRa.endPacket();
                loraTimer = millis();
                led.off();
        }
}

void sensorRoutine() {
        for (uint8_t i = 0; i < 5; i++) {
                sensor.getModule(i)->getSensorValue(&value[i]);
        }
}

void sensorDebug() {
        Serial.print("| sonar: ");
        Serial.print(value[0]);
        Serial.print("| temp: ");
        Serial.print(value[1]);
        Serial.print("| ph: ");
        Serial.print(value[2]);
        Serial.print("| turb: ");
        Serial.print(value[3]);
        Serial.print("| tds: ");
        Serial.print(value[4]);
        Serial.println();
}
