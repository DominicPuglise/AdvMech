#ifndef MOTOR_ENCODER_H
#define MOTOR_ENCODER_H
#endif

#include <Arduino.h>
#include <P1AM.h>

class MotorEncoder {
    private:
        int modInput;
        int modOutput;
        int pinCW;
        int pinCCW;
        int pinEncoder;
        int pulseCount;
        bool prevState;
        int dir;
        int pin_LimitSwitch;

    public:
        MotorEncoder(int mInput, int mOutput, int pCW, int pCCW, int pE, int sw) :
        modInput(mInput), modOutput(mOutput), pinCW(pCW), pinCCW(pCCW), pinEncoder(pE), 
        pulseCount(0), prevState(LOW), dir(1), pin_LimitSwitch(sw) {;}

        void begin() {
            //instance = this;

        }

        void MoveCW() {
            P1.writeDiscrete(HIGH, modOutput, pinCW);
            P1.writeDiscrete(LOW, modOutput, pinCCW);
            dir = -1;
        }
        void MoveCCW() {
            P1.writeDiscrete(LOW, modOutput, pinCW);
            P1.writeDiscrete(HIGH, modOutput, pinCCW);
            dir = 1;
        }

        void Stop() {
            P1.writeDiscrete(LOW, modOutput, pinCW);
            P1.writeDiscrete(LOW, modOutput, pinCCW);
        }

        void Home(){
            
            while(!P1.readDiscrete(modInput, pin_LimitSwitch)){
                MoveCCW();
                Serial.println("Homing");
                delay(10);
            }
            Stop();
            ZeroPulse();
        }


        void UpdatePulse(){
            bool currentState = P1.readDiscrete(modInput, pinEncoder);
            
            if (currentState && !prevState) {
                pulseCount += dir;
            }

            prevState = currentState;
        }



        bool MoveTo(int targetPos) {
            UpdatePulse();
            Serial.print("Current Pos: ");
            Serial.println(pulseCount);
            if (pulseCount < targetPos) {
                MoveCCW();
                return false;
            }
            else if (pulseCount > targetPos) {
                MoveCW();
                return false;
            }
            else {
                Stop();
                return true;
            }
            return false;
        }

        void ZeroPulse(){
            pulseCount = 0;
        }

        int GetPulse(){
            return pulseCount;
        }



};


