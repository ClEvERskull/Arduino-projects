#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "wifi";
const char* password = "heslo";
const char* mqtt_server = "broker.mqttdashboard.com";

const int cervena = 15;
const int zelena = 13;
const int sensor = 5;
bool detect = false;

WiFiClient espClient;
PubSubClient client(espClient);

void setup()
{
  pinMode(cervena, OUTPUT);
  pinMode(zelena, OUTPUT);
  pinMode(sensor, INPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server,1883);
  reconnect();
}

 void setup_wifi()
 {
  delay(10);
  WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
      Serial.println("connecting");
      delay(500);
      }
 }

 void reconnect()
 {
  while (!client.connected())
  {
    if (client.connect("ESP8266Client") && detect)
    {
      client.publish("ESP8266/connection_status", "on");
      }
     else
     {
      delay(5000);
      }
    }
  }

void loop()
{
 if (!client.connected())
 {
   if(digitalRead(sensor)== LOW)
  {
    detect = true;
    digitalWrite(cervena, HIGH);
    digitalWrite(zelena, LOW);
  }
  else{
    detect = false;
    digitalWrite(cervena, LOW);
    digitalWrite(zelena, HIGH);
  }
    reconnect();
  }
  client.loop();
}