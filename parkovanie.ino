#define led_green 2
#define led_red 3
#define sensor 4

bool parked = false;
bool reserved = false;

void readSensor();
void ligts();

void setup() {
pinMode(sensor,INPUT);
pinMode(led_green, OUTPUT);
pinMode(led_red, OUTPUT);
Serial.begin(9600);
}

void loop() {
  readSensor();
  delay(500);
}

void readSensor(){
  if(digitalRead(sensor)== LOW)
  {
    parked = true;
  }
  else{
    parked = false;
  }
  Serial.println(parked);
}

void ligts(){
  if(!parked && !reserved){
    digitalWrite(led_green, HIGH);
    digitalWrite(led_red, LOW);
  }
  else{
    digitalWrite(led_green, LOW);
    digitalWrite(led_green, HIGH);
  }
}