#include <ArduinoJson.h>
#include <SPI.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClientSecureBearSSL.h>

const char* ssid = "ASUS_6C";
String id = String("3");  
String json;    
const int led = 9;
const int dvierka = 10;
int opened = -500;
bool nahlas = false;

ESP8266WiFiMulti WiFiMulti;
void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFiMulti.addAP(ssid);

    if ((WiFiMulti.run() == WL_CONNECTED)) {
      StaticJsonDocument<48> doc;
      doc["applianceSerialNumber"] = id;
      serializeJson(doc, json);
      WiFiClientSecure client;
      client.setInsecure();
      client.connect("https://bp-smart-env-api.herokuapp.com/app/iot/register-appliance", 443);
      HTTPClient http;
      http.begin(client, "https://bp-smart-env-api.herokuapp.com/app/iot/register-appliance");
      http.addHeader("Content-Type", "application/json");
      int httpCode = http.POST(json);
      String payload = http.getString(); 
      Serial.println(httpCode); 
      Serial.println(payload); 
      http.end(); 
    }
    else{
      Serial.println("Error while registring.");
    }

    pinMode(led, OUTPUT);
    pinMode(dvierka, INPUT);
}

void loop() {
   while(digitalRead(dvierka) == HIGH){
      digitalWrite(led, HIGH);
      opened += 500;
      delay(500);
   }

   digitalWrite(led, LOW);

   if(opened > 30000){
      nahlas = true
      opened = 0;
   }
   else{
    opened = 0;
   }
   
  
   if ((WiFiMulti.run() == WL_CONNECTED) && (nahlas)) {
      StaticJsonDocument<128> doc;
      doc["applianceSerialNumber"] = id;
      doc["elementId"] = "1";
      doc["messageType"] = "ERROR";
      doc["value"] = "Otvorene";
      serializeJson(doc, json);

      WiFiClientSecure client;
      client.setInsecure(); 
      client.connect("https://bp-smart-env-api.herokuapp.com/app/iot", 443);
      HTTPClient http;  
      http.begin(client, "https://bp-smart-env-api.herokuapp.com/app/iot"); 
      http.addHeader("Content-Type", "application/json");
      int httpCode = http.POST(json);
      String payload = http.getString(); 
      Serial.println(httpCode); 
      Serial.println(payload); 
      http.end(); 
   }
}
