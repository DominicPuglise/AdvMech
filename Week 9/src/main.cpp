#include <AccelStepper.h>
#include <Arduino.h>
#include <MultiStepper.h>


AccelStepper xAxis(AccelStepper::DRIVER, 5, 4);
AccelStepper yAxis(AccelStepper::DRIVER, 6, 7);

MultiStepper cnc;



int target = 500;
int accel = 700;
int velocity = 700;

int xMin = 2;
int yMin = 10;

void Home(){
    xAxis.moveTo(-99999);
    yAxis.moveTo(-99999);
    while (digitalRead(xMin) || digitalRead(yMin)){
        if (digitalRead(xMin)){xAxis.run();}
        if (digitalRead(yMin)){yAxis.run();}
    }
    xAxis.setCurrentPosition(0);
    xAxis.moveTo(0);
    yAxis.setCurrentPosition(0);
    yAxis.moveTo(0);

}


void Rapid(int xPos, int yPos){
    xAxis.moveTo(xPos);
    yAxis.moveTo(yPos);
    while(xAxis.run() || yAxis.run()){
        Serial.println("moving to position");
    }


}

void Linear(int xPos, int yPos){
    long t[] = {xPos, yPos};
    cnc.moveTo(t);
    while (cnc.run()){
        Serial.println("linear move");
    }

}

void setup() {
    Serial.begin(9600);
    pinMode (xMin, INPUT);
    pinMode (yMin, INPUT);

    xAxis.setMaxSpeed(velocity);
    xAxis.setAcceleration(accel);


    cnc.addStepper(xAxis);
    cnc.addStepper(yAxis);

    Home();

    
    Rapid(200,300);
    delay(1000);
    Linear(500,500);
}

void loop(){
    xAxis.run();
    if (!xAxis.isRunning()){
        target = -target;

    }


}