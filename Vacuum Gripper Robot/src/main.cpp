#include <Arduino.h>
#include <P1AM.h>
#include <MotorEncoder.h>

// modules
int Input_module = 1;
int Output_module = 2;
int Analog_module = 3;

int ColorWaitingWhite = 11;
int ColorWaitingBlue = 12;
int ColorWaitingRed = 13;


// mod 1
int vertical_home_switch = 1; 
int horizontal_home_switch = 2;
int motor_encoder = 9;
int motor_limit_switch = 3;
int vertical_motor_limit_switch = 1;
int horizontal_motor_encoder = 7;
int horizontal_motor_limit_switch = 2;


// mod 2
int vertical_up = 1; 
int vertical_down = 2; 

int horizontal_forward = 3;
int horizontal_backward = 4;

int compressor = 7;

int motor_cw = 6;
int motor_ccw = 5;

int motor_up = 1;
int motor_down = 2;
int vert_motor_encoder = 5;

int motor_forward = 4;
int motor_backward = 3;


// MotorEncoder(int mInput, int mOutput, int pCw, int pCcw, int pE, int sw )
MotorEncoder turn_motor(Input_module, Output_module, motor_cw, motor_ccw, motor_encoder, motor_limit_switch);
MotorEncoder vertical_motor(Input_module, Output_module, motor_down, motor_up, vert_motor_encoder, vertical_motor_limit_switch);
MotorEncoder horizontal_motor(Input_module, Output_module, motor_forward, motor_backward, horizontal_motor_encoder, horizontal_motor_limit_switch);



bool White_detected() {
  return !P1.readDiscrete(Input_module, ColorWaitingWhite);
}

bool Blue_detected() {
  return !P1.readDiscrete(Input_module, ColorWaitingBlue);
}

bool Red_detected() {
  return !P1.readDiscrete(Input_module, ColorWaitingRed);
}


void setup() {
  delay(1000);
  Serial.begin(9600);
  delay(1000);

  while (!P1.init()) {
    Serial.print("Waiting for connection...");
  }
  Serial.println("Connected");


  P1.writeDiscrete(LOW, Output_module, vertical_up);
  P1.writeDiscrete(LOW, Output_module, vertical_down);
  P1.writeDiscrete(LOW, Output_module, horizontal_forward);
  P1.writeDiscrete(LOW, Output_module, horizontal_backward);

  horizontal_motor.begin();
  horizontal_motor.Home();
  horizontal_motor.ZeroPulse();
  delay(100);

  vertical_motor.begin();
  vertical_motor.Home();
  vertical_motor.ZeroPulse();
  delay(100);

  turn_motor.begin();
  turn_motor.Home();
  turn_motor.ZeroPulse();
  delay(100);


  while (!turn_motor.MoveTo(3) & !vertical_motor.MoveTo(3) & !horizontal_motor.MoveTo(3)){;};

}




void loop() {
  //if(White_detected()){Serial.println("White Trigger");}
  //if(Blue_detected()){Serial.println("Blue Trigger");}
  //if(Red_detected()){Serial.println("Red Trigger");}
  delay(200);
}

