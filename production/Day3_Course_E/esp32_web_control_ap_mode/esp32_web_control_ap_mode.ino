#include <WiFi.h>
#include <WebServer.h>
#include "webpage_code.h"
#define RXD2 16
#define TXD2 17

WebServer server(80);

const char* ssid = "CuGo-V3-AP";
const char* password = "1qaz2wsx";

/* Put IP Address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

void send_webpage()
{
  server.send(200,"text/html", webpageCode);
}
// esp32 as a sender
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  
  server.on("/", send_webpage);

  server.on("/forward", []() {
    sendCommand("forward");
  });

  server.on("/backward", []() {
    sendCommand("backward");
  });

  server.on("/mission1", []() {
    sendCommand("mission1");
  });

  server.begin();

  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
}

void loop() {
  
  server.handleClient();
  delay(100);
}

void sendCommand(const char* cmd) {
  
    if ( cmd == "forward") {
      Serial.println("execute forward 1s");
      Serial2.println("f");
    }
    else if ( cmd == "backward") {
      Serial.println("execute backward 1s");
      Serial2.println("b");
    }
    else if ( cmd == "mission1") {
      Serial.println("execute mission1");
      Serial2.println("m");
    }
            
}