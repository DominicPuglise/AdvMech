#include <Arduino.h>


// ---------------- PIN MAP ----------------
#define X_A 34
#define X_B 35

#define Y_A 32
#define Y_B 33

#define Z_A 25
#define Z_B 26

#define BUTTON 27
#define SYSTEM_OUTPUT 14    // HIGH when system is ON


// -------------------------------------------------------
// Encoder struct (must appear BEFORE updateEncoder())
// -------------------------------------------------------
struct Encoder {
  uint8_t prevState;
  unsigned long lastTime;
  unsigned long interval;

  uint8_t speedBuf[5];  // last 5 samples: 1 = FAST, 0 = SLOW
  uint8_t bufIndex;
};

Encoder encX, encY, encZ;


// -------------------------------------------------------
// Quadrature encoder handler (no position tracking)
// -------------------------------------------------------
void updateEncoder(Encoder &enc, uint8_t A, uint8_t B, const char *axisName) {

  uint8_t a = digitalRead(A);
  uint8_t b = digitalRead(B);
  uint8_t state = (a << 1) | b;

  if (state != enc.prevState) {
    unsigned long now = micros();
    enc.interval = now - enc.lastTime;
    enc.lastTime = now;

    int8_t direction = 0;

    // Quadrature decoding
    if ((enc.prevState == 0b00 && state == 0b01) ||
        (enc.prevState == 0b01 && state == 0b11) ||
        (enc.prevState == 0b11 && state == 0b10) ||
        (enc.prevState == 0b10 && state == 0b00)) {
      direction = 1;   // CW
    }
    else if ((enc.prevState == 0b00 && state == 0b10) ||
             (enc.prevState == 0b10 && state == 0b11) ||
             (enc.prevState == 0b11 && state == 0b01) ||
             (enc.prevState == 0b01 && state == 0b00)) {
      direction = -1;  // CCW
    }

    enc.prevState = state;

    if (direction != 0 && enc.interval > 0) {
      float speed = 1e6f / enc.interval;
      uint8_t isFast = (speed > 1000) ? 1 : 0;

      // ----- Smooth speed using last 5 samples -----
      enc.speedBuf[enc.bufIndex] = isFast;
      enc.bufIndex = (enc.bufIndex + 1) % 5;

      uint8_t sum = 0;
      for (int i = 0; i < 5; i++)
        sum += enc.speedBuf[i];

      bool smoothedFast = (sum >= 3);  // majority vote

      // ----- Direction output rules -----
      char sign;
      if (axisName[0] == 'Z') {
        // Z: CCW = negative
        sign = (direction == -1) ? '-' : '+';
      } else {
        // X and Y: CW = negative
        sign = (direction == 1) ? '-' : '+';
      }

      Serial.print(axisName);
      Serial.print("  Dir: ");
      Serial.print(sign);
      Serial.print("\tSpeed: ");
      Serial.println(smoothedFast ? "FAST" : "SLOW");
    }
  }
}


// -------------------------------------------------------
// TOGGLE BUTTON VARIABLES
// -------------------------------------------------------
bool systemOn = false;
bool lastButtonState = LOW;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 30;   // ms


// -------------------------------------------------------
// SETUP
// -------------------------------------------------------
void setup() {
  Serial.begin(115200);

  pinMode(X_A, INPUT);
  pinMode(X_B, INPUT);
  pinMode(Y_A, INPUT);
  pinMode(Y_B, INPUT);
  pinMode(Z_A, INPUT);
  pinMode(Z_B, INPUT);

  pinMode(BUTTON, INPUT);
  pinMode(SYSTEM_OUTPUT, OUTPUT);

  encX.prevState = (digitalRead(X_A) << 1) | digitalRead(X_B);
  encY.prevState = (digitalRead(Y_A) << 1) | digitalRead(Y_B);
  encZ.prevState = (digitalRead(Z_A) << 1) | digitalRead(Z_B);

  memset(encX.speedBuf, 0, 5);
  memset(encY.speedBuf, 0, 5);
  memset(encZ.speedBuf, 0, 5);

  digitalWrite(SYSTEM_OUTPUT, LOW);  // start OFF
}


// -------------------------------------------------------
// LOOP
// -------------------------------------------------------
void loop() {

  // ----------- Button toggle ON/OFF with debounce ----------
  bool reading = digitalRead(BUTTON);

  if (reading != lastButtonState)
    lastDebounceTime = millis();

  if ((millis() - lastDebounceTime) > debounceDelay) {
    static bool lastStableState = LOW;
    if (reading == HIGH && lastStableState == LOW) {
      systemOn = !systemOn;
      digitalWrite(SYSTEM_OUTPUT, systemOn ? HIGH : LOW);
    }
    lastStableState = reading;
  }

  lastButtonState = reading;


  // ----------- Only read encoders when ON -----------------
  if (systemOn) {
    updateEncoder(encX, X_A, X_B, "X");
    updateEncoder(encY, Y_A, Y_B, "Y");
    updateEncoder(encZ, Z_A, Z_B, "Z");
  }
}
