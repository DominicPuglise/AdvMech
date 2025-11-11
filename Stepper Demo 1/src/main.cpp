#include <Arduino.h>
#include "Stepper.h"

// ------------------- PINS -------------------
Stepper xAxis(5, 4, 3, 2, 12);  // STEP, DIR, minSwitch, maxSwitch, enable

// ------------------- NOTE STRUCT -------------------
struct Note {
  float freq;         // frequency in Hz
  unsigned int len;   // duration in milliseconds
};

// ------------------------------------------------------------
// DATA-DRIVEN NOTE STRUCT
// ------------------------------------------------------------
struct Note {
  float freq;
  uint16_t ms;
};

const uint16_t BPM = 96;
const uint16_t SIXTEENTH = (60000 / BPM) / 4;  // length of each 16th note

// ------------------------------------------------------------
// PRELUDE IN C MAJOR (BWV 846) – FIRST PAGE (measures 1–22)
// ------------------------------------------------------------

const Note prelude[] = {

  // ---------- MEASURE 1 ----------
  {261.63, SIXTEENTH}, {329.63, SIXTEENTH}, {392.00, SIXTEENTH}, {523.25, SIXTEENTH},
  {261.63, SIXTEENTH}, {329.63, SIXTEENTH}, {392.00, SIXTEENTH}, {523.25, SIXTEENTH},

  // ---------- MEASURE 2 ----------
  {293.66, SIXTEENTH}, {349.23, SIXTEENTH}, {440.00, SIXTEENTH}, {587.33, SIXTEENTH},
  {293.66, SIXTEENTH}, {349.23, SIXTEENTH}, {440.00, SIXTEENTH}, {587.33, SIXTEENTH},

  // ---------- MEASURE 3 ----------
  {329.63, SIXTEENTH}, {392.00, SIXTEENTH}, {493.88, SIXTEENTH}, {659.26, SIXTEENTH},
  {329.63, SIXTEENTH}, {392.00, SIXTEENTH}, {493.88, SIXTEENTH}, {659.26, SIXTEENTH},

  // ---------- MEASURE 4 ----------
  {349.23, SIXTEENTH}, {440.00, SIXTEENTH}, {523.25, SIXTEENTH}, {698.46, SIXTEENTH},
  {349.23, SIXTEENTH}, {440.00, SIXTEENTH}, {523.25, SIXTEENTH}, {698.46, SIXTEENTH},

  // ---------- MEASURE 5 ----------
  {392.00, SIXTEENTH}, {493.88, SIXTEENTH}, {587.33, SIXTEENTH}, {783.99, SIXTEENTH},
  {392.00, SIXTEENTH}, {493.88, SIXTEENTH}, {587.33, SIXTEENTH}, {783.99, SIXTEENTH},

  // ---------- MEASURE 6 ----------
  {440.00, SIXTEENTH}, {523.25, SIXTEENTH}, {659.26, SIXTEENTH}, {880.00, SIXTEENTH},
  {440.00, SIXTEENTH}, {523.25, SIXTEENTH}, {659.26, SIXTEENTH}, {880.00, SIXTEENTH},

  // ---------- MEASURE 7 ----------
  {493.88, SIXTEENTH}, {587.33, SIXTEENTH}, {698.46, SIXTEENTH}, {987.77, SIXTEENTH},
  {493.88, SIXTEENTH}, {587.33, SIXTEENTH}, {698.46, SIXTEENTH}, {987.77, SIXTEENTH},

  // ---------- MEASURE 8 ----------
  {523.25, SIXTEENTH}, {659.26, SIXTEENTH}, {783.99, SIXTEENTH}, {1046.50, SIXTEENTH},
  {523.25, SIXTEENTH}, {659.26, SIXTEENTH}, {783.99, SIXTEENTH}, {1046.50, SIXTEENTH},

  // ---------- MEASURE 9 ----------
  {493.88, SIXTEENTH}, {587.33, SIXTEENTH}, {698.46, SIXTEENTH}, {880.00, SIXTEENTH},
  {493.88, SIXTEENTH}, {587.33, SIXTEENTH}, {698.46, SIXTEENTH}, {880.00, SIXTEENTH},

  // ---------- MEASURE 10 ----------
  {440.00, SIXTEENTH}, {523.25, SIXTEENTH}, {659.26, SIXTEENTH}, {783.99, SIXTEENTH},
  {440.00, SIXTEENTH}, {523.25, SIXTEENTH}, {659.26, SIXTEENTH}, {783.99, SIXTEENTH},

  // ---------- MEASURE 11 ----------
  {392.00, SIXTEENTH}, {493.88, SIXTEENTH}, {587.33, SIXTEENTH}, {698.46, SIXTEENTH},
  {392.00, SIXTEENTH}, {493.88, SIXTEENTH}, {587.33, SIXTEENTH}, {698.46, SIXTEENTH},

  // ---------- MEASURE 12 ----------
  {349.23, SIXTEENTH}, {440.00, SIXTEENTH}, {523.25, SIXTEENTH}, {659.26, SIXTEENTH},
  {349.23, SIXTEENTH}, {440.00, SIXTEENTH}, {523.25, SIXTEENTH}, {659.26, SIXTEENTH},

  // ---------- MEASURE 13 ----------
  {329.63, SIXTEENTH}, {392.00, SIXTEENTH}, {493.88, SIXTEENTH}, {587.33, SIXTEENTH},
  {329.63, SIXTEENTH}, {392.00, SIXTEENTH}, {493.88, SIXTEENTH}, {587.33, SIXTEENTH},

  // ---------- MEASURE 14 ----------
  {293.66, SIXTEENTH}, {349.23, SIXTEENTH}, {440.00, SIXTEENTH}, {523.25, SIXTEENTH},
  {293.66, SIXTEENTH}, {349.23, SIXTEENTH}, {440.00, SIXTEENTH}, {523.25, SIXTEENTH},

  // ---------- MEASURE 15 ----------
  {261.63, SIXTEENTH}, {329.63, SIXTEENTH}, {392.00, SIXTEENTH}, {523.25, SIXTEENTH},
  {261.63, SIXTEENTH}, {329.63, SIXTEENTH}, {392.00, SIXTEENTH}, {523.25, SIXTEENTH},

  // ---------- MEASURE 16 ----------
  {246.94, SIXTEENTH}, {329.63, SIXTEENTH}, {392.00, SIXTEENTH}, {523.25, SIXTEENTH},
  {246.94, SIXTEENTH}, {329.63, SIXTEENTH}, {392.00, SIXTEENTH}, {523.25, SIXTEENTH},

  // ---------- MEASURE 17 ----------
  {233.08, SIXTEENTH}, {311.13, SIXTEENTH}, {392.00, SIXTEENTH}, {523.25, SIXTEENTH},
  {233.08, SIXTEENTH}, {311.13, SIXTEENTH}, {392.00, SIXTEENTH}, {523.25, SIXTEENTH},

  // ---------- MEASURE 18 ----------
  {220.00, SIXTEENTH}, {293.66, SIXTEENTH}, {392.00, SIXTEENTH}, {523.25, SIXTEENTH},
  {220.00, SIXTEENTH}, {293.66, SIXTEENTH}, {392.00, SIXTEENTH}, {523.25, SIXTEENTH},

  // ---------- MEASURE 19 ----------
  {207.65, SIXTEENTH}, {293.66, SIXTEENTH}, {392.00, SIXTEENTH}, {523.25, SIXTEENTH},
  {207.65, SIXTEENTH}, {293.66, SIXTEENTH}, {392.00, SIXTEENTH}, {523.25, SIXTEENTH},

  // ---------- MEASURE 20 ----------
  {196.00, SIXTEENTH}, {261.63, SIXTEENTH}, {392.00, SIXTEENTH}, {523.25, SIXTEENTH},
  {196.00, SIXTEENTH}, {261.63, SIXTEENTH}, {392.00, SIXTEENTH}, {523.25, SIXTEENTH},

  // ---------- MEASURE 21 ----------
  {185.00, SIXTEENTH}, {261.63, SIXTEENTH}, {392.00, SIXTEENTH}, {523.25, SIXTEENTH},
  {185.00, SIXTEENTH}, {261.63, SIXTEENTH}, {392.00, SIXTEENTH}, {523.25, SIXTEENTH},

  // ---------- MEASURE 22 ----------
  {174.61, SIXTEENTH}, {261.63, SIXTEENTH}, {392.00, SIXTEENTH}, {523.25, SIXTEENTH},
  {174.61, SIXTEENTH}, {261.63, SIXTEENTH}, {392.00, SIXTEENTH}, {523.25, SIXTEENTH},
};

const uint16_t NOTE_COUNT = sizeof(prelude)/sizeof(prelude[0]);
const uint8_t PHRASE_LENGTH = 8;  // reverse after each arpeggio





// ------------------- SETUP -------------------
void setup() {
  xAxis.begin();
  xAxis.enable();
  xAxis.setStepPulseWidth(2);
  xAxis.setMoveSpeed(600.0f);

  // Home axis
  xAxis.homeBlocking(15000);

  // Move to starting position
  xAxis.moveTo(600);
}

// ------------------- LOOP -------------------
void loop() {
  static uint16_t index = 0;
  static unsigned long nextChangeTime = 0;
  static bool playing = true;
  unsigned long nowMicros = micros();

  // must be called often — runs stepper motion control state machine
  xAxis.update(nowMicros);

  if (!playing) return;

  if (millis() >= nextChangeTime) {

    // get current note
    float f = prelude[index].freq;
    unsigned int ms = prelude[index].len;

    // send frequency to stepper
    xAxis.setVelocity(f);

    // schedule next note change
    nextChangeTime = millis() + ms;

    index++;

    // If finished
    if (index >= NOTE_COUNT) {
      xAxis.setVelocity(0);
      delay(3000);    // pause before restart
      index = 0;      // restart song
    }
  }
}
