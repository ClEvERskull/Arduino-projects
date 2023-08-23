    
    #include <ESP8266WiFi.h>
    #include <WebSocketClient.h>
    #include <ArduinoJson.h>
    #include <SPI.h>
    #include <ESP8266HTTPClient.h>
    #include <ESP8266WiFi.h>
    #include <ESP8266WiFiMulti.h>
    #include <WiFiClientSecureBearSSL.h>

    String id = String("3"); 
    String json;    
    const int led = 9;
    const int dvierka = 10;
    int button = 14;
    int opened = -500;
    bool nahlas = false;

    boolean handshakeFailed=0;
    String data= "";
    char path[] = "/";   //identifier of this device
    const char* ssid = "ASUS_6C";
    char* adresa = "https://bp-smart-env-api.herokuapp.com/app/iot";  
    const int port= 443;
      
    WebSocketClient webSocketClient;
    WiFiClient client;
    
    
    void setup() {
      Serial.begin(115200);
      EEPROM.begin(512);
      attachInterrupt(digitalPinToInterrupt(button), zresetuj(), RISING);
      pinMode(led, OUTPUT);
      pinMode(dvierka, INPUT);
      delay(10);
      // We start by connecting to a WiFi network
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
        if(digitalRead(dvierka) == HIGH){
            digitalWrite(led, HIGH);
            opened += 500;
        }
        else if(digitalRead(dvierka) == LOW){
          digitalWrite(led, LOW);
        }

        if(opened > 30000){
                nahlas = true
                opened = 0;
        }
        else{
                opened = 0;
        }
      if (nahlas) {
              if (client.connected()) {
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
                    Serial.println(json);
              }
          }
          delay(5);
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