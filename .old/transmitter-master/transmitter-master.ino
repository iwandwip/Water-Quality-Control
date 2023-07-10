#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include "Arduino.h"
#include "LoRa.h"
#include "SPI.h"
#include "Wire.h"
#include "input-module.h"
#include "output-module.h"
#include "timer-ticks.h"
#include "fis-header.h"
#include "serial-com.h"

float sensorValue[5];

Adafruit_SSD1306 display(128, 64, &Wire, 16);
SerialCom com;
TimerTicks loraTime(1000);
TimerTicks serialTime(1000);

const int fis_gcI = 5;
const int fis_gcO = 5;
const int fis_gcR = 243;

float g_fisInput[fis_gcI];
float g_fisOutput[fis_gcO];

void setup() {
        Serial.begin(9600);

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
        g_fisInput[0] = sensorValue[0];  // Read Input: Ketingian_Air
        g_fisInput[1] = sensorValue[1];  // Read Input: Kekeruhan_Air
        g_fisInput[2] = sensorValue[2];  // Read Input: Suhu_Air
        g_fisInput[3] = sensorValue[3];  // Read Input: pH_Air
        g_fisInput[4] = sensorValue[4];  // Read Input: Zat_Padat_Terlarut_Pada_Air

        g_fisOutput[0] = 0;
        g_fisOutput[1] = 0;
        g_fisOutput[2] = 0;
        g_fisOutput[3] = 0;
        g_fisOutput[4] = 0;

        fis_evaluate();

        if (serialTime.tick()) {
                Serial.print(g_fisOutput[0]);
                Serial.print(" ");
                Serial.print(g_fisOutput[1]);
                Serial.print(" ");
                Serial.print(g_fisOutput[2]);
                Serial.print(" ");
                Serial.print(g_fisOutput[3]);
                Serial.print(" ");
                Serial.print(g_fisOutput[4]);
                Serial.println();
        }
        // com.clearData();
        // com.addData(g_fisOutput[0]);  // Set output value: Pengisian
        // com.addData(g_fisOutput[1]);  // Set output value: Pengurasan
        // com.addData(g_fisOutput[2]);  // Set output value: Heater
        // com.addData(g_fisOutput[3]);  // Set output value: Cairan_Asam
        // com.addData(g_fisOutput[4]);  // Set output value: Cairan_Basa
        // com.sendData();

        com.receive(onReceive);
        displayOled();
        // debug();
}

void onReceive(String data) {
        for (uint8_t i = 0; i < 5; i++) {
                sensorValue[i] = com.getData(data, i);
        }
        if (loraTime.tick()) {
                // LoRa.beginPacket();
                // LoRa.print(com.getAllData());
                // LoRa.endPacket();
        }
}

void displayOled() {
        display.clearDisplay();
        display.setCursor(20, 0);
        display.println("SENSORS MONITOR");
        display.setTextSize(1);
        display.setCursor(0, 10);
        display.print("SONAR : ");
        display.println(sensorValue[0]);
        display.setCursor(0, 20);
        display.print("TEMP  : ");
        display.println(sensorValue[1]);
        display.setCursor(0, 30);
        display.print("PH    : ");
        display.println(sensorValue[2]);
        display.setCursor(0, 40);
        display.print("TURB  : ");
        display.println(sensorValue[3]);
        display.setCursor(0, 50);
        display.print("TDS   : ");
        display.println(sensorValue[4]);
        display.display();
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

        Serial.print("| Pengisian: ");
        Serial.print(g_fisOutput[0]);
        Serial.print("| Pengurasan: ");
        Serial.print(g_fisOutput[1]);
        Serial.print("| Heater: ");
        Serial.print(g_fisOutput[2]);
        Serial.print("| Asam: ");
        Serial.print(g_fisOutput[3]);
        Serial.print("| Basa: ");
        Serial.print(g_fisOutput[4]);
        Serial.println();
}
