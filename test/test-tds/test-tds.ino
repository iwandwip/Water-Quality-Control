#define TDS_SENSOR_PIN A1

float tdsValue;

const int VREF = 5.0;
const int SCOUNT = 30;
int analogBuffer[SCOUNT];
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0, copyIndex = 0;
float averageVoltage = 0;

void setup() {
        Serial.begin(9600);
        pinMode(TDS_SENSOR_PIN, INPUT);
}

void loop() {
        static unsigned long analogSampleTimepoint = millis();
        if (millis() - analogSampleTimepoint > 40U) {
                analogSampleTimepoint = millis();
                analogBuffer[analogBufferIndex] = analogRead(TDS_SENSOR_PIN);
                analogBufferIndex++;
                if (analogBufferIndex == SCOUNT) {
                        analogBufferIndex = 0;
                }
        }

        for (copyIndex = 0; copyIndex < SCOUNT; copyIndex++) {
                analogBufferTemp[copyIndex] = analogBuffer[copyIndex];
        }
        averageVoltage = getMedianNum(analogBufferTemp, SCOUNT) * (float)VREF / 1024.0;
        float compensationCoefficient = 1.0 + 0.02 * (25.0 - 25.0);
        float compensationVolatge = averageVoltage / compensationCoefficient;
        tdsValue = (133.42 * compensationVolatge * compensationVolatge * compensationVolatge - 255.86 * compensationVolatge * compensationVolatge + 857.39 * compensationVolatge) * 0.5;

        Serial.print("TDS: ");
        Serial.println(tdsValue);
        delay(1000);
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
