#define TURBIDITY_SENSOR_PIN A3

float turbidityValue;

void setup() {
        Serial.begin(9600);
        pinMode(TURBIDITY_SENSOR_PIN, INPUT);
}

void loop() {
        turbidityValue = analogRead(TURBIDITY_SENSOR_PIN);
        turbidityValue = map(turbidityValue, 0, 1023, 300, 0);

        Serial.print("Turbidity: ");
        Serial.println(turbidityValue);
        delay(1000);
}
