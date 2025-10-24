#include <Arduino.h>
#include <P1AM.h>
#include <MotorEncoder.h>

int increment_modifier = 2;

// modules
int Input_module = 1;
int Output_module = 2;
int Analog_module = 3;

int ColorWaitingWhite = 11;
int ColorWaitingBlue = 13;
int ColorWaitingRed = 12;

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
int vacuum_valve = 8;

int motor_cw = 6;
int motor_ccw = 5;
int motor_up = 1;
int motor_down = 2;
int vert_motor_encoder = 5;
int motor_forward = 4;
int motor_backward = 3;

// MotorEncoder(int mInput, int mOutput, int pCw, int pCcw, int pE, int sw)
MotorEncoder turn_motor(Input_module, Output_module, motor_cw, motor_ccw, motor_encoder, motor_limit_switch);

MotorEncoder horizontal_motor(Input_module, Output_module, motor_forward, motor_backward, horizontal_motor_encoder, horizontal_motor_limit_switch);

MotorEncoder vertical_motor(Input_module, Output_module, motor_down, motor_up, vert_motor_encoder, vertical_motor_limit_switch);


void home_all(){

  vertical_motor.begin();
  delay(100);
  vertical_motor.Home();
  Serial.println("Homed vertical");

  horizontal_motor.begin();
  delay(100);
  horizontal_motor.Home();
  Serial.println("Homed horizontal");
  delay(100);

  delay(100);
  turn_motor.begin();
  delay(100);
  turn_motor.Home();
  Serial.println("Homed turn");
  delay(100);
};

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

  P1.writeDiscrete(HIGH, Output_module, compressor);
  P1.writeDiscrete(LOW, Output_module, vacuum_valve);

  home_all();

  horizontal_motor.ZeroPulse();
  vertical_motor.ZeroPulse();
  turn_motor.ZeroPulse();
}

int warehouse_signal = 9;
int warehouse_ready = 14;

void deliver(){
  while(!vertical_motor.MoveTo(5)){;};
  while(!horizontal_motor.MoveTo(5)){;};
  while(!turn_motor.MoveTo(330*increment_modifier)){;};
  while(!horizontal_motor.MoveTo(57*increment_modifier)){;};
  while(!vertical_motor.MoveTo(50*increment_modifier)){;};
  P1.writeDiscrete(LOW, Output_module, vacuum_valve);
  while(!vertical_motor.MoveTo(5)){;};
  home_all();

  P1.writeDiscrete(HIGH, Output_module, warehouse_signal);

  while(!P1.readDiscrete(Input_module,warehouse_ready)){
    Serial.println("Waiting for warehouse");
  }
  P1.writeDiscrete(LOW, Output_module, warehouse_signal);
  Serial.println("Ready");
}


int WarehouseReady = 14;

void loop() {
  Serial.println("Waiting for puck");
  
  if(P1.readDiscrete(Input_module,WarehouseReady)){

    if(P1.readDiscrete(Input_module, ColorWaitingBlue)){ // BLUE
      while(!turn_motor.MoveTo(73*increment_modifier)){;};
      while(!horizontal_motor.MoveTo(140*increment_modifier)){;};
      while(!vertical_motor.MoveTo(210*increment_modifier)){;};

      P1.writeDiscrete(HIGH, Output_module, vacuum_valve);
  
      delay(300);
      deliver();
      
    }

    if(P1.readDiscrete(Input_module, ColorWaitingRed)){ // RED
      while(!turn_motor.MoveTo(90*increment_modifier)){;};
      while(!horizontal_motor.MoveTo(110*increment_modifier)){;};
      while(!vertical_motor.MoveTo(210*increment_modifier)){;};
      P1.writeDiscrete(HIGH, Output_module, vacuum_valve);
      delay(300);
      deliver();
    }

    if(P1.readDiscrete(Input_module, ColorWaitingWhite)){ // WHITE
      while(!turn_motor.MoveTo(110*increment_modifier)){;};
      while(!horizontal_motor.MoveTo(90*increment_modifier)){;};
      while(!vertical_motor.MoveTo(210*increment_modifier)){;};
      P1.writeDiscrete(HIGH, Output_module, vacuum_valve);
      delay(300);
      deliver();
    }
  }
}
