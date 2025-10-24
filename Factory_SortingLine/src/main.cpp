#include <Arduino.h>
#include <P1AM.h>



// Modules
int modInput = 1;
int modOutput = 2;
int modAnalog = 3;

// Inputs
int pulse = 1;
int lbIn = 2;
int lbOut = 3;

// Outputs
int conv = 1;
int compressor = 2;
int ejectWhite = 3;
int ejectRed = 4;
int ejectBlue = 5;

// Analog Inputs
int color = 1;


enum MachineState {
  Waiting,
  ColorSensing,
  CountedMove,
  Eject
};
volatile MachineState currentState = Waiting;




int colorValue = 10000;
int moveSteps = 0;
bool prevKeyState = false;
bool currentKeyState = false;
String eject = "None";

void setup() {
  Serial.begin(9600);
  while (!P1.init()) {delay(10);}
  delay(1000);
  Serial.println("Sorting Line Initialized");
}

bool getPulseKey(){
  return P1.readDiscrete(modInput, pulse);
}
bool inputTriggered() {
  return !P1.readDiscrete(modInput, lbIn);
}

bool outputTriggered() {
  return !P1.readDiscrete(modInput, lbOut);
}
void toggleConveyor(bool state) {
  P1.writeDiscrete(state, modOutput, conv);
}
int readColor() {
  return P1.readAnalog(modAnalog, color);
}
void triggerEjector(String color) {
  int ejector = 0;
  if (color == "White") {
    ejector = ejectWhite;
  } else if (color == "Red") {
    ejector = ejectRed;
  } else if (color == "Blue") {
    ejector = ejectBlue;
  }
  P1.writeDiscrete(true, modOutput, ejector);
  delay(100);
  P1.writeDiscrete(false, modOutput, ejector);
}






void loop() {

  if(!P1.readDiscrete(modInput, 4)){
    P1.writeDiscrete(HIGH, modOutput, 6);
    Serial.println("White waiting");
  }else{
    P1.writeDiscrete(LOW, modOutput, 6);
  }

  if(!P1.readDiscrete(modInput, 5)){
    P1.writeDiscrete(HIGH, modOutput, 7);
    Serial.println("Red waiting");
  }else{
    P1.writeDiscrete(LOW, modOutput, 7);
  }

  if(!P1.readDiscrete(modInput, 6)){
    P1.writeDiscrete(HIGH, modOutput, 8);
    Serial.println("Blue waiting");
  }else{
    P1.writeDiscrete(LOW, modOutput, 8);
  }




  switch (currentState) {
    case Waiting:
      if (inputTriggered()) {
        currentState = ColorSensing;
        toggleConveyor(true);
        colorValue = 10000;
      }
      break;

    case ColorSensing:
      colorValue = min(readColor(), colorValue);

      if (outputTriggered()) {
        currentState = CountedMove;
        if (colorValue < 2500) {
          moveSteps = 4;
          eject = "White";


        } else if (colorValue < 5000) {
          moveSteps = 9;
          eject = "Red";


        } else {
          moveSteps = 13;
          eject = "Blue";
        }
        P1.writeDiscrete(true, modOutput, compressor);
      }
      break;

    case CountedMove:
      currentKeyState = getPulseKey();
      if (currentKeyState && !prevKeyState) {
        moveSteps--;
      }
      prevKeyState = currentKeyState;
      if (moveSteps <= 1) {
        currentState = Eject;
        toggleConveyor(false);
      }
      break;

    case Eject:
      Serial.println("Ejecting " + eject);
      triggerEjector(eject);
      eject = "None";
      P1.writeDiscrete(false, modOutput, compressor);
      currentState = Waiting;
      break;
  }
  delay(50);
}