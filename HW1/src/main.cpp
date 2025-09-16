#include <Arduino.h>


const int Load     = 13;  
const int Insert   = 12;  
const int Detonate = 11;   
const int Place    = 10; 

const int ledLoad     = 18;   
const int ledMaterial = 17;   
const int ledDetonate = 16;  
const int ledReady    = 15;





bool isLoading = false;
bool isLoaded = false;
bool materialInserted = false;
bool isDetonating = false;
bool readyToPlace = false;
bool ledBlinkState = false;
unsigned long loadingStartTime = 0;
unsigned long detonationStartTime = 0;
unsigned long lastBlinkTime = 0;

void setup() {
  Serial.begin(115200);
  pinMode(Load, INPUT);
  pinMode(Insert, INPUT);
  pinMode(Detonate, INPUT);
  pinMode(Place, INPUT);
  pinMode(ledLoad, OUTPUT);
  pinMode(ledMaterial, OUTPUT);
  pinMode(ledDetonate, OUTPUT);
  pinMode(ledReady, OUTPUT);


  analogWrite(ledLoad, 0);
  analogWrite(ledMaterial, 0);
  analogWrite(ledDetonate, 0);
  analogWrite(ledReady, 0);
}

void loop() {
  unsigned long currentMillis = millis();

  // Load button pressed
  if (digitalRead(Load) == HIGH) {
    delay(100);
    if (!isLoading && !isLoaded) {
      isLoading = true;
      loadingStartTime = currentMillis;
      analogWrite(ledLoad, 255);
      Serial.println("Loading started...");
    } else if (isLoading) {
      isLoading = false;
      analogWrite(ledLoad, 0);
      Serial.println("Loading cancelled.");
    }
  }

  if (isLoading && (currentMillis - loadingStartTime >= 3000)) {
    isLoading = false;
    isLoaded = true;
    analogWrite(ledLoad, 0);
    Serial.println("Device loaded.");
  }


  if (digitalRead(Insert) == HIGH) {
    delay(100);
    if (isLoaded && !materialInserted) {
      materialInserted = true;
      analogWrite(ledLoad, 0);
      analogWrite(ledMaterial, 255);
      analogWrite(ledDetonate, 0);
      analogWrite(ledReady, 0);
      Serial.println("Material inserted.");
    }
  }





  if (digitalRead(Detonate) == HIGH) {
    delay(100);
    if (materialInserted && !isDetonating) {
      isDetonating = true;
      detonationStartTime = currentMillis;
      analogWrite(ledLoad, 0);
      analogWrite(ledMaterial, 0);
      analogWrite(ledDetonate, 0);
      analogWrite(ledReady, 0);
      Serial.println("Detonation started...");
    }
  }




  if (isDetonating) {
    if (currentMillis - detonationStartTime >= 10000) {
      isDetonating = false;
      readyToPlace = true;
      analogWrite(ledDetonate, 0);
      analogWrite(ledReady, 255);
      Serial.println("Device ready to load.");
    } else if (currentMillis - lastBlinkTime >= 1000) {
      lastBlinkTime = currentMillis;
      ledBlinkState = !ledBlinkState;
      analogWrite(ledDetonate, ledBlinkState ? 255 : 0);
    }
  }






  
  if (digitalRead(Place) == HIGH) {
    delay(100);
    if (readyToPlace) {
      isLoading = false;
      isLoaded = false;
      materialInserted = false;
      isDetonating = false;
      readyToPlace = false;
      ledBlinkState = false;

      analogWrite(ledLoad, 0);
      analogWrite(ledMaterial, 0);
      analogWrite(ledDetonate, 0);
      analogWrite(ledReady, 0);

      Serial.println("Device placed. System reset.");
    }
  }

  delay(10);
}
