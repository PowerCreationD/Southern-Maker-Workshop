const byte led_pin = 12;
char receivedChar;
bool newData = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(led_pin, OUTPUT);
  showNewData();
}

void recvOneChar() {
  if (Serial.available() > 0) {
    receivedChar = Serial.read();
    if (!isSpace(receivedChar)) {
      Serial.println(receivedChar); 
      newData = true;
    }
  }
}

void showNewData() {
  if (newData == true) {

    newData = false;

    switch (receivedChar) {
      case '0':  // receiving 0 turn off light
        Serial.println("turn off light!");
        digitalWrite(12, LOW);
        break;
      case '1':  // receiving 1 will turn on light
        Serial.println("turn on light!");
        digitalWrite(12, HIGH);
        break;
    }
  }
}

void loop() {
  showNewData();
  recvOneChar();
  delay(100);

}