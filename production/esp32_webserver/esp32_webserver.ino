#include <WiFi.h>
#include <WebServer.h>
#include "webpage_code.h"
//------------------------------------------
WebServer server(80);
const char* ssid = "MobileMover";
const char* password = "19991225";
//------------------------------------------
void webpage()
{
  server.send(200,"text/html", webpageCode);
}
//=================================================================
void setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while(WiFi.status()!=WL_CONNECTED){delay(500);Serial.print(".");}
  Serial.println();
  Serial.print("IP Address: "); Serial.println(WiFi.localIP());

  server.on("/", webpage);
  server.begin();
}
//=================================================================
void loop()
{
  server.handleClient();
}