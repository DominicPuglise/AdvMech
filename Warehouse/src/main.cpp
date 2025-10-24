#include <Arduino.h>
#include <P1AM.h>
#include <MotorEncoder.h>

int Input_module = 1;  
int Output_module = 2;  

int gripper_signal = 11;   // Input
int warehouse_ready = 13;  // Output
bool loaded = false;

int vertical_home_switch = 4;
int vertical_encoder = 7;
int vertical_up = 6;
int vertical_down = 5;
MotorEncoder vertical_motor(Input_module, Output_module, vertical_down, vertical_up, vertical_encoder, vertical_home_switch);

int horizontal_home_switch = 1;
int horizontal_encoder = 5;
int horizontal_towards_rack = 3;
int horizontal_towards_conveyor = 4;
MotorEncoder horizontal_motor(Input_module, Output_module, horizontal_towards_rack, horizontal_towards_conveyor, horizontal_encoder, horizontal_home_switch);

int probe_arm_rear_limit = 10;
int probe_arm_backward = 8;
int probe_arm_forward_limit = 9;
int probe_arm_forward = 7;

int light_barrier_outside = 3;
int conveyor_forward = 1;

int light_barrier_inside = 2;
int conveyor_backward = 2;

int x_offset = -20;

bool A1_filled=false,A2_filled=false,A3_filled=false,
     B1_filled=false,B2_filled=false,B3_filled=false,
     C1_filled=false,C2_filled=false,C3_filled=false;

void probe_arm_extend() {
  P1.writeDiscrete(HIGH, Output_module, probe_arm_forward);
  while(!P1.readDiscrete(Input_module, probe_arm_forward_limit)){;}
  P1.writeDiscrete(LOW, Output_module, probe_arm_forward);
}

void probe_arm_retract() {
  P1.writeDiscrete(HIGH, Output_module, probe_arm_backward);
  while(!P1.readDiscrete(Input_module, probe_arm_rear_limit)){;}
  P1.writeDiscrete(LOW, Output_module, probe_arm_backward);
}

void home_all() {
  P1.writeDiscrete(HIGH, Output_module, probe_arm_backward);
  while(!P1.readDiscrete(Input_module, probe_arm_rear_limit)){;}
  vertical_motor.Home();
  P1.writeDiscrete(LOW, Output_module, probe_arm_backward);
  delay(100);
  horizontal_motor.Home();
  delay(100);
}

void conveyor_out() {
  P1.writeDiscrete(HIGH, Output_module, conveyor_forward);
  while (!P1.readDiscrete(Input_module, light_barrier_outside)) {;}
  delay(80);
  while (P1.readDiscrete(Input_module, light_barrier_outside)) {;}
  P1.writeDiscrete(LOW, Output_module, conveyor_forward);
}

void conveyor_in() {
  P1.writeDiscrete(LOW, Output_module, warehouse_ready);
  P1.writeDiscrete(HIGH, Output_module, conveyor_backward);
  while (!P1.readDiscrete(Input_module, light_barrier_inside)) {;}
  delay(80);
  while (P1.readDiscrete(Input_module, light_barrier_inside)) {;}
  P1.writeDiscrete(LOW, Output_module, conveyor_backward);
}

void block_A1(int a){int x=967+x_offset,y=30;P1.writeDiscrete(LOW, Output_module, warehouse_ready); if(a==0){while(!horizontal_motor.MoveTo(x));while(!vertical_motor.MoveTo(y));probe_arm_extend();while(!vertical_motor.MoveTo(y-30));probe_arm_retract();A1_filled=false;}else{while(!horizontal_motor.MoveTo(x));while(!vertical_motor.MoveTo(y-29));probe_arm_extend();while(!vertical_motor.MoveTo(y));probe_arm_retract();A1_filled=true;}}
void block_A2(int a){int x=677+x_offset,y=30;P1.writeDiscrete(LOW, Output_module, warehouse_ready);if(a==0){while(!horizontal_motor.MoveTo(x));while(!vertical_motor.MoveTo(y));probe_arm_extend();while(!vertical_motor.MoveTo(y-30));probe_arm_retract();A2_filled=false;}else{while(!horizontal_motor.MoveTo(x));while(!vertical_motor.MoveTo(y-29));probe_arm_extend();while(!vertical_motor.MoveTo(y));probe_arm_retract();A2_filled=true;}}
void block_A3(int a){int x=385+x_offset,y=30;P1.writeDiscrete(LOW, Output_module, warehouse_ready);if(a==0){while(!horizontal_motor.MoveTo(x));while(!vertical_motor.MoveTo(y));probe_arm_extend();while(!vertical_motor.MoveTo(y-30));probe_arm_retract();A3_filled=false;}else{while(!horizontal_motor.MoveTo(x));while(!vertical_motor.MoveTo(y-29));probe_arm_extend();while(!vertical_motor.MoveTo(y));probe_arm_retract();A3_filled=true;}}
void block_B1(int a){int x=968,y=230;P1.writeDiscrete(LOW, Output_module, warehouse_ready);if(a==0){while(!horizontal_motor.MoveTo(x));while(!vertical_motor.MoveTo(y));probe_arm_extend();while(!vertical_motor.MoveTo(y-50));probe_arm_retract();B1_filled=false;}else{while(!horizontal_motor.MoveTo(x));while(!vertical_motor.MoveTo(y-39));probe_arm_extend();while(!vertical_motor.MoveTo(y));probe_arm_retract();B1_filled=true;}}
void block_B2(int a){int x=677,y=230;P1.writeDiscrete(LOW, Output_module, warehouse_ready);if(a==0){while(!horizontal_motor.MoveTo(x));while(!vertical_motor.MoveTo(y));probe_arm_extend();while(!vertical_motor.MoveTo(y-50));probe_arm_retract();B2_filled=false;}else{while(!horizontal_motor.MoveTo(x));while(!vertical_motor.MoveTo(y-39));probe_arm_extend();while(!vertical_motor.MoveTo(y));probe_arm_retract();B2_filled=true;}}
void block_B3(int a){int x=380,y=230;P1.writeDiscrete(LOW, Output_module, warehouse_ready);if(a==0){while(!horizontal_motor.MoveTo(x));while(!vertical_motor.MoveTo(y));probe_arm_extend();while(!vertical_motor.MoveTo(y-50));probe_arm_retract();B3_filled=false;}else{while(!horizontal_motor.MoveTo(x));while(!vertical_motor.MoveTo(y-39));probe_arm_extend();while(!vertical_motor.MoveTo(y));probe_arm_retract();B3_filled=true;}}
void block_C1(int a){int x=969,y=420;P1.writeDiscrete(LOW, Output_module, warehouse_ready);if(a==0){while(!horizontal_motor.MoveTo(x));while(!vertical_motor.MoveTo(y));probe_arm_extend();while(!vertical_motor.MoveTo(y-50));probe_arm_retract();C1_filled=false;}else{while(!horizontal_motor.MoveTo(x));while(!vertical_motor.MoveTo(y-39));probe_arm_extend();while(!vertical_motor.MoveTo(y));probe_arm_retract();C1_filled=true;}}
void block_C2(int a){int x=677,y=420;P1.writeDiscrete(LOW, Output_module, warehouse_ready);if(a==0){while(!horizontal_motor.MoveTo(x));while(!vertical_motor.MoveTo(y));probe_arm_extend();while(!vertical_motor.MoveTo(y-50));probe_arm_retract();C2_filled=false;}else{while(!horizontal_motor.MoveTo(x));while(!vertical_motor.MoveTo(y-39));probe_arm_extend();while(!vertical_motor.MoveTo(y));probe_arm_retract();C2_filled=true;}}
void block_C3(int a){int x=380,y=420;P1.writeDiscrete(LOW, Output_module, warehouse_ready);if(a==0){while(!horizontal_motor.MoveTo(x));while(!vertical_motor.MoveTo(y));probe_arm_extend();while(!vertical_motor.MoveTo(y-50));probe_arm_retract();C3_filled=false;}else{while(!horizontal_motor.MoveTo(x));while(!vertical_motor.MoveTo(y-39));probe_arm_extend();while(!vertical_motor.MoveTo(y));probe_arm_retract();C3_filled=true;}}

void goto_conveyor(int action){
    P1.writeDiscrete(LOW, Output_module, warehouse_ready);
    int conveyor_x = 5;
    int conveyor_y = 339;
    if (action == 0) { // store filled box
        while(!horizontal_motor.MoveTo(conveyor_x)){;}
        while(!vertical_motor.MoveTo(conveyor_y)){;}
        probe_arm_extend();
        while(!vertical_motor.MoveTo(conveyor_y - 30)){;}
        probe_arm_retract();
    } else { // deliver empty box
        while(!horizontal_motor.MoveTo(conveyor_x)){;}
        while(!vertical_motor.MoveTo(conveyor_y - 39)){;}
        probe_arm_extend();
        while(!vertical_motor.MoveTo(conveyor_y)){;}
        probe_arm_retract();
    }
}

void deliver_empty(){
    P1.writeDiscrete(LOW, Output_module, warehouse_ready);
    if (!A1_filled) block_A1(0);
    else if (!A2_filled) block_A2(0);
    else if (!A3_filled) block_A3(0);
    else if (!B1_filled) block_B1(0);
    else if (!B2_filled) block_B2(0);
    else if (!B3_filled) block_B3(0);
    else if (!C1_filled) block_C1(0);
    else if (!C2_filled) block_C2(0);
    else if (!C3_filled) block_C3(0);
    goto_conveyor(1);
    conveyor_out();
    P1.writeDiscrete(HIGH, Output_module, warehouse_ready);
    delay(333);
    loaded = true;
    P1.writeDiscrete(LOW, Output_module, warehouse_ready);
}

void retrieve_filled(){
    P1.writeDiscrete(LOW, Output_module, warehouse_ready);
    loaded = false;
    conveyor_in();
    delay(333);
    goto_conveyor(0);
    if (!A1_filled) {block_A1(1); Serial.println("Filled box stored in A1"); return;}
    if (!A2_filled) {block_A2(1); Serial.println("Filled box stored in A2"); return;}
    if (!A3_filled) {block_A3(1); Serial.println("Filled box stored in A3"); return;}
    if (!B1_filled) {block_B1(1); Serial.println("Filled box stored in B1"); return;}
    if (!B2_filled) {block_B2(1); Serial.println("Filled box stored in B2"); return;}
    if (!B3_filled) {block_B3(1); Serial.println("Filled box stored in B3"); return;}
    if (!C1_filled) {block_C1(1); Serial.println("Filled box stored in C1"); return;}
    if (!C2_filled) {block_C2(1); Serial.println("Filled box stored in C2"); return;}
    if (!C3_filled) {block_C3(1); Serial.println("Filled box stored in C3"); return;}
    Serial.println("No empty slots available for filled box");
    P1.writeDiscrete(LOW, Output_module, warehouse_ready);
}

void setup() {
  horizontal_motor.begin();
  vertical_motor.begin();
  Serial.begin(9600);
  while(!P1.init()){;}
  home_all();
  deliver_empty();
}

int i = 1;
bool prev_gripper = false;

void loop() {
  bool curr_gripper = P1.readDiscrete(Input_module, gripper_signal);
  Serial.print("gripper_signal: "); Serial.print(curr_gripper); 
  Serial.print(", prev_gripper: "); Serial.print(prev_gripper); 
  Serial.print(", loaded: "); Serial.print(loaded); 
  Serial.print(", i: "); Serial.println(i);

  if (curr_gripper && !prev_gripper) { 
    Serial.println("Gripper signal detected, retrieving filled box");
    retrieve_filled();
    if (i < 9) {
      i++;
      Serial.println("Delivering new empty box");
      deliver_empty();
    }
  }
  prev_gripper = curr_gripper;

  if (i > 9) {
    Serial.println("All boxes filled. System idle.");
    while(true){;}
  }
}