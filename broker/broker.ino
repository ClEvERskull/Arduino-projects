#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "wifi";
const char* password = "heslo";
const char* mqtt_server = "broker.mqttdashboard.com";

const int modra = 13;
const int cervena = 15;

WiFiClient espClient1;
PubSubClient client(espClient1);

void setup()
{
  pinMode(modra, OUTPUT);
  pinMode(cervena, OUTPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  reconnect();
  }

void callback(char* topic, byte* payload, unsigned int length)
{
  if((char)payload[0] == 'o' && (char)payload[1] == 'n'){
  digitalWrite(cervena, HIGH);
  digitalWrite(modra, LOW);
  }
  else{
    digitalWrite(modra, HIGH);
    digitalWrite(cervena, LOW);
  }
}

void setup_wifi()
{
  delay(10);
  WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED)
   {
    delay(500);
    }
  }

 void reconnect()
 {
  while(!client.connected())
  {
    if (client.connect("ESP8266Client1"))
    {
      client.subscribe("ESP8266/connection_status");
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
    reconnect();
  }
  client.loop();
  }