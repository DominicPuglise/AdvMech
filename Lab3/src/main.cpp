#define MICRO
#include <stdint.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <SparkFun_Qwiic_OLED.h>
#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme;




#if defined(TRANSPARENT)
QwiicTransparentOLED myOLED;
const char * deviceName = "Transparent OLED";
#elif defined(NARROW)
QwiicNarrowOLED myOLED;
const char * deviceName = "Narrow OLED";
#else
QwiicMicroOLED myOLED;
const char * deviceName = "Micro OLED";
#endif





int yoffset;
float targetTemperature = 20.0;
char degreeSys = 'C';

int pinButton = 12;
int pinUp = 13;
int pinDown = 11;
int heater = A0;

bool prevPressed = false;
bool prevUp = false;
bool prevDown = false;

enum MachineStates {
    DisplayTemps,
    SetTemp,
    ChooseSystem
};

MachineStates currentState;







void setup()
{
    currentState = DisplayTemps;
    pinMode(pinButton, INPUT_PULLDOWN);
    pinMode(pinUp, INPUT_PULLDOWN);
    pinMode(pinDown, INPUT_PULLDOWN);
    pinMode(heater, OUTPUT);

    Serial.begin(9600);
    delay(3000);
    if (! bme.begin(0x77, &Wire)) {
        while (1);
    }
    delay(500);
    if(!myOLED.begin()){
        while(1);
    }
    yoffset = (myOLED.getHeight() - myOLED.getFont()->height)/2;
    delay(1000);
}



float cToF(float degC) {
    return (degC * 9.0 / 5.0) + 32.0;
}




void loop(){
    float temp = bme.readTemperature();
    if (degreeSys == 'F') temp = cToF(temp);

    if(targetTemperature > temp){digitalWrite(heater, LOW);}
    else{digitalWrite(heater, HIGH);}

    if(digitalRead(pinButton) && !prevPressed) {
        currentState = MachineStates(((int)currentState + 1) % 3);
    }
    prevPressed = digitalRead(pinButton);





    
    char myNewText[50];
    if (currentState == DisplayTemps) {
        sprintf(myNewText, "Tc: %.1f ", temp );
        myOLED.erase();
        myOLED.text(3, yoffset, myNewText);
        sprintf(myNewText, "Ttar: %.1f", targetTemperature);
        myOLED.text(3, yoffset + 12, myNewText);
        myOLED.display();
    } else if (currentState == SetTemp) {
        if (digitalRead(pinUp) && !prevUp) {
            targetTemperature++;
        }
        if (digitalRead(pinDown) && !prevDown) {
            targetTemperature--;
        }
        prevUp = digitalRead(pinUp);
        prevDown = digitalRead(pinDown);
        sprintf(myNewText, "Ttar: %.1f", targetTemperature);
        myOLED.erase();
        myOLED.text(3, yoffset, myNewText);
        myOLED.display();





    } else if (currentState == ChooseSystem) {
        if (digitalRead(pinUp) && !prevUp) {
            degreeSys = (degreeSys == 'C') ? 'F' : 'C';
        }
        prevUp = digitalRead(pinUp);
        sprintf(myNewText, "System: %c", degreeSys);
        myOLED.erase();
        myOLED.text(3, yoffset, myNewText);
        myOLED.display();
    }
}
