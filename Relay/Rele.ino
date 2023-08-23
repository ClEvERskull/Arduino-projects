#include <ArduinoJson.h>
#include <SPI.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClientSecureBearSSL.h>
#include <Servo.h> 

ESP8266WiFiMulti WiFiMulti;

const char* ssid = "ASUS_6C";
char* myId = "4";  
String json;

char* applianceId;
char* elementId;
char* id;
char* scheduleState;
char* sentAt;
char* triggerAt;
char* value;
int number;

void setup() {
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFiMulti.addAP(ssid);

    if ((WiFiMulti.run() == WL_CONNECTED)) {
      StaticJsonDocument<48> doc;
      doc["applianceSerialNumber"] = myId;
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
  if ((WiFiMulti.run() == WL_CONNECTED)) {

      WiFiClientSecure client;
      client.setInsecure(); 
      client.connect("https://bp-smart-env-api.herokuapp.com/scheduling/schedule/2", 443);
      HTTPClient http;  
      http.begin(client, "https://bp-smart-env-api.herokuapp.com/scheduling/schedule/4"); 
      http.addHeader("Content-Type", "application/json");
      int httpCode = http.GET();
     
      DynamicJsonDocument doc(2048);
      deserializeJson(doc, http.getStream());
      applianceId = doc["applianceId"];
      elementId = doc["elementId"];
      id = doc["id"];
      scheduleState = doc["scheduleState"];
      sentAt = doc["sentAt"];
      triggerAt = doc["triggerAt"];
      value = doc["value"];
      number = int(value - 48);

      String payload = http.getString(); 
      Serial.println(httpCode); 
      Serial.println(payload);
      http.end(); 
  }

    if(scheduleState == "S"){
      switch (number){
        
        case '1':
          if(digitalRead(2) == LOW){
            digitalWrite(2, HIGH);
          }
          else{
            digitalWrite(2, LOW);
          }
          break;
        case '2':
          if(digitalRead(3) == LOW){
            digitalWrite(3, HIGH);
          }
          else{
            digitalWrite(3, LOW);
          }
          break;

        case '3':
          if(digitalRead(4) == LOW){
            digitalWrite(4, HIGH);
          }
          else{
            digitalWrite(4, LOW);
          }
          break;
        default:
          digitalWrite(2, LOW);
          digitalWrite(3, LOW);
          digitalWrite(4, LOW);
          break;
          
      }
    }
}









/*
void httpGETRequest(const char* server) {
  HTTPClient http;
    
  http.begin(server);
  
  http.addHeader("Content-Type", "application/json");            

   int httpResponseCode = http.GET();   
 
   if(httpResponseCode>0){
 
    String response = http.getString();   
 
    Serial.println(httpResponseCode);
    Serial.println(response);          
 
   }else{
 
    Serial.print("Error on sending POST Request: ");
    Serial.println(httpResponseCode);
 
   }

  DynamicJsonDocument doc(2048);
  deserializeJson(data, http.getStream());

  DeserializationError error = deserializeJson(data, http.getStream());
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
 
   http.end();

  char rele1 = data["relay1"}.as<char*>();
  char rele2 = data["relay2"}.as<char*>();
  char rele3 = data["relay3"}.as<char*>();
  relay(rele1, rele2, rele3);
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
}

void relay(char rele1, char rele2, char rele3){
  if(rele1 == 'y')
    digitalWrite(2, HIGH);
   else
    digitalWrite(2, LOW);

   if(rele2 == 'y')
    digitalWrite(3, HIGH);
   else
    digitalWrite(3, LOW);

   if(rele3 == 'y')
    digitalWrite(4, HIGH);
   else
    digitalWrite(4, LOW);
}*/
