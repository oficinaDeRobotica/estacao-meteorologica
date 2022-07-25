/*
#include <SPI.h>
#define BME_SCK 18
#define BME_MISO 19
#define BME_MOSI 23
#define BME_CS 5
*/
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#define SEALEVELPRESSURE_HPA (1013.25)

#ifdef ESP32
  #include <WiFi.h>
  #include <HTTPClient.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
  #include <WiFiClient.h>
  #define buttonPin 12 //D6
  #define inPluv 14   //D5
#endif

Adafruit_BME280 bme;  // I2C

/*
 * outras opcoes

Adafruit_BME280 bme(BME_CS);  // hardware SPI
Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK);  // software SPI

*/

const char* ssid = "wifi";
const char* password = "12341234";
const char* serverName = "http://estacao-db.herokuapp.com/post-data.php";

String apiKeyValue = "tPmAT5Ab3j7F9";
String sensorName = "BME280";
String sensorLocation = "LABCI";

unsigned long lastTime = 0;
unsigned long timerDelay = 60000;

bool newInput, lastInput, pluvInputAnt, pluvInputAp = 1;
bool a = 0;
float raio = 0.0008;
int volt, bat, countVen, countPluv, rpm, vel, pluv, previousTime;
float temperature, humidity, pressure;

void getReadings(bool a){

  newInput = digitalRead(buttonPin);
  pluvInputAnt = digitalRead(inPluv);

  if (newInput == LOW){
    if (newInput != lastInput) {
      countVen++;
    }
  }
  if (pluvInputAnt == LOW){
    if (pluvInputAnt != pluvInputAp){
      countPluv++;
    }
  }

  pluvInputAp = pluvInputAnt;
  lastInput = newInput;

  if (a){
    volt = analogRead(A0);
    bat = map(volt, 365, 651, 0, 100);
    Serial.println(bat);
    Serial.println(countVen);
    temperature = bme.readTemperature();
    humidity = bme.readHumidity();
    pressure = bme.readPressure();

    pluv = countPluv;
    //pluv = countPluv*0.254;
    rpm = countVen/3;
    
    vel = 40 * 3.14 * countVen * raio;
    countVen = 0;
    countPluv = 0;
    a = false;
  }
  
  delay(20);
}

void setup() {
  Serial.begin(115200);

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(inPluv, INPUT_PULLUP);

  WiFi.begin(ssid, password);
  Serial.println("Conectando");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // (you can also pass in a Wire library object like &Wire2)
  bool status = bme.begin(0x76);
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring or change I2C address!");
    while (1);
  }
  
  Serial.println("Conectado no Wifi com endereco IP: ");
  Serial.println(WiFi.localIP());

}

void loop() {

  getReadings(false);
  
  //Send an HTTP POST request every 60 seconds
  if ((millis() - lastTime) > timerDelay) {
    Serial.println("enviando");

    getReadings(true);

    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;

      // Your Domain name with URL path or IP address with path
      http.begin(client, serverName);

      // Specify content-type header
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");

      // Prepare your HTTP POST request data
      String httpRequestData = "api_key=" + apiKeyValue + "&sensor=" + sensorName+ "&location=" + sensorLocation + "&temp=" + String(bme.readTemperature())+ "&humi=" + String(bme.readHumidity()) + "&press=" + String(bme.readPressure()/100.0F) + "&rain=" + String(pluv) + "&wind=" + String(vel) + "&bat=" + String(bat);
      Serial.print("httpRequestData: ");
      Serial.println(httpRequestData);

      /*
      // You can comment the httpRequestData variable above
      // then, use the httpRequestData variable below (for testing purposes without the BME280 sensor)
      String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=BME280&location=Escola&temp=30.00&humi=49.54&press=1005.14&rain=0&wind=8";

      // Send HTTP POST request
      int httpResponseCode = http.POST(httpRequestData);
      
      //Serial.println(httpRequestData);

     
       If you need an HTTP request with a content type: text/plain
      http.addHeader("Content-Type", "text/plain");
      int httpResponseCode = http.POST("Hello, World!");

       If you need an HTTP request with a content type: application/json, use the following:
      http.addHeader("Content-Type", "application/json");
      int httpResponseCode = http.POST("{\"value1\":\"19\",\"value2\":\"67\",\"value3\":\"78\"}");

      */
      int httpResponseCode = http.POST(httpRequestData);
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
    lastTime = millis();
  }
}
