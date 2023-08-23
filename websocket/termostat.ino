    
    #include <ESP8266WiFi.h>
    #include <WebSocketClient.h>
    #include <ArduinoJson.h>
    #include <SPI.h>
    #include <ESP8266HTTPClient.h>
    #include <ESP8266WiFi.h>
    #include <ESP8266WiFiMulti.h>
    #include <WiFiClientSecureBearSSL.h>
    
    String json;    
    boolean handshakeFailed=0;
    String data= "";
    int teplota = 0;
    int heating = 5;
    int cooling = 4;
    char path[] = "/";   //identifier of this device
    const char* ssid = "ASUS_6C";
    char* adresa = "https://bp-smart-env-api.herokuapp.com/app/iot";  
    const int port= 443;
      
    WebSocketClient webSocketClient;
    unsigned long previousMillis = 0;
    unsigned long currentMillis;
    unsigned long interval=300; 
    WiFiClient client;

    char* applianceId;
    char* elementId;
    char* id;
    char* scheduleState;
    char* sentAt;
    char* triggerAt;
    char* value;
    
    
    void setup() {
      Serial.begin(115200);
      pinMode(heating, OUTPUT);     
      pinMode(cooling, OUTPUT);   
      delay(10);
      Serial.println();
      Serial.println();
      Serial.print("Connecting to ");
      Serial.println(ssid);
      
      WiFi.begin(ssid);
      char registered = char(EEPROM.read(0x0F))

      if ((WiFi.status() == WL_CONNECTED) && (registered == '1')) {
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
            EEPROM.write(addr, '0'); 
        }
        else{
            Serial.println("Error while registring.");
        }
    wsconnect();
    //  wifi_set_sleep_type(LIGHT_SLEEP_T);


    }
    void loop() {
      if (client.connected()) {
            currentMillis=millis(); 
            webSocketClient.getData(data);  
            DynamicJsonDocument doc(2048);
            deserializeJson(doc, data);
            applianceId = doc["applianceId"];
            elementId = doc["elementId"];
            id = doc["id"];
            scheduleState = doc["scheduleState"];
            sentAt = doc["sentAt"];
            triggerAt = doc["triggerAt"];
            value = doc["value"];
            number = int(value - 48);

            teplota = value.toInt();

          if(teplota > 30){
                digitalWrite(cooling, HIGH); 
          }
          else if(teplota < 16){
                digitalWrite(heating, HIGH);
          }
          else{
                digitalWrite(cooling, LOW);
                digitalWrite(heating, LOW);
          }
          delay(5);
      }
    }
    //*********************************************************************************************************************
    //***************function definitions**********************************************************************************
    void wsconnect(){
      // Connect to the websocket server
      if (client.connect(adresa, port)) {
        Serial.println("Connected");
      } else {
        Serial.println("Connection failed.");
          delay(1000);  
       
       if(handshakeFailed){
        handshakeFailed=0;
        ESP.restart();
        }
        handshakeFailed=1;
      }
      // Handshake with the server
      webSocketClient.path = path;
      webSocketClient.host = adresa;
      if (webSocketClient.handshake(client)) {
        Serial.println("Handshake successful");
      } else {
        
        Serial.println("Handshake failed.");
       delay(4000);  
       
       if(handshakeFailed){
        handshakeFailed=0;
        ESP.restart();
        }
        handshakeFailed=1;
      }
    }

    void zresetuj(){
        EEPROM.write(addr, '1'); 
    }
