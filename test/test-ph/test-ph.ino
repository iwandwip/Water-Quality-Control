#define PH_SENSOR_PIN A2

float calibration_value, pHValue;
unsigned long int avgval;
int buffer_arr[10], temp;

void setup() {
        Serial.begin(9600);
        pinMode(PH_SENSOR_PIN, INPUT);
}

void loop() {
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

        Serial.print("pH: ");
        Serial.println(pHValue);
        delay(1000);
}
