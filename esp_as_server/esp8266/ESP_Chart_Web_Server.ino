#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include "DHT.h"

#define DHTTYPE DHT22
#define rainDigital 18

const char* ssid = "apezin";
const char* password = "05082003";
float rainDigitalVal, temperature, humidity;
uint8_t DHTPin = 2; 

DHT dht(DHTPin, DHTTYPE); 

AsyncWebServer server(80);

String readdhtHumidity() {
  humidity = dht.readHumidity();
  return String(humidity);
}

String readdhtTemperature() {
  temperature = dht.readTemperature();
  return String(temperature);
}

String readraindigital() {
  
  rainDigitalVal = digitalRead(rainDigital);
  return String(rainDigitalVal);
}

void setup(){

  Serial.begin(115200);

  pinMode(rainDigital,INPUT);
  pinMode(DHTPin, INPUT);

  dht.begin();  

  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });
  server.on("/highcharts.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/highcharts.js", "text/javascript");
  });
//  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
//    request->send_P(200, "text/plain", readdhtTemperature().c_str());
//  });
//  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
//    request->send_P(200, "text/plain", readdhtHumidity().c_str());
//  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", "30");
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain",  "30");
  });

  //server.on("/chuva", HTTP_GET, [](AsyncWebServerRequest *request){
  //  request->send_P(200, "text/plain", readraindigital().c_str());
  //});

  // Start server
  server.begin();
}
 
void loop(){
  
}
