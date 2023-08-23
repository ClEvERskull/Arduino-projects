    
    #include <ESP8266WiFi.h>
    #include <WebSocketClient.h>
    #include <ArduinoJson.h>
    #include <SPI.h>
    #include <ESP8266HTTPClient.h>
    #include <ESP8266WiFi.h>
    #include <ESP8266WiFiMulti.h>
    #include <WiFiClientSecureBearSSL.h>
    #include <DHT.h>
    #include <EEPROM.h>

    #define DHTTYPE DHT22
    #define DHTPIN  12

    DHT dht(DHTPIN, DHTTYPE);
    int addr = 0;
    int button = 14;
    String id = String("1"); 
    String json;    


    float temp;
    String T;
    boolean handshakeFailed=0;
    char path[] = "/";   
    const char* ssid = "ASUS_6C";
    char* adresa = "https://bp-smart-env-api.herokuapp.com/app/iot";  
    const int port= 443;
      
    WebSocketClient webSocketClient;
    unsigned long previousMillis = 0;
    unsigned long currentMillis;
    unsigned long interval=300; 
    WiFiClient client;
    
    
    void setup() {
      Serial.begin(115200);
      EEPROM.begin(512);
      attachInterrupt(digitalPinToInterrupt(button), zresetuj(), RISING);
      delay(10);
      Serial.println();
      Serial.println();
      Serial.print("Connecting to ");
      Serial.println(ssid);
      
      WiFi.begin(ssid);
      
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
      Serial.println("");
      Serial.println("WiFi connected");  
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
      delay(1000);
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
    }

    void loop() {
      if (client.connected()) {
          currentMillis=millis(); 
              if (abs(currentMillis - previousMillis) >= interval) {
                  previousMillis = currentMillis;
                  temp = dht.readTemperature();
                  T = String(temp,2);
                  Serial.println(T);
                  StaticJsonDocument<128> doc;
                  doc["applianceSerialNumber"] = id;
                  doc["elementId"] = "1";
                  doc["messageType"] = "DATA";
                  doc["value"] = T;
                  serializeJson(doc, json);
            
                  WiFiClientSecure client;
                  client.setInsecure(); 
                  client.connect("https://bp-smart-env-api.herokuapp.com/app/iot", 443);
                  webSocketClient.sendData(json);
                  Serial.println(json);
              }
          
          delay(5);
      }

    }
    void wsconnect(){
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
