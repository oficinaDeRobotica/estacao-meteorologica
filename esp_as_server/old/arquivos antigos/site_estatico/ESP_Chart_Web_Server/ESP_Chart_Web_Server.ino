/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include "DHT.h"

#define DHTTYPE DHT22
#define rainDigital 18

// Replace with your network credentials
const char* ssid = "MARATONA 2017_2G";
const char* password = "96515518";
int rainDigitalVal;
int Temperature, Humidity;
// Sensor DHT
uint8_t DHTPin = 2; 

DHT dht(DHTPin, DHTTYPE); 

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String readdhtHumidity() {
  n = dht.readHumidity();
  string = String(n);
  return string;
}

String readdhtTemperature() {
  return dht.readTemperature();
}

String readraindigital() {
  return digitalRead(rainDigital);
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

  pinMode(rainDigital,INPUT);
  pinMode(DHTPin, INPUT);

  dht.begin();  

  // Initialize SPIFFS
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html");
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readdhtTemperature().c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readdhtHumidity().c_str());
  });

  //server.on("/chuva", HTTP_GET, [](AsyncWebServerRequest *request){
  //  request->send_P(200, "text/plain", readraindigital().c_str());
  //});

  // Start server
  server.begin();
}
 
void loop(){
  
}
