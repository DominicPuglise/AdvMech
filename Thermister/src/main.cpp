#include <Arduino.h>

float inToVoltage(int reading) {
  return float(reading) * 3.3 / 4095.0;
}

float voltageToResistance(float voltage) {
  return (3.3 - Vout) / (3.3 - voltage);
}


void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Thermister Voltage Reader");
  tempTimer = timerBegin(0, 80, true);
  timerAttachInterrupt(tempTimer, &getTemp, true);
  timerAlarmWrite(tempTimer, 1000000, true);
  timerAlarmEnable(tempTimer);
}





void loop() {
  int sensorValue = analogRead(A0);
  float voltage = inToVoltage(sensorValue);
  Serial.println(voltage);
  delay(10);
}

