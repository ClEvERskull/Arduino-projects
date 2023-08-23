#include "mastermind.h"
#include "lcd_wrapper.h"

void setup(){
  pinMode(A0, INPUT);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  lcd_init();
  randomSeed(analogRead(A1));
  Serial.begin(115200);
}

void loop(){
  char* code = generate_code(false, 4);
  Serial.println(code);
  Serial.println(A0);
  play_game(code);
  free(code);
}
