#include <SPI.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include "DHT.h"
#include <Wire.h>
#include <Adafruit_BMP085.h>

#define DHTTYPE DHT22
#define rainDigital 19

const char* ssid = "wifi";
const char* password = "12341234";
float temperature, humidity, pressure, altitude;
uint8_t DHTPin = 2; 
Adafruit_BMP085 bmp;


DHT dht(DHTPin, DHTTYPE); 

AsyncWebServer server(80);

String readdhtHumidity(){
  humidity = dht.readHumidity();
  if (round(humidity)){
    return String(round(humidity));
  }
  else{
    return String("00");
  }
}

String readdhtTemperature() {
  temperature = dht.readTemperature();
  if (round(temperature)){
    return String(round(temperature));
  }
  else{
    return String("00");
  }
}

String readBmpPressure() {
  pressure = bmp.readPressure();
  if (round(pressure)){
    return String(round(pressure));
  }
  else{
    return String("00");
  }
}

String readBmpAltitude() {
  altitude = bmp.readAltitude(102000);
    if (round(altitude)){
    return String(round(altitude));
  }
  else{
    return String("00");
  }
}

String readraindigital() {
    return String(digitalRead(rainDigital));  
}

void setup(){

  Serial.begin(115200);

  pinMode(rainDigital,INPUT);
  pinMode(DHTPin, INPUT);

  dht.begin();  

  if (!bmp.begin()) {
    Serial.println("Não foi possivel achar um sensor BMP085/BMP180, checar conexões!");
    while (1) {}
  }

  if(!SPIFFS.begin()){
    Serial.println("Não foi possível montar o SPIFFS");
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
  server.on("/logo", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/logo.png", "image/png");
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readdhtTemperature().c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readdhtHumidity().c_str());
  });
  server.on("/chuva", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send_P(200, "text/plain", readraindigital().c_str());
  });
  server.on("/pressure", HTTP_GET, [](AsyncWebServerRequest *request){
     request->send_P(200, "text/plain", readBmpPressure().c_str());
   });
   server.on("/altitude", HTTP_GET, [](AsyncWebServerRequest *request){
     request->send_P(200, "text/plain", readBmpAltitude().c_str());
   });
  // server.on("/pressure_sea", HTTP_GET, [](AsyncWebServerRequest *request){
  // request->send_P(200, "text/plain", readBmpSeaLevelPressure().c_str());
  // });

  // server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
  //   request->send_P(200, "text/plain", "30");
  // });
  // server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
  //   request->send_P(200, "text/plain",  "30");
  // });
  // server.on("/chuva", HTTP_GET, [](AsyncWebServerRequest *request){
  //   request->send_P(200, "text/plain", "30");
  // });

  // Start server
  server.begin();
}
 
void loop(){

}