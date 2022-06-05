bool newInput, lastInput, pluvInputAnt, pluvInputAp = 1;
unsigned long countVen,countPluv, rpm, vel, multi, pluv;
const unsigned long eventInterval = 1000;
unsigned long previousTime = 0;
const int buttonPin = 4;
const int inPluv = 7;
float raio = 0.0008;


void setup() {

  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, HIGH);
  pinMode(inPluv, INPUT);
  digitalWrite(inPluv, HIGH);
}

void loop() {
newInput = digitalRead(buttonPin);
pluvInputAnt = digitalRead(inPluv);
unsigned long currentTime = millis();

  if (newInput == LOW){
    if (newInput != lastInput) {
      countVen++;
    }
  }
  if (pluvInputAnt == LOW)
  {
    if (pluvInputAnt != pluvInputAp)
    {
      countPluv++;
      pluv = countPluv*50;
    }
  }
    if (currentTime - previousTime >= eventInterval) {
    /* Event code */
    countVen = countVen * 20;
    multi = 120*3.14*countVen;
    vel = multi*raio;
    Serial.print(pluv);
    Serial.print("mm");
    Serial.print("   ");
    Serial.print(countVen);
    Serial.print(" rpm ");
    Serial.print(vel);
    Serial.println("km/h");
    countVen = 0;
    /* Update the timing for the next time around */
    previousTime = currentTime;
  }
  
  delay(20);
  pluvInputAp = pluvInputAnt;
  lastInput = newInput;
}
