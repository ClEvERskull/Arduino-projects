#include <DHT.h>
#include <DHT_U.h>

#include <U8glib.h>

U8GLIB_SSD1306_128X64 oled(U8G_I2C_OPT_NONE);
#define DHTPIN 7     
#define DHTTYPE DHT22   
DHT dht(DHTPIN, DHTTYPE);

const int MQ = A0;
const int pohybTv = 2;
const int pohybNoc = 3;
const int led = 9;
int senzorTV = LOW;
int senzorNoc = LOW;

int ovzdusie, svetlo;
float vlhkost, teplo;
int brightness = 0;    
int fadeAmount = 10;

void setup(void) {
    Serial.begin(9600);
    dht.begin();
    oled.sleepOff();
    oled.setFont(u8g_font_unifont);
    pinMode(MQ, INPUT);
    pinMode(pohybTv, INPUT);
    pinMode(pohybNoc, INPUT);
    pinMode(led, OUTPUT);
}

void loop(void) {
    pohybPriTV();
    pohybVNoci();
    delay(500);
}

void merania(){
    ovzdusie = analogRead(MQ);
    vlhkost = dht.readHumidity();
    teplo = dht.readTemperature();
    Serial.print("Ovzdusie: ");
    Serial.println(ovzdusie);
    Serial.print("vlhkost: ");
    Serial.println(vlhkost);
    Serial.print("Teplota: ");
    Serial.println(teplo);
}

void obrazovka(){
    merania();
    oled.sleepOff();
    oled.setPrintPos(0, 10);
    oled.print("Teplota");
    oled.setPrintPos(0, 25);
    oled.print(teplo);
    oled.setPrintPos(0, 40);
    oled.print("vlhkost:");
    oled.setPrintPos(40, 55);
    oled.print(vlhkost);
    delay(500);
}

void obrazovkaOff(){
    oled.sleepOn();
}

void zapniLed(){
    while(brightness < 120){
        brightness = brightness + fadeAmount;
        analogWrite(led, brightness);
        delay(30);
    }
}

void vypniLed(){

    while(brightness > 0){
        brightness = brightness - fadeAmount;
        analogWrite(led, brightness);
        delay(30);
    }
}

void pohybPriTV(){
    int val = digitalRead(pohybTv);  // read input value
    if (val == HIGH) {            // check if the input is HIGH
        obrazovka();  // turn LED ON
        if (senzorTV == LOW) {
      // we have just turned on
            Serial.println("Motion detected!");
      // We only want to print on the output change, not state
            senzorTV = HIGH;
        }
    } else {
        obrazovkaOff(); // turn LED OFF
        if (senzorTV == HIGH){
      // we have just turned of
            Serial.println("Motion ended!");
      // We only want to print on the output change, not state
            senzorTV = LOW;
        }
    }
}

void pohybVNoci(){
    int val = digitalRead(pohybNoc);  // read input value
    if (val == HIGH) {            // check if the input is HIGH
        zapniLed();  // turn LED ON
        if (senzorNoc == LOW) {
      // we have just turned on
            Serial.println("Motion detected!");
      // We only want to print on the output change, not state
            senzorNoc = HIGH;
        }
    } else {
        vypniLed(); // turn LED OFF
        if (senzorNoc == HIGH){
      // we have just turned of
            Serial.println("Motion ended!");
      // We only want to print on the output change, not state
            senzorNoc = LOW;
        }
    }
}
