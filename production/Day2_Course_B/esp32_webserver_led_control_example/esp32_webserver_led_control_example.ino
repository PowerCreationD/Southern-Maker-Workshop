#include <WiFi.h>
#include <WebServer.h>
#include "webpage_code.h"
#define RXD2 16
#define TXD2 17
const byte led_pin = 27;

WebServer server(80);

const char* ssid = "CuGo-V3-AP";
const char* password = "1qaz2wsx";

/* Put IP Address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

// esp32 as a sender
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  pinMode(led_pin, OUTPUT);

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  
  server.on("/", []{
    server.send(200,"text/html", webpageCode);
  });

  server.on("/on", []() {
    sendCommand("on");
  });

  server.on("/off", []() {
    sendCommand("off");
  });

  server.begin();
}

void loop() {
  
  server.handleClient();
  delay(100);
}

void sendCommand(const char* cmd) {
  
    if ( cmd == "on") {
      digitalWrite(led_pin, HIGH);
      Serial.println("turn on");
      Serial2.println("1");
    } 
    else if ( cmd == "off") {
      digitalWrite(led_pin, LOW);
      Serial.println("turn off");
      Serial2.println("0");
    }
}