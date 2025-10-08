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

    public:
        MotorEncoder(int mInput, int mOutput, int pCW, int pCCW, int pE) :
        modInput(mInput), modOutput(mOutput), pinCW(pCW), pinCCW(pCCW), pinEncoder(pE), 
        pulseCount(0), prevState(LOW), dir(1) {;}

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


        void UpdatePulse(){
            bool currentState = P1.readDiscrete(modInput, pinEncoder);
            
            if (currentState && !prevState) {
                pulseCount += dir;
            }

            prevState = currentState;
        }



        bool MoveTo(int targetPos) {
            if (pulseCount < targetPos) {
                MoveCW();
                return false;
            }
            else if (pulseCount > targetPos) {
                MoveCCW();
                return false;
            }
            else {
                Stop();
                return true;
        }
        }

        void ZeroPulse(){
            pulseCount = 0;
        }


};


