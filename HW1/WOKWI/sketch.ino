const int Load = 4;       
const int Insert = 5;    
const int Detonate = 6;  
const int Place = 7;   
const int ledLoad = 15;         
const int ledMaterial = 16;    
const int ledDetonate = 17;     
const int ledReady = 18;  

bool isLoading = false;
bool isLoaded = false;
bool materialInserted = false;
bool isDetonating = false;
bool readyToPlace = false;





unsigned long loadingStartTime = 0;
unsigned long detonationStartTime = 0;
unsigned long lastBlinkTime = 0;
bool ledBlinkState = false;


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

  digitalWrite(ledLoad, LOW);
  digitalWrite(ledMaterial, LOW);
  digitalWrite(ledDetonate, LOW);
  digitalWrite(ledReady, LOW);
}







void loop() {
  unsigned long currentMillis = millis();

  if (digitalRead(Load) == HIGH) {
    delay(100); 
    if (isLoading == false && isLoaded == false) {
      isLoading = true;
      loadingStartTime = currentMillis;
      digitalWrite(ledLoad, HIGH);
      Serial.println("Loading started...");
    } else {
      if (isLoading == true) {
        isLoading = false;
        digitalWrite(ledLoad, LOW);
        Serial.println("Loading cancelled.");
      }
    }
  }


  if (isLoading == true) {
    if (currentMillis - loadingStartTime >= 3000) {
      isLoading = false;
      isLoaded = true;
      digitalWrite(ledLoad, LOW);
      Serial.println("Device loaded.");
    }
  }










  if (digitalRead(Insert) == HIGH) {
    delay(100);
    if (isLoaded == true && materialInserted == false) {
      materialInserted = true;
      digitalWrite(ledLoad, LOW);
      digitalWrite(ledMaterial, LOW);
      digitalWrite(ledDetonate, LOW);
      digitalWrite(ledReady, LOW);
      digitalWrite(ledMaterial, HIGH);
      Serial.println("Material inserted.");
    }
  }







  if (digitalRead(Detonate) == HIGH) {
    delay(100);
    if (materialInserted == true && isDetonating == false) {
      isDetonating = true;
      detonationStartTime = currentMillis;
      digitalWrite(ledLoad, LOW);
      digitalWrite(ledMaterial, LOW);
      digitalWrite(ledDetonate, LOW);
      digitalWrite(ledReady, LOW);
      Serial.println("Detonation started...");
    }
  }


  if (isDetonating == true) {
    if (currentMillis - detonationStartTime >= 10000) {
      isDetonating = false;
      readyToPlace = true;
      digitalWrite(ledDetonate, LOW);
      digitalWrite(ledReady, HIGH);
      Serial.println("Device ready to load.");
    } 
    else {
      if (currentMillis - lastBlinkTime >= 1000) {
        lastBlinkTime = currentMillis;
        if (ledBlinkState == false) {
          ledBlinkState = true;
          digitalWrite(ledDetonate, HIGH);
        } else {
          ledBlinkState = false;
          digitalWrite(ledDetonate, LOW);
        }
      }
    }
  }








  if (digitalRead(Place) == HIGH) {
    delay(100); 
    if (readyToPlace == true) {
      isLoading = false;
      isLoaded = false;
      materialInserted = false;
      isDetonating = false;
      readyToPlace = false;
      ledBlinkState = false;
      isLoading = false;
      isLoaded = false;
      materialInserted = false;
      isDetonating = false;
      readyToPlace = false;
      ledBlinkState = false;
      
      digitalWrite(ledLoad, LOW);
      digitalWrite(ledMaterial, LOW);
      digitalWrite(ledDetonate, LOW);
      digitalWrite(ledReady, LOW);
      Serial.println("Device placed. System reset.");
    }
  }

  delay(10);
}
