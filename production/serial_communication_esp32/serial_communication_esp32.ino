#define RXD2 16
#define TXD2 17

// esp32 as a sender
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
}

void loop() {

  delay(1000);

  Serial.print("0");
  Serial2.print("0");

  delay(1000);

  Serial.print("1");
  Serial2.print("1");
}



// esp32 as a receiver
// void setup() {
//   // put your setup code here, to run once:

//   // Note the format for setting a serial port is as follows:
//   // Serial2.begin(baud-rate, protocol, RX pin, TX pin)
//   Serial.begin(115200);
//   Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
// }

// void loop() {
//   // put your main code here, to run repeatedly:
//   Serial.println(Serial2.readString());
//   delay(200);

// }