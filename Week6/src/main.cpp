#include <Arduino.h>
#include <P1AM.h>
#include <MotorEncoder.h>



int modInput = 1;
int modOutput = 2;
int modAnalogInput = 3;

MotorEncoder motor1(modInput, modOutput, 4, 3, 7);// CW, CCW, Encoder


void setup() {
  delay(1000);
  Serial.begin(115200); 
  delay(1000);
  while (!P1.init()){
    Serial.println("No modules found");
  }
  Serial.println("Modules found");


}

void loop() {
  motor1.MoveCW();
  delay(1000);
  motor1.MoveCCW();
  delay(1000);
  motor1.Stop();
  delay(1000);
}

