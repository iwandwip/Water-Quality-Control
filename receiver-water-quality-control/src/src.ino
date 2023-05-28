#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include "Arduino.h"
#include "LoRa.h"
#include "SPI.h"
#include "Wire.h"
#include "firebase-handler.h"
#include "input-module.h"
#include "output-module.h"
#include "serial-com.h"

void onReceive(String data);

Adafruit_SSD1306 display(128, 64, &Wire, 16);
FirebaseModule firebase;
SerialCom com;

float value[5];

void setup() {
        Serial.begin(115200);
        firebase.connectToWiFi("Polinema Hotspot AJ LT 3 Tengah", "");
        firebase.waitConnection(1000);
        firebase.init();

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
        int packetSize = LoRa.parsePacket();
        if (packetSize) {
                String dataBuffer;
                while (LoRa.available())
                        dataBuffer += (char)LoRa.read();
                onReceive(dataBuffer);
        }
}

void serverHandler(void* pvParameter) {
        firebase.waitConnection(3000);
        for (;;) {
                if (firebase.isConnect()) {
                        firebase.clearData();
                        firebase.addData(value[0], "/sonar-value");
                        firebase.addData(value[1], "/temperature-value");
                        firebase.addData(value[2], "/ph-value");
                        firebase.addData(value[3], "/turbidity-value");
                        firebase.addData(value[4], "/tds-value");
                        firebase.sendData(2000);
                }
                vTaskDelay(20 / portTICK_PERIOD_MS);
        }
}

void onReceive(String data) {
        for (uint8_t i = 0; i < 5; i++) {
                value[i] = com.getData(data, i);
        }
}