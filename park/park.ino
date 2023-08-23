#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHTesp.h>

DHTesp dht;

const int sensor = A0;
const int out = 16;
const int teplomer = 14;

const char* ssid = "wifi";
const char* password = "heslo";

const char* mqtt_server = "192.168.0.99";

WiFiClient espClient;
PubSubClient client(espClient);

int value;
int humidity;
int temp;
static char json[128];

void setup_wifi();
void measurement();
void reconnect();

void setup() {
  pinMode(sensor, INPUT);
  pinMode(out, OUTPUT);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  dht.setup(teplomer, DHTesp::DHT11);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  if(!client.loop())
    client.connect("ESP8266Client");
  measurement();
  client.publish("data/parks", json);

  ESP.deepSleep(36e8);
}

void measurement(){
  digitalWrite(out, HIGH);
  value = analogRead(sensor);
  temp = dht.getTemperature();
  humidity = (value-110)*100L/(1023-110);
  Serial.println(humidity);
  String str;
  String tep;
  str=String(humidity);
  //str.toCharArray(message,2); 
  StaticJsonDocument<128> doc;

  doc["DeviceID"] = "100";
  doc["Humidity"] = str;
  doc["Temperature"] = tep;
  doc["Lon"] = "48.75335";
  doc["Lat"] = "21.9224";

  serializeJson(doc, json);
  digitalWrite(out, LOW);
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
