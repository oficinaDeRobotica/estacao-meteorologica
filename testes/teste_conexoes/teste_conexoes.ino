#include <Wire.h>
#include <Adafruit_BMP085.h>
#include "DHT.h"
#include <SPI.h>

#define DHTTYPE DHT22

bool newInput, lastInput, newPluv, lastPluv;
unsigned long countVen, countPluv, previousTime;
const int eventInterval = 1000;
float t, h, p, hic;
uint8_t DHTPin = 13; //D7
uint8_t buttonPin = 14; //D5
uint8_t inPluv = 12; //D6

DHT dht(DHTPin, DHTTYPE); 
Adafruit_BMP085 bmp;

void setup() {
  Serial.begin(115200);

  pinMode(DHTPin, INPUT_PULLUP);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(inPluv, INPUT_PULLUP);
  dht.begin();  

  if (!bmp.begin()) {
    Serial.println("Não foi possivel achar um sensor BMP085/BMP180, checar conexões!");
    while (1) {}
  }
}

void loop() {

  t = bmp.readTemperature();
  h = dht.readHumidity();
  p = bmp.readPressure();
  hic = dht.computeHeatIndex(t, h, false);

  newInput = digitalRead(buttonPin);
  newPluv = digitalRead(inPluv);
  unsigned long currentTime = millis();

  if (newInput == LOW){
    if (newInput != lastInput) {
      countVen++;
    }
  }
  if (newPluv == LOW)
  {
    if (newPluv != lastPluv)
    {
      countPluv++;
    }
  }
  
  if (currentTime - previousTime >= eventInterval) {
  
      String test = "T= " + String(t)+ " U= " + String(h) + " P= " + String(p) + "Hic= " + String(hic) + " Cliques pluv= " + String(countPluv) + " Cliques anem= " + String(countVen);
      Serial.println(test);

    previousTime = currentTime;
  }
  
  delay(20);
  lastPluv = newPluv;
  lastInput = newInput;
}
