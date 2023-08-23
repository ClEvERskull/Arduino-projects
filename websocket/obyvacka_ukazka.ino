    #include <ESP8266WiFi.h>
    #include <SPI.h>
    #include <ESP8266HTTPClient.h>
    #include <ESP8266WiFiMulti.h>
    #include <Wire.h>
    #include <Adafruit_GFX.h>
    #include <Adafruit_SSD1306.h>
    #include <Servo.h> 

    #define SCREEN_WIDTH 128 // OLED display width, in pixels
    #define SCREEN_HEIGHT 64 // OLED display height, in pixels
    #define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
    Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

    const char* ssid = "ASUS_6C";
    unsigned long previousMillis = 0;
    unsigned long currentMillis;
    unsigned long interval=300; 
    WiFiClient client;
    int servo = 16;
    Servo zaclony;
    int kanal = 1;
    int hlasitost = 10;
    
    
    void setup() {
      Serial.begin(115200); 
      zaclony.attach(servo);
      delay(10);
      Serial.println();
      Serial.println();
      Serial.print("Connecting to ");
      Serial.println(ssid);
      
      WiFi.begin(ssid);

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
    }


    void loop() {

        String value = Serial.read()
      
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