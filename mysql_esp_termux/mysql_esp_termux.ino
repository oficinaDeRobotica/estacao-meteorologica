/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/visualize-esp32-esp8266-sensor-readings-from-anywhere/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

*/

#include <Wire.h>
#include <Adafruit_BMP085.h>
#include "DHT.h"
#include <SPI.h>

#define DHTTYPE DHT22
#ifdef ESP32
  #include <WiFi.h>
  #include <HTTPClient.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
  #include <WiFiClient.h>
#endif

float temperature, humidity, pressure, hic;
uint8_t DHTPin = 2; 

DHT dht(DHTPin, DHTTYPE); 
Adafruit_BMP085 bmp;


// Replace with your network credentials
const char* ssid     = "Wi-fi 2G";
const char* password = "05082003";

// REPLACE with your Domain name and URL path or IP address with path
const char* serverName = "http://192.168.1.3:8080/post-data.php";

// Keep this API Key value to be compatible with the PHP code provided in the project page. 
// If you change the apiKeyValue value, the PHP file /post-data.php also needs to have the same key 
String apiKeyValue = "tPmAT5Ab3j7F9";

String readDhtHumidity(){
  humidity = dht.readHumidity();
  if (round(humidity)){
    return String(round(humidity));
  }
  else{
    return String("00");
  }
}

String readBmpTemperature() {
  temperature = bmp.readTemperature();
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

String calcHic() {
  hic = dht.computeHeatIndex(temperature, humidity, false);
}

void setup() {
  Serial.begin(115200);

  pinMode(DHTPin, INPUT);

  dht.begin();  
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  if (!bmp.begin()) {
    Serial.println("Não foi possivel achar um sensor BMP085/BMP180, checar conexões!");
    while (1) {}
  }
}

void loop() {

  temperature = bmp.readTemperature();
  humidity = dht.readHumidity();
  pressure = bmp.readPressure();
  hic = dht.computeHeatIndex(temperature, humidity, false);
  
  //Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){
    WiFiClient client;
    HTTPClient http;
    
    // Your Domain name with URL path or IP address with path
    http.begin(client, serverName);
    
    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    // Prepare your HTTP POST request data
    /*
    String httpRequestData = "api_key=" + apiKeyValue + "&value1=" + String(bme.readTemperature())
                           + "&value2=" + String(bme.readHumidity()) + "&value3=" + String(bme.readPressure()/100.0F) + "";
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);
    */

 String httpRequestData = "api_key=" + apiKeyValue + "&value1=" + String(temperature)
                           + "&value2=" + String(humidity) + "&value3=" + String(pressure) + "&value4=" + String(hic);
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);
    
/*
    String httpRequestData = "api_key=" + apiKeyValue + "&value1=" + String(30)
                           + "&value2=" + String(30) + "&value3=" + String(30) + "&value4=" + String(30);
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);
    
    // You can comment the httpRequestData variable above
    // then, use the httpRequestData variable below (for testing purposes without the BME280 sensor)
    //String httpRequestData = "api_key=tPmAT5Ab3j7F9&value1=24.75&value2=49.54&value3=1005.14";
     */



     
    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);
     
    // If you need an HTTP request with a content type: text/plain
    //http.addHeader("Content-Type", "text/plain");
    //int httpResponseCode = http.POST("Hello, World!");
    
    // If you need an HTTP request with a content type: application/json, use the following:
    //http.addHeader("Content-Type", "application/json");
    //int httpResponseCode = http.POST("{\"value1\":\"19\",\"value2\":\"67\",\"value3\":\"78\"}");
    
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
  //Send an HTTP POST request every 60 seconds
  delay(60000);  
}
