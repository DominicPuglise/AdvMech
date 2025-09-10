#include <Arduino.h>

int x = 0;


void setup() {
  Serial.begin(115200);
  Serial.println("starting...");
  pinMode(9, OUTPUT);
  pinMode(A0, INPUT);  
}
 

void loop() {
  delay(250);
  int currentReading = analogRead(A0)/10;
  int x = 255-currentReading;
  analogWrite(9, x);
  Serial.println(currentReading);
}
