// Include Arduino Wire library for I2C
#include <Wire.h>

// Define Slave I2C Address
#define SLAVE_ADDR 9

const byte led_pin = 12;
char receivedChar;

void setup() {

  // Initialize I2C communications as Slave
  Wire.begin(SLAVE_ADDR);
  
  // Function to run when data received from master
  Wire.onReceive(receiveEvent);

  pinMode(led_pin, OUTPUT);
  
  // Setup Serial Monitor 
  Serial.begin(9600);
  Serial.println("I2C Slave Demonstration");
}

void loop() {

  // Time delay in loop
  delay(50);
}


void receiveEvent() {

  // Read while data received
  while ( Wire.available() > 0) {
    
    byte msg = Wire.read();
    receivedChar = msg;
    
    switch (receivedChar) {
      case '0':
        digitalWrite(led_pin, LOW);
        break;
      case '1': 
        digitalWrite(led_pin, HIGH);
        break;
    }
  }
  Serial.println("Receive event");
}