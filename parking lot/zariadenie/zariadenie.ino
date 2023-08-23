#include <ESP8266WiFi.h>
#include "ESPAsyncWebServer.h"

const char* ssid = "ESP-Senzor";
const char* password = "123456789";

const int cervena = 15;
const int zelena = 13;
const int sensor = 5;

AsyncWebServer server(80);

String readSensor() {
  if(digitalRead(sensor == LOW)){
    digitalWrite(cervena, HIGH);
    digitalWrite(zelena, LOW);
    return 0
  }
  else{
    digitalWrite(zelena, HIGH);
    digitalWrite(cervena, LOW);
    return 1
  }
}

void setup(){
  Serial.begin(115200);
  Serial.println();

  Serial.print("Setting AP (Access Point)…");

  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.on("/device", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readTemp().c_str());
  });
  
  server.begin();
}
 
void loop(){
  
}
