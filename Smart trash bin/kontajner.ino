#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define echoPin 5
#define trigPin 4 
#define analog A0
#define potentiometer 16
#define smoke 14

const char* ssid = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";

const char* mqtt_server = "192.168.0.99";

WiFiClient espClient;
PubSubClient client(espClient);

long duration; 
int distance; 
int sensorThres = 400; 
static char json[128];

void setup_wifi();
void measurement();
void reconnect();

void setup() {
  pinMode(analog, INPUT);
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  pinMode(potentiometer, OUTPUT);
  pinMode(smoke, OUTPUT);
  Serial.begin(9600); 
  Serial.println("Ultrasonic Sensor HC-SR04 Test"); 
  Serial.println("with Arduino UNO R3");
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  if(!client.loop())
    client.connect("Kontajner");
  measurement();
  client.publish("data/dumpster", json);
}

void measurement(){
  int air = analogRead(smoke);
  int weight = analogRead(potentiometer);
  weight = map(weight, 0, 250, 1100, 0);
  Serial.print("Gas: ");
  Serial.println(air);
  Serial.print("Weight: ");
  Serial.println(weight);
  delay(100);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2; 
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  StaticJsonDocument<128> doc;

  doc["DeviceID"] = "100";
  doc["Name"] = "random";
  doc["Fill"] = String(distance);
  doc["Air"] = String(air);
  doc["Smell"] = "200";
  doc["Weight"] = String(weight);
  doc["Lon"] = "48.75335";
  doc["Lat"] = "21.9224";

  serializeJson(doc, json);
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
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

