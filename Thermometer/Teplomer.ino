#include <ArduinoJson.h>
#include <SPI.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClientSecureBearSSL.h>
#include <DHT.h>

#define DHTTYPE DHT22
#define DHTPIN  5

ESP8266WiFiMulti WiFiMulti;
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "ASUS_6C";
String id = String("1"); 
String json;    


float temp;
String T;

/*unsigned long previousMillis = 0;
const long interval = 15000; */

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ssid);
  dht.begin();
  
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
  temp = dht.readTemperature();
  T = String(temp,2);
  Serial.println(T);
  
    if ((WiFiMulti.run() == WL_CONNECTED)) {

        StaticJsonDocument<128> doc;
        doc["applianceSerialNumber"] = id;
        doc["elementId"] = "1";
        doc["messageType"] = "DATA";
        doc["value"] = T;
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
   delay(10000);
}
/*
void httpPUTRequest(const char* server, String message) {
  HTTPClient http;
    
  http.begin(server);
  
  http.addHeader("Content-Type", "application/json");            

   int httpResponseCode = http.PUT(message);   
 
   if(httpResponseCode>0){
 
    String response = http.getString();   
 
    Serial.println(httpResponseCode);
    Serial.println(response);          
 
   }else{
 
    Serial.print("Error on sending PUT Request: ");
    Serial.println(httpResponseCode);
 
   }
 
   http.end();
}


void httpRegisterDevice(const char* server, String message) {
  HTTPClient http;
    
  http.begin(server);
  
  http.addHeader("Content-Type", "application/json");            

   int httpResponseCode = http.POST(message);   
 
   if(httpResponseCode>0){
 
    String response = http.getString();   
 
    Serial.println(httpResponseCode);
    Serial.println(response);          
 
   }else{
 
    Serial.print("Error on sending POST Request: ");
    Serial.println(httpResponseCode);
 
   }
 
   http.end();
}*/
