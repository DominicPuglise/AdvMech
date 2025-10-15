#include <Arduino.h>
#include <Wire.h>
#include "SCMD.h"
#include <PID_v1.h>




// I could not arrive at a smooth result with the PID library (as evident in the video). 
// I believe this is mostly due to the issues I've encountered with the incremental encoder at speeds greater than 0.5 revolution per second. 
// The output of the encoder is too smooth to distinguish between states, though this could likely be improved by decreasing the amount of holes in the encoder wheel. 



double Setpoint, Input, Output;
double Kp = 2, Ki = 0.1, Kd = 0.5;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);
SCMD motorDriver;

int sensorPin = A3;
int threshold = 1500;


bool prevHigh = false;
unsigned long lastTime = 0;
int pulseCount = 0;


void setup() {
  Serial.begin(9600);
  delay(1000);
  Wire.begin();
  delay(1000);



  Setpoint = 10.0;  

  Serial.print("Target speed set to: ");
  Serial.println(Setpoint);




  // motor
  motorDriver.settings.commInterface = I2C_MODE;  // Correct constant for I2C
  motorDriver.settings.I2CAddress = 0x5D;  // Default address, adjust if necessary
  motorDriver.settings.chipSelectPin = 10;  // Not used for I2C, but set if required
  while (motorDriver.begin() == false) {
    Serial.println("Failed to initialize motor driver. Retrying...");
    delay(500);
  }
  motorDriver.inversionMode(1, 0);  
  motorDriver.enable();

  
  // PID
  Input = 0;
  myPID.SetOutputLimits(0, 255);  
  myPID.SetMode(AUTOMATIC);

  lastTime = millis();
}

void loop() {


  int val = analogRead(sensorPin);
  bool isHigh = (val > threshold);
  if (isHigh && !prevHigh) {
    pulseCount++;
  }
  prevHigh = isHigh;

  


  unsigned long currentTime = millis();
  if (currentTime - lastTime >= 100) {

    // speed in pulses per second
    double elapsedTime = (currentTime - lastTime) / 1000.0;
    Input = pulseCount / elapsedTime;



    //Serial.print("Desired (setpoint): ");
    //Serial.print(Setpoint);
    //Serial.print(" | Actual (input): ");
    //Serial.println(Input);
    //Serial.println(isHigh);


    pulseCount = 0;
    lastTime = currentTime;

    
    myPID.Compute();
    motorDriver.setDrive(1, 0, (uint8_t)Output);
  }
}