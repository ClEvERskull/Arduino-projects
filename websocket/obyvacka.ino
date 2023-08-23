    
    #include <ESP8266WiFi.h>
    #include <WebSocketClient.h>
    #include <ArduinoJson.h>
    #include <SPI.h>
    #include <ESP8266HTTPClient.h>
    #include <ESP8266WiFi.h>
    #include <ESP8266WiFiMulti.h>
    #include <WiFiClientSecureBearSSL.h>
    #include <Wire.h>
    #include <Adafruit_GFX.h>
    #include <Adafruit_SSD1306.h>
    #include <Servo.h> 

    #define SCREEN_WIDTH 128 // OLED display width, in pixels
    #define SCREEN_HEIGHT 64 // OLED display height, in pixels
    #define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
    Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
     
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
    int servo = 16;
    Servo zaclony;
    int kanal = 1;
    int hlasitost = 10;

    char* applianceId;
    char* elementId;
    char* id;
    char* scheduleState;
    char* sentAt;
    char* triggerAt;
    char* value;
    
    
    void setup() {
      Serial.begin(115200); 
      zaclony.attach(servo);
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

        if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
    // Clear the buffer
  display.clearDisplay();

  // Draw a single pixel in white
  display.drawPixel(10, 10, WHITE);

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
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

          if (data.length() > 0) {
              Serial.println(data);
              if (abs(currentMillis - previousMillis) >= interval) {
                  previousMillis = currentMillis;
                  StaticJsonDocument<128> doc;
                    doc["applianceSerialNumber"] = id;
                    doc["elementId"] = "1";
                    doc["messageType"] = "ERROR";
                    doc["value"] = String(kanal);
                    serializeJson(doc, json);
                    WiFiClientSecure client;
                    client.setInsecure(); 
                    client.connect("https://bp-smart-env-api.herokuapp.com/app/iot", 443);
                    webSocketClient.sendData(json);
              }
          }
          delay(5);
      }
      if(value == "ch+"){
          kanal++;
          zobraz();
          value = "0";
      }
      else if(value == "ch-"){
          kanal--;
          zobraz();
          value = "0";
      }
      else if(value == "v+"){
          hlasitost++;
          zobraz();
          value = "0";
      }
      else if(value == "v-"){
          hlasitost--;
          zobraz();
          value = "0";
      }
      else if(value == "servo"){
          if(zaclony.read() == 180)
                zaclony.write(0);
            else
                zaclony.write(180);
            value = "0";
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

    void zobraz(){
          display.clearDisplay();
          display.setTextSize(1);
          display.setTextColor(WHITE);
          display.setCursor(0, 10);
          display.println("kanal:");
          display.setCursor(8, 10);
          display.println(kanal);
          display.setCursor(0, 20);
          display.println("hlasitost:");
          display.setCursor(11, 20);
          display.println(hlasitost);
          display.display(); 
    }
