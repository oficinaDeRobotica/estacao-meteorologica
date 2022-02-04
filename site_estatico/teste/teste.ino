#include <Arduino.h>
#include <DHT.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <WebSocketsServer.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Adafruit_BMP085.h>

Adafruit_BMP085 bmp;

String serverHost = "https://www.oficina-robotica.tech/scoket.io";
String valores;
WiFiMulti    WiFiMulti;
WebServer    server(80);
WebSocketsServer    webSocket = WebSocketsServer(81);

// Declaracao dos pinos
const int chuva_REED = 6;
int barometro_pino_d = 0; 
int barometro_pino_a = 2;

// Valores de retorno dos sensores
int valor_Umidade;
int valor_Temperatura;
int valor_Barometrico;
int valor_Chuva;

// Iniciar os sensores
DHT dht(4, DHT22);

// Variaveis utilizadas pelos sensores
int barometro_val_d = 0;
int barometro_val_a = 0; 
int old_val, val = 0;
int chuva_val = 0;       
int chuva_old_val = 0;   
int chuva_REEDCOUNT = 0;
int chuva_old_value, chuva_value;

char html_template[] PROGMEM = R"=====(
<html lang="en">
  <head>
      <meta charset="utf-8">
      <meta name="viewport" content="width=device-width, initial-scale=1">
      <title>webserver control</title>
    <script>
      var connection = new WebSocket('ws://'+serverHost+':81/', ['arduino']);connection.onopen = function () {  connection.send('Connect ' + new Date()); };
       
      connection.onerror = function (error) {    console.log('WebSocket Error ', error);};
      connection.onmessage = function (e) {  
            console.log('Server: ', e.data);
            console.log("[socket] " + e.data);
        };
    </script>
  </head>
  <body style="max-width:400px;margin: auto;font-family:Arial, Helvetica, sans-serif;text-align:center">
   </body>
</html>
)=====";


void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
      
    case WStype_CONNECTED: {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        // send message to client
        webSocket.sendTXT(num, "0");
      }
      break;
  }

}

void setup() {
  
  Serial.begin(115200);
  delay(1000);
  if (!bmp.begin()) {
  Serial.println("Could not find a valid BMP085/BMP180 sensor, check wiring!");
  while (1) {}
  }
  // adicionar NOME da rede e PASSWORD
  WiFiMulti.addAP("MARATONA 2017_2G", "96515518");
  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print("nao conectado");
    delay(100);
  }
  /* modo de cada pino */
  pinMode(barometro_pino_d, INPUT);
  pinMode(barometro_pino_a, INPUT);
  pinMode (chuva_REED, INPUT_PULLUP); //This activates the internal pull up resistor
  dht.begin();
  Serial.println(WiFi.localIP());

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  server.on("/", handleMain);
  server.onNotFound(handleNotFound);
  server.begin();

}

void handleMain() {
  server.send_P(200, "text/html", html_template ); 
}
void handleNotFound() {
  server.send(404,   "text/html", "<html><body><p>404 Error</p></body></html>" );
}

void loop() {
  webSocket.loop();
  server.handleClient();
  chuva_val = digitalRead(chuva_REED);
   if ((chuva_val == LOW) && (chuva_old_val == HIGH)){ 
       delay(10);                   
       chuva_REEDCOUNT = chuva_REEDCOUNT + 1;   
       chuva_old_val = chuva_val;              
   } 
   else {
      old_val = val;             
   }
   
  //char[] valorUmidade = (String)valor_Umidade = dht.readHumidity();
  //char[] valorTemperatura = (String)valor_Temperatura = dht.readTemperature(); 
  //char[] valorBarometrico = (String)barometro_val_a = analogRead(barometro_pino_a);
  //char[] valorChuva = (String)valor_Chuva = REEDCOUNT*0.25;

Serial.println(dht.readTemperature());
  
 // char[] valores = "Umidade:"+valorUmidade+";Temperatura:"+valorTemperatura+";Barometro"+valorBarometrico+";Chuva"+valorChuva+";";
  //webSocket.broadcastTXT(valores);
  delay(50);
}
