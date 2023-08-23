#include <ArduinoJson.h>
#include <SPI.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClientSecureBearSSL.h>
#include <Servo.h> 

ESP8266WiFiMulti WiFiMulti;

const char* ssid = "ASUS_6C";
char* myId = "2";  
String json;

Servo zaclony;
Servo dvere;

int zaclonyPin = 10; 
int dverePin = 11; 
bool zaclonyOtv = false;
bool dvereOtv = false;

char* applianceId;
char* elementId;
char* id;
char* scheduleState;
char* sentAt;
char* triggerAt;
char* value;

int number;

void setup() {
    zaclony.attach(zaclonyPin); 
    dvere.attach(dverePin);

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
      http.begin(client, "https://bp-smart-env-api.herokuapp.com/scheduling/schedule/2"); 
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
          if(zaclonyOtv){
            zaclony.write(0);
            zaclonyOtv = false;
          }
          else{
            zaclony.write(180);
            zaclonyOtv = true;
          }
          break;
        case '2':
          if(dvereOtv){
            dvere.write(0);
            dvereOtv = false;
          }
          else{
            dvere.write(180);
            dvereOtv = true;
          }
          break;
        default:
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

  char zaclonyServo = data["servo1"}.as<char*>();
  char dvereServo = data["servo2"}.as<char*>();
  servoOvladanie(zaclonyServo, dvereServo);
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

void servoOvladanie(char zaclonyServo, char dvereServo){
  if(zaclonyServo == 'o')
    zaclony.write(180); 
   else
    zaclony.write(0); 

   if(dvereServo == 'o')
    dvere.write(90); 
   else
    dvere.write(0); 
}*/
