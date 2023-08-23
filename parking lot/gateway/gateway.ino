#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

ESP8266WiFiMulti WiFiMulti;
WiFiClient espClient1;

const char* ssid = "ESP-Senzor";
const char* password = "123456789";

const char* ssid_mqtt = "REPLACE_WITH_YOUR_SSID";
const char* password_mqtt = "REPLACE_WITH_YOUR_PASSWORD";

const char* mqtt_server = "192.168.0.99";

//Your IP address or domain name with URL path
const char* serverName = "http://192.168.4.1/device";
unsigned long previousMillis = 0;
const long interval = 5000; 
bool reading = false;

const int modra = 13;
const int cervena = 15;
int num = 0;
PubSubClient client(espClient1);
static char json[128];

void setup_wifi();
void send(int);
void reconnect();

void setup() {
  Serial.begin(115200);
  Serial.println();
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ssid, password);
  while((WiFiMulti.run() == WL_CONNECTED)) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Connected to WiFi");
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  unsigned long currentMillis = millis();
  
  if(currentMillis - previousMillis >= interval) {
    WiFiMulti.addAP(ssid, password);
  while((WiFiMulti.run() == WL_CONNECTED)) { 
    delay(500);
    Serial.print(".");
  }
     // Check WiFi connection status
    if ((WiFiMulti.run() == WL_CONNECTED)) {
      reading = httpGETRequest(serverName);
      Serial.println(reading);
      previousMillis = currentMillis;
    }
    else {
      Serial.println("WiFi Disconnected");
    }
  }
  if(reading == '1'){
        digitalWrite(modra, HIGH);
        digitalWrite(cervena, LOW);
        num = 1;
      }
  else{
        digitalWrite(cervena, HIGH);
        digitalWrite(modra, LOW);
        num = 0;
      }
  reconnect();
  send(num);
  client.publish("data/parking", json);
}

String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
    
  // Your IP address with path or Domain name with URL path 
  http.begin(client, serverName);
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "--"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}

void send(int pocet){
  StaticJsonDocument<128> doc;

  doc["DeviceID"] = "100";
  doc["Max"] = "6";
  doc["Name"] = "parkovisko";
  doc["Free"] = String(pocet);
  doc["Lon"] = "48.75335";
  doc["Lat"] = "21.9224";

  serializeJson(doc, json);
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");  
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid_mqtt, password_mqtt);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}