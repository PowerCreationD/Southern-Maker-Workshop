#include <WiFi.h>
#include <WebServer.h>
#include "webpage_code.h"
#define RXD2 16
#define TXD2 17

WebServer server(80);

const char* ssid = "MobileMover";
const char* password = "19991225";

void webpage()
{
  server.send(200,"text/html", webpageCode);
}
// esp32 as a sender
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(27, OUTPUT);
  WiFi.begin(ssid, password);
  while( WiFi.status() != WL_CONNECTED ){
    digitalWrite(27, LOW);
    delay(500);
    Serial.print(".");
  }
  digitalWrite(27, HIGH);
  Serial.println();
  Serial.print("IP Address: "); 
  Serial.println(WiFi.localIP());
  
  server.on("/", webpage);

  server.on("/on", []() {
    Serial.println("ignite");
    sendCommand("on");
  });

  server.on("/off", []() {
    sendCommand("off");
  });

  server.on("/forward", []() {
    sendCommand("forward");
  });

  server.on("/backward", []() {
    sendCommand("backward");
  });

  server.begin();

  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
}

void loop() {
  
  server.handleClient();
  delay(100);
}

void sendCommand(const char* cmd) {
  
    if ( cmd == "on") {
      Serial.println("on");
      Serial2.println("m");
    } 
    else if ( cmd == "off") {
      Serial.println("off");
      Serial2.println("0");
    }
    else if ( cmd == "forward") {
      Serial.println("forward");
      Serial2.println("f");
    }
    else if ( cmd == "backward") {
      Serial.println("backward");
      Serial2.println("b");
    }
            
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