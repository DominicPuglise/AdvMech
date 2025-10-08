#include <Arduino.h>
#include "SparkFun_LSM6DSV16X.h"
#include <Wire.h>

SparkFun_LSM6DSV16X myLSM;

// Structs for X,Y,Z data
sfe_lsm_data_t accelData;
sfe_lsm_data_t gyroData;

int buttonCounter = 0;
bool prevPressed = false;
int switchPin = 10;
unsigned long debounceDelay = 50;
unsigned long prevTime = 0;
unsigned long doublePressTime = 500;


enum PressType {
  NONE, //0
  SINGLE, //1
  DOUBLE, //2
  LONG //3
};
PressType currentPress = NONE;

enum MachineState {
  Offstate,
  TwoAxis,
  XAxis,
  YAxis,
  RawData,
  StateLength
};


volatile MachineState currentState = Offstate;
volatile PressType currentPress = NONE;





void buttonPress() {
  unsigned long currentTime = millis();
  if (currentTime - prevTime > debounceDelay) {
    if (currentTime - prevTime < doublePressTime) {
      currentPress = DOUBLE;
    } else {
      prevTime = currentTime;
      currentPress = SINGLE;
    }
  }
}





void setup() {
  wire.begin();
  Serial.begin(9600);
  delay(2000);
  pinMode(switchPin, INPUT_PULLDOWN);
  attachInterrupt(switchPin, buttonPress, RISING);


  if (!myLSM.begin())
    {
        Serial.println("Did not begin, check your wiring and/or I2C address!");
        while (1)
            ;
    }

    // Reset the device to default settings. This if helpful is you're doing multiple
    // uploads testing different settings.
    myLSM.deviceReset();

    // Wait for it to finish reseting
    while (!myLSM.getDeviceReset())
    {
        delay(1);
    }

    Serial.println("Board has been Reset.");
    Serial.println("Applying settings.");

    // Accelerometer and Gyroscope registers will not be updated
    // until read.
    myLSM.enableBlockDataUpdate();

    // Set the output data rate and precision of the accelerometer
    myLSM.setAccelDataRate(LSM6DSV16X_ODR_AT_7Hz5);
    myLSM.setAccelFullScale(LSM6DSV16X_16g);

    // Set the output data rate and precision of the gyroscope
    myLSM.setGyroDataRate(LSM6DSV16X_ODR_AT_15Hz);
    myLSM.setGyroFullScale(LSM6DSV16X_2000dps);

    // Enable filter settling.
    myLSM.enableFilterSettling();

    // Turn on the accelerometer's filter and apply settings.
    myLSM.enableAccelLP2Filter();
    myLSM.setAccelLP2Bandwidth(LSM6DSV16X_XL_STRONG);

    // Turn on the gyroscope's filter and apply settings.
    myLSM.enableGyroLP1Filter();
    myLSM.setGyroLP1Bandwidth(LSM6DSV16X_GY_ULTRA_LIGHT);

    
}






void loop() {
  if (currentPress == DOUBLE && currrentState != Offstate) {
    currentState = (MachineState)(((int)currentState + 1) % (int)StateLength);
    currentState - (MachineState)max((int)currentState, 1);
()

  if (currentPress != NONE) {
    switch (currentPress) {
      case SINGLE:
        Serial.println("Single Press Detected");
        break;
      case DOUBLE:
        Serial.println("Double Press Detected");
        break;
      case LONG:
        Serial.println("Long Press Detected");
        break;
      default:
        break;
    }


    if(currentPress != NONE)
    currentPress = NONE;

  
  }
  
}