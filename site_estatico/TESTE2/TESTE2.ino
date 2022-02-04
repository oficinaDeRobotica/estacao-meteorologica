#include <WiFi.h>
#include <WebServer.h>
#include "DHT.h"

// Remova o comentário de uma das linhas abaixo para qualquer tipo de sensor DHT que você está usando!
//#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
#define DHTTYPE DHT22
#define rainDigital 18
const char* ssid = "MARATONA 2017_2G";  // Entre SSID aqui
const char* password = "96515518";  // Insira a senha aqui

WebServer server(80);

int rainDigitalVal, pressao;

// Sensor DHT
uint8_t DHTPin = 2; 
               
// Inicializa o sensor DHT.
DHT dht(DHTPin, DHTTYPE);                

float Temperature;
float Humidity;
 
void setup() {
  Serial.begin(115200);
  delay(100);

  pinMode(rainDigital,INPUT);
  pinMode(DHTPin, INPUT);

  dht.begin();              

  Serial.println("Conectando à ");
  Serial.println(ssid);

  // conecte-se à sua rede wi-fi local
  WiFi.begin(ssid, password);
  // verifique se o wi-fi está conectado à rede wi-fi
  while (WiFi.status() != WL_CONNECTED) {
     delay(1000);
     Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi conectado..!");
  Serial.print("IP obtido: ");  
  Serial.println(WiFi.localIP());

  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("Servidor HTTP iniciado");
}

void loop() {
  server.handleClient();
  Serial.println(dht.readHumidity());
}

void handle_OnConnect() {
 rainDigitalVal = digitalRead(rainDigital);
 Temperature = dht.readTemperature(); // Obtém os valores da temperatura
  Humidity = dht.readHumidity(); // Obtém os valores da umidade
  server.send(200, "text/html", SendHTML(Temperature,Humidity, rainDigitalVal)); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Não encontrado");
}

String SendHTML(float Temperaturestat,float Humiditystat, int rainDigitalVal){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta charset=˜utf-8˜/><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>Boletim meteorológico ESP32</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr +="p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<div id=\"webpage\">\n";
  ptr +="<h1>Boletim meteorológico ESP32</h1>\n";
  
  ptr +="<p>Temperatura: ";
  ptr +=(int)Temperaturestat;
  ptr +="°C</p>";
  ptr +="<p>Umidade: ";
  ptr +=(int)Humiditystat;
  ptr +="%</p>";
  ptr +="<p>Sensor chuva: ";
  ptr +=(int)rainDigitalVal;
  ptr +="°C</p>";
  
  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
