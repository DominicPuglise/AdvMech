#include <Arduino.h>
#include <P1AM.h>
#include <MotorEncoder.h>

// modules
int Input_module = 1;
int Output_module = 2;

int valve_feeder = 14;

int turntable_cw = 1;
int turntable_ccw = 2;
int turntable_suction_limit = 1;
int turntable_conveyor_limit = 2;
int turntable_saw_limit = 4;
void turntable_to_suction(){
    P1.writeDiscrete(HIGH, Output_module, turntable_ccw);
    while(!P1.readDiscrete(Input_module, turntable_suction_limit)){;}
    P1.writeDiscrete(LOW, Output_module, turntable_ccw);
}
void turntable_to_conveyor(){
    P1.writeDiscrete(HIGH, Output_module, turntable_cw);
    while(!P1.readDiscrete(Input_module, turntable_conveyor_limit)){;}
    P1.writeDiscrete(LOW, Output_module, turntable_cw);
}
void turntable_to_saw(){
    P1.writeDiscrete(HIGH, Output_module, turntable_cw);
    while(!P1.readDiscrete(Input_module, turntable_saw_limit)){;}
    P1.writeDiscrete(LOW, Output_module, turntable_cw);
}

int valve_kiln_door = 13;
void open_door(){
    P1.writeDiscrete(HIGH, 2, 10);
    P1.writeDiscrete(HIGH, Output_module, valve_kiln_door);
}
void close_door(){
    P1.writeDiscrete(LOW, Output_module, valve_kiln_door);
}

int oven_inside_limit = 6;
int oven_outside_limit = 7;
int retract_oven_feeder_motor = 5;
int extend_oven_feeder_motor = 6;
void feeder_extend(){
    P1.writeDiscrete(LOW, Output_module, retract_oven_feeder_motor);
    P1.writeDiscrete(HIGH, Output_module, extend_oven_feeder_motor);
    while(!P1.readDiscrete(Input_module, oven_outside_limit)){;}
    P1.writeDiscrete(LOW, Output_module, extend_oven_feeder_motor);
}
void feeder_retract(){
    P1.writeDiscrete(HIGH, Output_module, retract_oven_feeder_motor);
    P1.writeDiscrete(LOW, Output_module, extend_oven_feeder_motor);
    while(!P1.readDiscrete(Input_module, oven_inside_limit)){;}
    P1.writeDiscrete(LOW, Output_module, retract_oven_feeder_motor);
}   

int gripper_towards_oven_motor = 7;
int gripper_towards_turntable_motor = 8;
int gripper_turntable_limit = 5;
int gripper_oven_limit = 8;
void gripper_to_oven(){
    P1.writeDiscrete(HIGH, Output_module, gripper_towards_oven_motor);
    P1.writeDiscrete(LOW, Output_module, gripper_towards_turntable_motor);
    while(!P1.readDiscrete(Input_module, gripper_oven_limit)){;}
    P1.writeDiscrete(LOW, Output_module, gripper_towards_oven_motor);
}
void gripper_to_turntable(){
    P1.writeDiscrete(HIGH, Output_module, gripper_towards_turntable_motor);
    P1.writeDiscrete(LOW, Output_module, gripper_towards_oven_motor);
    while(!P1.readDiscrete(Input_module, gripper_turntable_limit)){;}
    P1.writeDiscrete(LOW, Output_module, gripper_towards_turntable_motor);
}

int vacuum_valve = 11;
int lowering_valve = 12;
void grab(){
    P1.writeDiscrete(LOW, Output_module, vacuum_valve);
    P1.writeDiscrete(HIGH, Output_module, lowering_valve);
    delay(1000);
    P1.writeDiscrete(HIGH, Output_module, vacuum_valve);
    delay(500);
    P1.writeDiscrete(LOW, Output_module, lowering_valve);
}
void release(){
    P1.writeDiscrete(HIGH, Output_module, lowering_valve);
    delay(1000);
    P1.writeDiscrete(LOW, Output_module, vacuum_valve);
    delay(500);
    P1.writeDiscrete(LOW, Output_module, lowering_valve);
}

void standby(){
    open_door();
    feeder_extend();
    gripper_to_turntable();
    turntable_to_suction();
    
}

bool cooked = false;
int lamp = 9;
void bake(){
    feeder_retract();
    close_door();
    delay(500);
    P1.writeDiscrete(HIGH, Output_module, lamp);
    delay(3000);
    P1.writeDiscrete(LOW, Output_module, lamp);
    open_door();
    feeder_extend();
    cooked = true;
}

void setup() {
  delay(1000);
  Serial.begin(9600);
  delay(1000);
  while (!P1.init()) {
    Serial.print("Waiting for connection...");
  }
  Serial.println("Started");

  P1.writeDiscrete(LOW, Output_module, 11);
  P1.writeDiscrete(LOW, Output_module, 12);
  P1.writeDiscrete(LOW, Output_module, 13);
  P1.writeDiscrete(LOW, Output_module, 14);
  P1.writeDiscrete(LOW, Output_module, valve_feeder);
  P1.writeDiscrete(HIGH, 2, 10);
  
  standby();
  
  Serial.println("End of startup sequence");
}

int light_barrier = 9;
int motor_saw = 4;
int conveyor_motor = 3;
void loop() {

    Serial.println("waiting for puck");
    if(!cooked && !P1.readDiscrete(Input_module, light_barrier)){
        delay(500);
        bake();
        gripper_to_oven();
        grab();
        gripper_to_turntable();
        turntable_to_suction();
        release();
        turntable_to_saw();
            P1.writeDiscrete(HIGH, Output_module, motor_saw);
            delay(5000);
            P1.writeDiscrete(LOW, Output_module, motor_saw);
        turntable_to_conveyor();
            P1.writeDiscrete(HIGH, Output_module, valve_feeder);
            P1.writeDiscrete(HIGH, Output_module, conveyor_motor);
        delay(1000);
        P1.writeDiscrete(LOW, Output_module, valve_feeder);
        standby();
        delay(2000);
        P1.writeDiscrete(LOW, Output_module, conveyor_motor);
        
        cooked = false;
    }


}




