/*
#include <SPI.h>
#define BME_SCK 18
#define BME_MISO 19
#define BME_MOSI 23
#define BME_CS 5
*/

//pro upload via wifi
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

//pro bme
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#define SEALEVELPRESSURE_HPA (1013.25)

//wifi pra requisição e pro upload via wifi
#ifndef STASSID
#define STASSID "apezin"
#define STAPSK  "05082003"
#endif
const char* ssid = STASSID;
const char* password = STAPSK;

//bibliotecas e pinos diferentes pra cada placa
#ifdef ESP32
  #include <WiFi.h>
  #include <HTTPClient.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
  #include <ESP8266mDNS.h>
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
const char* serverName = "http://estacao-db.herokuapp.com/xxxpost-data.php";

String apiKeyValue = "tPmAT5Ab3j7F9";
String sensorName = "BME280";
String sensorLocation = "LABCI";

unsigned long lastTime = 0;
unsigned long timerDelay = 60000;

bool newInput, lastInput, pluvInputAnt, pluvInputAp = 1;
bool a = 0;
float raio = 0.0008;
int countVen, countPluv, rpm, vel, pluv, previousTime;
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
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("conexao ao wifi falhou! reiniciando...");
    delay(5000);
    ESP.restart();
  }

  // (you can also pass in a Wire library object like &Wire2)
  bool status = bme.begin(0x76);
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring or change I2C address!");
    while (1);
  }

  /* tudo do ota
  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");
  */

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  Serial.println("Pronto");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  ArduinoOTA.handle();
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
      String httpRequestData = "api_key=" + apiKeyValue + "&sensor=" + sensorName+ "&location=" + sensorLocation + "&temp=" + String(bme.readTemperature())+ "&humi=" + String(bme.readHumidity()) + "&press=" + String(bme.readPressure()/100.0F) + "&rain=" + String(pluv) + "&wind=" + String(vel);
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
   
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}
