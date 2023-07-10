#include "serial-com.h"

SerialCom com;

void setup() {
        Serial.begin(9600);
        com.begin(9600);
}

void loop() {
        com.clearData();
        com.addData("Arduino");
        com.addData(50);
        com.addData(60);
        com.addData(70);
        com.addData(80);        
        com.sendData();
        com.receive(onReceive);
}

void onReceive(String data) {
        Serial.print(data);
}