#include <WiFi.h>
#include <WebServer.h>
#include "webpage_code.h"

// Include Arduino Wire library for I2C
#include <Wire.h>

// Define Slave I2C Address
#define SLAVE_ADDR 9

const char* ssid = "CuGo-V3-AP";
const char* password = "1qaz2wsx";

/* Put IP Address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

WebServer server(80);

void webpage()
{
  server.send(200,"text/html", webpageCode);
}
// esp32 as a sender
void setup() {
  // Initialize I2C communications as Master
  Wire.begin();
  Serial.println("I2C Master Demonstration");
  // put your setup code here, to run once:
  Serial.begin(9600);
 
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  
  server.on("/", webpage);
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
      Wire.beginTransmission(SLAVE_ADDR);
      Wire.write("1");
      Wire.endTransmission();
    } 
    else if ( cmd == "off") {
      Wire.beginTransmission(SLAVE_ADDR);
      Wire.write("0");
      Wire.endTransmission();
    }
            
}