#include "serial-com.h"

SerialCom com;

void setup() {
        Serial.begin(9600);
}

void loop() {
        com.clearData();
        com.addData("TTGO");
        com.addData(10);
        com.addData(20);
        com.addData(30);
        com.addData(40);
        com.sendData();
        com.receive(onReceive);
}

void onReceive(String data) {
        Serial.print(data);
}
