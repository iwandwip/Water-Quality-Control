#include "FireTimer.h"
#include "NewPing.h"
#include "OneWire.h"
#include "DallasTemperature.h"
#include "SoftwareSerial.h"
#include "SimpleKalmanFilter.h"

#define ULTRASONIC_TRIGG_PIN 11
#define ULTRASONIC_ECHO_PIN 12
#define TURBIDITY_SENSOR_PIN A3
#define TEMPERATURE_SENSOR_PIN 8
#define PH_SENSOR_PIN A2
#define TDS_SENSOR_PIN A1

#define PENGISIAN_OUTPUT_PIN 6
#define PENGURASAN_OUTPUT_PIN 9
#define HEATER_OUTPUT_PIN 10
#define PHDOWN_OUTPUT_PIN 5
#define PHUP_OUTPUT_PIN 4

FireTimer ultrasonicTimer, turbidityTimer, temperatureTimer;
FireTimer pHTimer, tdsTimer, serialTimer, debugTimer;

NewPing sonar(ULTRASONIC_TRIGG_PIN, ULTRASONIC_ECHO_PIN, 200);
OneWire oneWire(TEMPERATURE_SENSOR_PIN);
DallasTemperature temperature(&oneWire);

SoftwareSerial ttgoSerial(3, 2);
SimpleKalmanFilter kf(2, 2, 0.01);

float ultrasonicValue, turbidityValue, temperatureValue;
float pHValue, tdsValue;

float statePengisian, statePengurasan, stateHeater;
float statePhDown, statePhUp;

float calibration_value;
unsigned long int avgval;
int buffer_arr[10], temp;

const int VREF = 5.0;
const int SCOUNT = 30;
int analogBuffer[SCOUNT];
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0, copyIndex = 0;
float averageVoltage = 0;

void setup() {
        Serial.begin(9600);
        ttgoSerial.begin(9600);

        pinMode(TURBIDITY_SENSOR_PIN, INPUT);
        temperature.begin();
        pinMode(PH_SENSOR_PIN, INPUT);
        pinMode(TDS_SENSOR_PIN, INPUT);

        pinMode(PENGISIAN_OUTPUT_PIN, OUTPUT);
        pinMode(PENGURASAN_OUTPUT_PIN, OUTPUT);
        pinMode(HEATER_OUTPUT_PIN, OUTPUT);
        pinMode(PHDOWN_OUTPUT_PIN, OUTPUT);
        pinMode(PHUP_OUTPUT_PIN, OUTPUT);

        digitalWrite(PENGISIAN_OUTPUT_PIN, HIGH);
        digitalWrite(PENGURASAN_OUTPUT_PIN, !HIGH);
        digitalWrite(HEATER_OUTPUT_PIN, !HIGH);
        digitalWrite(PHDOWN_OUTPUT_PIN, HIGH);
        digitalWrite(PHUP_OUTPUT_PIN, HIGH);

        ultrasonicTimer.begin(50);
        turbidityTimer.begin(500);
        temperatureTimer.begin(3000);
        pHTimer.begin(30);
        tdsTimer.begin(500);
        serialTimer.begin(1000);
        debugTimer.begin(500);
}

void loop() {
        if (ultrasonicTimer.fire()) {
                ultrasonicValue = sonar.ping_cm();

                ultrasonicValue = (ultrasonicValue > 200) ? 200 : ultrasonicValue;
                ultrasonicValue = (ultrasonicValue < 0) ? 0 : ultrasonicValue;
        }
        if (turbidityTimer.fire()) {
                turbidityValue = analogRead(TURBIDITY_SENSOR_PIN);

                // turbidityValue = turbidityValue * (5.0 / 1024.0);
                // turbidityValue = (-1120.4 * (turbidityValue * turbidityValue)) + (5742.3 * turbidityValue) - 4352.9;

                turbidityValue = map(turbidityValue, 0, 1023, 300, 0);
                turbidityValue -= 30;
                // turbidityValue = regressTurbidity(turbidityValue);
                // turbidityValue = kf.updateEstimate(turbidityValue);

                turbidityValue = (turbidityValue > 300) ? 300 : turbidityValue;
                turbidityValue = (turbidityValue < 0) ? 0 : turbidityValue;

                // turbidityValue = 180 + random(-7, 7);
        }
        if (temperatureTimer.fire()) {
                temperature.requestTemperatures();
                temperatureValue = temperature.getTempCByIndex(0);

                temperatureValue = (temperatureValue > 40) ? 40 : temperatureValue;
                temperatureValue = (temperatureValue < 0) ? 0 : temperatureValue;
        }

        if (pHTimer.fire()) {
                for (int i = 0; i < 10; i++) {
                        buffer_arr[i] = analogRead(PH_SENSOR_PIN);
                        delay(30);
                }
                for (int i = 0; i < 9; i++) {
                        for (int j = i + 1; j < 10; j++) {
                                if (buffer_arr[i] > buffer_arr[j]) {
                                        temp = buffer_arr[i];
                                        buffer_arr[i] = buffer_arr[j];
                                        buffer_arr[j] = temp;
                                }
                        }
                }
                avgval = 0;
                for (int i = 2; i < 8; i++) {
                        avgval += buffer_arr[i];
                }
                float volt = ((float)avgval * 5.0 / 1024.0 / 6.0) - 0.2;
                pHValue = -5.70 * volt + 27.0 - 1.5;
                // pHValue = volt * 3.5;

                pHValue = regressPh(pHValue);
                pHValue = (pHValue > 14) ? 14 : pHValue;
                pHValue = (pHValue < 0) ? 0 : pHValue;
        }
        static unsigned long analogSampleTimepoint = millis();
        if (millis() - analogSampleTimepoint > 40U) {
                analogSampleTimepoint = millis();
                analogBuffer[analogBufferIndex] = analogRead(TDS_SENSOR_PIN);
                analogBufferIndex++;
                if (analogBufferIndex == SCOUNT) {
                        analogBufferIndex = 0;
                }
        }
        if (tdsTimer.fire()) {
                for (copyIndex = 0; copyIndex < SCOUNT; copyIndex++) {
                        analogBufferTemp[copyIndex] = analogBuffer[copyIndex];
                }
                averageVoltage = getMedianNum(analogBufferTemp, SCOUNT) * (float)VREF / 1024.0;
                float compensationCoefficient = 1.0 + 0.02 * (25.0 - 25.0);
                float compensationVolatge = averageVoltage / compensationCoefficient;
                tdsValue = (133.42 * compensationVolatge * compensationVolatge * compensationVolatge - 255.86 * compensationVolatge * compensationVolatge + 857.39 * compensationVolatge) * 0.5;

                tdsValue = regressTds(tdsValue);
                tdsValue = (tdsValue > 1500) ? 1500 : tdsValue;
                tdsValue = (tdsValue < 0) ? 0 : tdsValue;
        }

        if (serialTimer.fire()) {
                ttgoSerial.print(ultrasonicValue);
                ttgoSerial.print(" ");
                ttgoSerial.print(turbidityValue);
                ttgoSerial.print(" ");
                ttgoSerial.print(temperatureValue);
                ttgoSerial.print(" ");
                ttgoSerial.print(pHValue);
                ttgoSerial.print(" ");
                ttgoSerial.print(tdsValue);
                ttgoSerial.print(" ");
                ttgoSerial.println();
        }

        debug();

        if (ttgoSerial.available()) {
                char rxBuffer[250];
                uint8_t rxBufferPtr = 0;
                rxBuffer[rxBufferPtr++] = ttgoSerial.read();
                while (1) {
                        if (ttgoSerial.available()) {
                                rxBuffer[rxBufferPtr++] = ttgoSerial.read();
                                if (rxBuffer[rxBufferPtr - 1] == '\n') break;
                        }
                }
                rxBuffer[rxBufferPtr] = 0;
                String data = String(rxBuffer);

                statePengisian = parseString(data, " ", 0).toFloat();
                statePengurasan = parseString(data, " ", 1).toFloat();
                stateHeater = parseString(data, " ", 2).toFloat();
                statePhDown = parseString(data, " ", 3).toFloat();
                statePhUp = parseString(data, " ", 4).toFloat();

                statePengisian = (statePengisian > 0.5) ? 1 : 0;
                statePengurasan = (statePengurasan > 0.5) ? 1 : 0;
                stateHeater = (stateHeater > 0.5) ? 1 : 0;
                statePhDown = (statePhDown > 0.5) ? 1 : 0;
                statePhUp = (statePhUp > 0.5) ? 1 : 0;
        }

        // digitalWrite(PENGISIAN_OUTPUT_PIN, !statePengisian);
        // digitalWrite(PENGURASAN_OUTPUT_PIN, statePengurasan);
        // digitalWrite(HEATER_OUTPUT_PIN, stateHeater);
        // digitalWrite(PHDOWN_OUTPUT_PIN, !statePhDown);
        // digitalWrite(PHUP_OUTPUT_PIN, !statePhUp);

        // digitalWrite(PENGISIAN_OUTPUT_PIN, HIGH);
        // digitalWrite(PENGURASAN_OUTPUT_PIN, HIGH);
        // digitalWrite(HEATER_OUTPUT_PIN, HIGH);
        // digitalWrite(PHDOWN_OUTPUT_PIN, !HIGH);
        // digitalWrite(PHUP_OUTPUT_PIN, !HIGH);
}

void debug() {
        if (debugTimer.fire()) {
                Serial.print("| ult: ");
                Serial.print(ultrasonicValue);
                Serial.print("| turb: ");
                Serial.print(turbidityValue);
                Serial.print("| temp: ");
                Serial.print(temperatureValue);
                Serial.print("| pH: ");
                Serial.print(pHValue);
                Serial.print("| tds: ");
                Serial.print(tdsValue);

                Serial.print("|| isi: ");
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
}

int getMedianNum(int bArray[], int iFilterLen) {
        int bTab[iFilterLen];
        for (byte i = 0; i < iFilterLen; i++)
                bTab[i] = bArray[i];
        int i, j, bTemp;
        for (j = 0; j < iFilterLen - 1; j++) {
                for (i = 0; i < iFilterLen - j - 1; i++) {
                        if (bTab[i] > bTab[i + 1]) {
                                bTemp = bTab[i];
                                bTab[i] = bTab[i + 1];
                                bTab[i + 1] = bTemp;
                        }
                }
        }
        if ((iFilterLen & 1) > 0)
                bTemp = bTab[(iFilterLen - 1) / 2];
        else
                bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
        return bTemp;
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

double regressTemperature(double x) {
        double terms[] = {
                -4.5197482926191643e+000,
                1.8374383667945264e+000,
                -3.9735727059549732e-002,
                5.3684760244197382e-004
        };

        double t = 1;
        double r = 0;
        for (double c : terms) {
                r += c * t;
                t *= x;
        }
        return r;
}

double regressTds(double x) {
        double terms[] = {
                -3.0460006406495404e+002,
                2.8740747459556530e+000,
                -3.2812579818402732e-003,
                1.4502901747211550e-006
        };

        double t = 1;
        double r = 0;
        for (double c : terms) {
                r += c * t;
                t *= x;
        }
        return r;
}

double regressPh(double x) {
        double terms[] = {
                1.1170070597956316e-001,
                1.3094174512877332e+000,
                -1.1500452858032849e-001,
                4.7679635411859815e-003
        };

        double t = 1;
        double r = 0;
        for (double c : terms) {
                r += c * t;
                t *= x;
        }
        return r;
}

double regressTurbidity(double x) {
        double terms[] = {
                -6.3599127569219219e+006,
                1.3308308604657976e+005,
                -1.0439571519823733e+003,
                3.6385271047145480e+000,
                -4.7540210129208144e-003
        };

        double t = 1;
        double r = 0;
        for (double c : terms) {
                r += c * t;
                t *= x;
        }
        return r;
}