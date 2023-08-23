    
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
    char* messageType;
    char* value;
    
    
    void setup() {
      Serial.begin(115200);   
      delay(10);
      Serial.println();
      Serial.println();
      Serial.print("Connecting to ");
      Serial.println(ssid);
      
      WiFi.begin(ssid);
      if(char(EEPROM.read(0x0F) == '0'){
        registruj();
      }
    wsconnect();
    }
    void loop() {
      if (client.connected()) {
          currentMillis=millis(); 
          prijmiData();
          if (data.length() > 0) {
              Serial.println(data);
              if (abs(currentMillis - previousMillis) >= interval) {
                  previousMillis = currentMillis;
                  odosliData();
              }
          }
          delay(5);
      }
    }
    
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
        EEPROM.write(addr, '0'); 
        regidtruj();
    }

    void registruj(){

      if (WiFi.status() == WL_CONNECTED) {
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
            EEPROM.write(addr, '1'); 
        }
        else{
            Serial.println("Error while registring.");
        }

    }

    void prijmiData(){
            webSocketClient.getData(data);  
            DynamicJsonDocument doc(2048);
            deserializeJson(doc, data);
            applianceId = doc["applianceId"];
            elementId = doc["elementId"];
            messageType = doc["messageType"];
            value = doc["value"];
    }
    void odosliData(){
                    StaticJsonDocument<128> doc;
                    doc["applianceSerialNumber"] = id;
                    doc["elementId"] = "1";
                    doc["messageType"] = "ERROR";
                    doc["value"] = "Otvorene";
                    serializeJson(doc, json);
                    WiFiClientSecure client;
                    client.setInsecure(); 
                    client.connect("https://bp-smart-env-api.herokuapp.com/app/iot", 443);
                    webSocketClient.sendData(json);
    }
