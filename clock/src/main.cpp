#include <Arduino.h>
#include <WiFi.h>
#include <time.h>



// was not able to get clock to reliably set to the correct time primarily due to inaccuracies in homing 


const char* ssid     = "NSA Security Van HW";
const char* password = "windowtohallway";





const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -21600;     
const int daylightOffset_sec = 3600;   

const int STEP1 = 9;
const int DIR1  = 10;
const int STEP2 = 11;
const int DIR2  = 12;


const int MOTOR1_HOME_PIN = 13;  // digital input, HIGH = triggered
const int MOTOR2_HOME_PIN = A3;  // analog input, > threshold = triggered
const int ANALOG_THRESHOLD = 900;


const int STEP_DELAY = 7;   // microseconds between step pulses
const int STEPS_PER_MOVE = 16; // 16 steps = one "time advance" tick

int currentHour   = 0;  // Position the clock *thinks* it is at
int currentMinute = 0; 


void moveSteps(int stepPin, int dirPin, int steps) {
  digitalWrite(dirPin, HIGH);
  for (int i = 0; i < steps; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(STEP_DELAY);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(STEP_DELAY);
  }
}


void homeMotor1() {
  Serial.println("Homing Motor 1...");
  while (digitalRead(MOTOR1_HOME_PIN) == LOW) {
    moveSteps(STEP1, DIR1, 1);
  }
  Serial.println("Motor 1 homed.");
}

void homeMotor2() {
  Serial.println("Homing Motor 2...");
  while (analogRead(MOTOR2_HOME_PIN) < ANALOG_THRESHOLD) {
    moveSteps(STEP2, DIR2, 1);
  }
  Serial.println("Motor 2 homed.");
}




void get_time(int &hr, int &mn) {
  struct tm timeinfo;
  getLocalTime(&timeinfo);

  hr = timeinfo.tm_hour;
  mn = timeinfo.tm_min;

  Serial.printf("TIME: %02d:%02d\n", hr, mn);
}






void set_time() {
  int realHour, realMinute;
  get_time(realHour, realMinute);
  int targetMinute = (realMinute / 5) * 5;




  // set motor 2
  int hourDifference = (realHour % 12) - (currentHour % 12);
  if (hourDifference < 0) hourDifference += 12;

  while (hourDifference > 0) {
    moveSteps(STEP2, DIR2, STEPS_PER_MOVE);
    hourDifference--;
    currentHour = (currentHour + 1) % 12;
  }



  // set motor 1
  int minuteDifference = (targetMinute - currentMinute) / 5;
  if (minuteDifference < 0) minuteDifference += 12;

  while (minuteDifference > 0) {
    moveSteps(STEP1, DIR1, STEPS_PER_MOVE);
    minuteDifference--;
    currentMinute += 5;
    if (currentMinute >= 60) currentMinute = 0;
  }

}





void setup() {
  Serial.begin(115200);

  // Stepper setup
  pinMode(STEP1, OUTPUT);
  pinMode(DIR1, OUTPUT);
  pinMode(STEP2, OUTPUT);
  pinMode(DIR2, OUTPUT);

  pinMode(MOTOR1_HOME_PIN, INPUT);
  pinMode(MOTOR2_HOME_PIN, INPUT);

  Serial.println("Connecting WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  Serial.println("Starting homing...");
  homeMotor1();
  homeMotor2();
  Serial.println("Homing Complete!");

  set_time();
}






void loop() {
  static unsigned long lastSync = 0;

  if (millis() - lastSync >= 300000) {  // update every 5 minutes
    lastSync = millis();
    set_time();
  }

  delay(1000);
}
