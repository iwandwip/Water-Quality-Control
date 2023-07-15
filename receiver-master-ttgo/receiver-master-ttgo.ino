#include "FireTimer.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include "Wire.h"
#include "LoRa.h"
#include "SPI.h"

#include "WiFi.h"
#include "Firebase_ESP_Client.h"
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define WIFI_SSID "FreshwaterAquaculture"
#define WIFI_PASSWORD "1234567890"

// #define WIFI_SSID "KASTARA GROUP INDONESIA"
// #define WIFI_PASSWORD "KASTARA@2022"

#define API_KEY "AIzaSyAzYdsu_LMrp86Ec3oezJsJdckmh1_FWPM"
#define DATABASE_URL "https://water-quality-control-616f0-default-rtdb.firebaseio.com/"
#define USER_EMAIL "water-quality@gmail.com"
#define USER_PASSWORD "water-quality"

Adafruit_SSD1306 display(128, 64, &Wire, 16);

TaskHandle_t serverTask;

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

FireTimer firebaseTimer, loraTimer, debugTimer;

float ultrasonicValue, turbidityValue, temperatureValue;
float pHValue, tdsValue;

float statePengisian, statePengurasan, stateHeater;
float statePhDown, statePhUp, stateMode;

String cekKoneksi = "X";

void setup() {
        Serial.begin(115200);

        xTaskCreatePinnedToCore(serverHandler, "server_task", 20000, NULL, 1, &serverTask, 1);

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

        loraTimer.begin(1000);
        firebaseTimer.begin(2000);
        debugTimer.begin(1000);

        WiFi.mode(WIFI_OFF);
        delay(1000);
        WiFi.mode(WIFI_STA);
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        Serial.print("Connecting to : ");
        Serial.println(WIFI_SSID);
        while (WiFi.status() != WL_CONNECTED) {
                Serial.print(".");
                delay(1000);
        }
        Serial.print("Connected with IP: ");
        Serial.println(WiFi.localIP());
        Serial.println();

        config.api_key = API_KEY;
        auth.user.email = USER_EMAIL;
        auth.user.password = USER_PASSWORD;
        config.database_url = DATABASE_URL;
        config.token_status_callback = tokenStatusCallback;
        fbdo.setResponseSize(2048);
        Firebase.begin(&config, &auth);
        Firebase.reconnectWiFi(true);
        Firebase.setDoubleDigits(2);
        config.timeout.serverResponse = 10 * 1000;
}

void loop() {
        int packetSize = LoRa.parsePacket();
        if (packetSize) {
                String data;
                while (LoRa.available()) {
                        data += (char)LoRa.read();
                }
                ultrasonicValue = parseString(data, " ", 0).toFloat();
                turbidityValue = parseString(data, " ", 1).toFloat();
                temperatureValue = parseString(data, " ", 2).toFloat();
                pHValue = parseString(data, " ", 3).toFloat();
                tdsValue = parseString(data, " ", 4).toFloat();
        }
        if (loraTimer.fire()) {
                String loraData;
                loraData += String(statePengisian) + " ";
                loraData += String(statePengurasan) + " ";
                loraData += String(stateHeater) + " ";
                loraData += String(statePhDown) + " ";
                loraData += String(statePhUp) + " ";
                loraData += String(stateMode) + " ";

                LoRa.beginPacket();
                LoRa.print(loraData);
                LoRa.endPacket();
        }

        debug();

        display.clearDisplay();
        display.setCursor(5, 0);
        display.print("SENSORS MONITOR | " + String(cekKoneksi));
        display.setTextSize(1);
        display.setCursor(0, 10);
        display.print("SONAR : ");
        display.print(ultrasonicValue);
        display.setCursor(0, 20);
        display.print("TURB  : ");
        display.print(turbidityValue);
        display.setCursor(0, 30);
        display.print("TEMP  : ");
        display.print(temperatureValue);
        display.setCursor(0, 40);
        display.print("PH    : ");
        display.print(pHValue);
        display.setCursor(0, 50);
        display.print("TDS   : ");
        display.print(tdsValue);
        display.display();
}

void serverHandler(void* pvParameter) {
        for (;;) {
                if (firebaseTimer.fire() && Firebase.ready()) {
                        Firebase.RTDB.setFloat(&fbdo, "/sonar-value", int(ultrasonicValue));
                        Firebase.RTDB.setFloat(&fbdo, "/turbidity-value", int(turbidityValue));
                        Firebase.RTDB.setFloat(&fbdo, "/temperature-value", temperatureValue);
                        Firebase.RTDB.setFloat(&fbdo, "/ph-value", pHValue);
                        Firebase.RTDB.setFloat(&fbdo, "/tds-value", int(tdsValue));

                        if (Firebase.RTDB.getString(&fbdo, "/btn-pengisian")) {
                                statePengisian = fbdo.to<String>().toFloat();
                                cekKoneksi = "V";
                        } else {
                                cekKoneksi = "X";
                        }
                        if (Firebase.RTDB.getString(&fbdo, "/btn-pengurasan")) {
                                statePengurasan = fbdo.to<String>().toFloat();
                                cekKoneksi = "V";
                        } else {
                                cekKoneksi = "X";
                        }
                        if (Firebase.RTDB.getString(&fbdo, "/btn-heater")) {
                                stateHeater = fbdo.to<String>().toFloat();
                                cekKoneksi = "V";
                        } else {
                                cekKoneksi = "X";
                        }
                        if (Firebase.RTDB.getString(&fbdo, "/btn-pHdown")) {
                                statePhDown = fbdo.to<String>().toFloat();
                                cekKoneksi = "V";
                        } else {
                                cekKoneksi = "X";
                        }
                        if (Firebase.RTDB.getString(&fbdo, "/btn-pHup")) {
                                statePhUp = fbdo.to<String>().toFloat();
                                cekKoneksi = "V";
                        } else {
                                cekKoneksi = "X";
                        }
                        if (Firebase.RTDB.getString(&fbdo, "/btn-mode")) {
                                stateMode = fbdo.to<String>().toFloat();
                                cekKoneksi = "V";
                        } else {
                                cekKoneksi = "X";
                        }
                }
                vTaskDelay(20 / portTICK_PERIOD_MS);
        }
}

void debug() {
        Serial.print("| sonar: ");
        Serial.print(ultrasonicValue);
        Serial.print("| turb: ");
        Serial.print(turbidityValue);
        Serial.print("| temp: ");
        Serial.print(temperatureValue);
        Serial.print("| pH: ");
        Serial.print(pHValue);
        Serial.print("| tds: ");
        Serial.print(tdsValue);

        Serial.print("| mode: ");
        Serial.print(stateMode);
        Serial.print("| isi: ");
        Serial.print(statePengisian);
        Serial.print("| kuras: ");
        Serial.print(statePengurasan);
        Serial.print("| heat: ");
        Serial.print(stateHeater);
        Serial.print("| phdown: ");
        Serial.print(statePhDown);
        Serial.print("| phup: ");
        Serial.print(statePhUp);
        Serial.println();
}

String parseString(String data, char separator[], int index) {
        int found = 0;
        int strIndex[] = { 0, -1 };
        int maxIndex = data.length() - 1;
        for (int i = 0; i <= maxIndex && found <= index; i++) {
                if (data.charAt(i) == separator[0] || i == maxIndex) {
                        found++;
                        strIndex[0] = strIndex[1] + 1;
                        strIndex[1] = (i == maxIndex) ? i + 1 : i;
                }
        }
        return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}