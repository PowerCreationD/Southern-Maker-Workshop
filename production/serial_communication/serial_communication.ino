#include <SoftwareSerial.h>
#define rxPin 0
#define txPin 1

SoftwareSerial mySerial(rxPin, txPin);

char receivedChar;
boolean newData = false;

void setup() {
  mySerial.begin(9600);
  Serial.begin(9600);
  Serial.println("<Arduino is ready>");
  pinMode(12, OUTPUT);
}

void loop() {
  recvOneChar();
  showNewData();
}

void recvOneChar() {
  if (mySerial.available() > 0) {
    receivedChar = mySerial.read();
    Serial.println(receivedChar);
    newData = true;
  }
}

void showNewData() {
  if (newData == true) {

    newData = false;

    switch (receivedChar) {
      case '0':  // receiving 0 will lock
        Serial.println("lock");
        digitalWrite(12, LOW);
        break;
      case '1':  // receiving 0 will unlock
        Serial.println("unlock");
        digitalWrite(12, HIGH);
        break;
    }
  }
}
