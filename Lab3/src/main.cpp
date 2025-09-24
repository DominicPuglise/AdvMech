#include <Arduino.h>
#include <SparkFun_Qwiic_OLED.h>
#include <Adafruit_BME280.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <stdint.h>

#define MICRO


QwiicMicroOLED myOLED;
const char* deviceName = "Micro OLED";

int yoffset;
Adafruit_BME280 bme;

// Define buttons (example, update with your actual pins)
const int buttonUpPin = 2;     // Up button to increase temperature
const int buttonDownPin = 3;   // Down button to decrease temperature
const int buttonSelectPin = 4; // Select button to cycle through states

enum MachineState {
    DisplayTemps,
    SetTemp,
    ChooseSystem
};

MachineState currentState;

float targetTemperature = 21.0;  // Target temperature in Celsius
char degreeSystem[] = "C";       // Temperature unit (Celsius by default)
bool heaterOn = false;           // Heater status

void setup() {
    Serial.begin(115200);
    delay(500);

    if (!myOLED.begin()) {
        Serial.println("- Device Begin Failed");
        while (1);
    }
    yoffset = (myOLED.getHeight() - myOLED.getFont()->height) / 2;

    if (!bme.begin(0x77, &Wire)) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }

    // Initialize button pins
    pinMode(buttonUpPin, INPUT_PULLUP);
    pinMode(buttonDownPin, INPUT_PULLUP);
    pinMode(buttonSelectPin, INPUT_PULLUP);

    currentState = DisplayTemps; // Start in DisplayTemps state
}

void scroll_right(void) {
    myOLED.scrollStop();
    myOLED.scrollRight(0, 7, SCROLL_INTERVAL_2_FRAMES);
}

void scroll_stop(void) {
    myOLED.scrollStop();
}

void updateHeaterStatus(float currentTemp) {
    // Turn on heater if the current temp is lower than target
    heaterOn = (currentTemp < targetTemperature);
}

void displayTemps() {
    float currentTemp = bme.readTemperature();
    updateHeaterStatus(currentTemp);

    char Text[50];
    if (strcmp(degreeSystem, "F") == 0) {
        currentTemp = currentTemp * 9.0 / 5.0 + 32.0; 
        sprintf(Text, "Trgt=%.1f *F\n", targetTemperature * 9.0 / 5.0 + 32.0);
        sprintf(Text + strlen(Text), "Crnt=%.1f *F\n", currentTemp);
    } else {
        sprintf(Text, "Trgt=%.1f *C\n", targetTemperature);
        sprintf(Text + strlen(Text), "Crnt=%.1f *C\n", currentTemp);
    }

    myOLED.erase();
    myOLED.text(3, yoffset, Text);
    myOLED.display();

    // Display heater status
    myOLED.setCursor(0, yoffset + 30);
    myOLED.print(heaterOn ? "Heater: ON" : "Heater: OFF");
    myOLED.display();

    delay(300);
}

void setTemp() {
    char Text[50];
    sprintf(Text, "Ttar: %.1f %s", targetTemperature, degreeSystem);
    myOLED.erase();
    myOLED.text(3, yoffset, Text);
    myOLED.display();
}

void chooseSystem() {
    char Text[50];
    sprintf(Text, "System: %s", degreeSystem);
    myOLED.erase();
    myOLED.text(3, yoffset, Text);
    myOLED.display();
}

void loop() {
    float currentTemp = bme.readTemperature();

    // Button control for cycling through states
    if (digitalRead(buttonSelectPin) == LOW) {
        delay(300); // debounce delay
        currentState = (MachineState)((currentState + 1) % 3);
    }

    if (currentState == DisplayTemps) {
        displayTemps();
    } else if (currentState == SetTemp) {
        // Button control for increasing/decreasing target temperature
        if (digitalRead(buttonUpPin) == LOW) {
            delay(300); // debounce delay
            targetTemperature += 1.0;
        }
        if (digitalRead(buttonDownPin) == LOW) {
            delay(300); // debounce delay
            targetTemperature -= 1.0;
        }
        setTemp();
    } else if (currentState == ChooseSystem) {
        // Button control for switching between Celsius and Fahrenheit
        if (digitalRead(buttonUpPin) == LOW) {
            delay(300); // debounce delay
            if (strcmp(degreeSystem, "C") == 0) {
                strcpy(degreeSystem, "F");
            } else {
                strcpy(degreeSystem, "C");
            }
        }
        chooseSystem();
    }
}
