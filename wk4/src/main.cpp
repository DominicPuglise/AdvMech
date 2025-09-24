#include <Arduino.h>


volatile int buttonCounter = 0;
int switchPin = 10; 
bool prevPressed = false;
unsigned long debounceDelay = 50;
unsigned long prevTime = 0;



void ButtonPress() {
  unsigned long currentTime = millis();
  if (currentTime - prevTime > debounceDelay) {
    buttonCounter++;
    prevTime = currentTime;
  }
}


void setup() {
Serial.begin(9600);
delay(2000); // wait for serial monitor to open
pinMode(switchPin, INPUT_PULLUP);
attachInterrupt(switchPin, ButtonPress, FALLING);


}

void loop() {
  Serial.println(" ");
  Serial.println(buttonCounter);

  if (digitalRead(switchPin) && !prevPressed){
    buttonCounter++;
  }
  prevPressed = digitalRead(switchPin);
}

