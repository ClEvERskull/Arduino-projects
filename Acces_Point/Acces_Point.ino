#include "WiFi.h"
#include "ESPAsyncWebServer.h"
const char* ssid = "ESP32-Access-Point";
const char* password = "123456789";

AsyncWebServer server(80);

String temperature;
String humidity;
String ac;
String lights;


void setup() {
  Serial.begin(115200);
  Serial.println();
  
  // Setting the ESP as an access point
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  server.on("/getTemperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", temperature.c_str());
  });
  server.on("/getHumidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", humidity.c_str());
  });
  server.on("/putTemperature", HTTP_PUT, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", temperature.c_str());
  });
  server.on("/putHumidity", HTTP_PUT, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", humidity.c_str());
  });
  server.on("/getAc", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", ac.c_str());
  });
  server.on("/putAc", HTTP_PUT, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", ac.c_str());
  });
  server.on("/getLights", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", lights.c_str());
  });
  server.on("/putLights", HTTP_PUT, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", lights.c_str());
  });

  server.begin();
}

void loop() {
  Serial.println(temperature);
  Serial.println(humidity);
}
