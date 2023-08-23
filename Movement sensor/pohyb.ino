#include <ArduinoJson.h>
#include <SPI.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClientSecureBearSSL.h>

const char* ssid = "ASUS_6C";
String id = String("5");  
String json;    
String element;

const int senzorTv = 2;
const int senzorVon = 3;
const int ledTv = 12;
const int ledVon = 13;
bool zmena = false;

ESP8266WiFiMulti WiFiMulti;
void setup() {
    pinMode(senzorTv, INPUT);
    pinMode(senzorVon, INPUT);
    pinMode(ledTv, OUTPUT);
    pinMode(ledVon, OUTPUT);
    pinMode(A0, INPUT);
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
}

void loop() {
   senzory();
   zmena = false;

   if ((WiFiMulti.run() == WL_CONNECTED)) {
      StaticJsonDocument<128> doc;
      doc["applianceSerialNumber"] = id;
      doc["elementId"] = element;
      doc["messageType"] = "DATA";
      doc["value"] = "1";
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

void senzory(){
  digitalWrite(ledTv, LOW);
  digitalWrite(ledVon, LOW);
  while(!zmena){
    if(analogRead(A0) < 400){
      if(digitalRead(senzorTv) == HIGH){
        element = String("1");
        zmena = true;
        digitalWrite(ledTv, HIGH);
      }
      else if(digitalRead(senzorVon) == HIGH){
        element = String("2");
        zmena = true;
        digitalWrite(ledVon, HIGH);
      }
    }
  }
}
