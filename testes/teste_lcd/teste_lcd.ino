#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
 
#define BLpin 2   //Pino que controla a luz de fundo
 
// CLK, DIN, DC, CE, RST
Adafruit_PCD8544 display = Adafruit_PCD8544(8,9,10,11,12);
 
PROGMEM const unsigned char img[] = 
{
 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x07, 0xff, 0xff, 0xff, 0xff, 
  0xf0, 0xff, 0xff, 0xff, 0xff, 0xfb, 0xfd, 0xff, 0xff, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 0xff, 
  0xe7, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 0xff, 0x9f, 0xff, 0x9f, 0xff, 0xff, 
  0xff, 0xf0, 0xff, 0xff, 0xff, 0xff, 0x7f, 0xff, 0xe7, 0xff, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 
  0xfc, 0xff, 0xff, 0xf1, 0xff, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 0xf3, 0xff, 0xff, 0xfc, 0xff, 
  0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 0xcf, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xf0, 0xff, 0xff, 
  0xff, 0xbf, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 0x7f, 0xff, 0xff, 0xff, 
  0xef, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xf7, 0xff, 0xff, 0xf0, 0xff, 
  0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xfd, 0xff, 0xff, 0xff, 
  0xff, 0xfb, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xfd, 0xff, 0x3f, 0xff, 0xff, 0xfb, 0xff, 0xff, 0xf0, 
  0xff, 0xff, 0xfd, 0xf8, 0x03, 0xf8, 0x00, 0x3f, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 0xf0, 0x01, 
  0xf8, 0x00, 0x3f, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 0xe1, 0xf0, 0xf8, 0xfe, 0x3f, 0xff, 0xff, 
  0xf0, 0xff, 0xff, 0xff, 0xc7, 0xf8, 0x78, 0xfc, 0x3f, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 0xc7, 
  0xfc, 0x78, 0xfc, 0x7f, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 0x8f, 0xfe, 0x38, 0xf8, 0x7f, 0xff, 
  0xff, 0xf0, 0xff, 0xff, 0xff, 0x8f, 0xfe, 0x38, 0xf0, 0xff, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 
  0x8f, 0xfe, 0x38, 0xe1, 0xff, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 0x8f, 0xfe, 0x38, 0xe3, 0xff, 
  0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 0x8f, 0xfe, 0x38, 0xe3, 0xff, 0xff, 0xff, 0xf0, 0xff, 0xff, 
  0xff, 0x8f, 0xfe, 0x78, 0xf1, 0xff, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 0xc7, 0xfc, 0x78, 0xf8, 
  0xff, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 0xc3, 0xf8, 0xf8, 0xf8, 0x7f, 0xff, 0xff, 0xf0, 0xff, 
  0xff, 0xff, 0xe1, 0xf0, 0xf8, 0xfc, 0x7f, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 0xf0, 0x01, 0xf8, 
  0xfe, 0x3f, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xfd, 0xf8, 0x03, 0xf8, 0xff, 0x1f, 0xff, 0xff, 0xf0, 
  0xff, 0xff, 0xfd, 0xff, 0xff, 0xff, 0xff, 0xfb, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xfd, 0xff, 0xff, 
  0xff, 0xff, 0xfb, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xf0, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xf7, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 0x7f, 
  0xff, 0xff, 0xff, 0xef, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 0xbf, 0xff, 0xff, 0xff, 0xdf, 0xff, 
  0xff, 0xf0, 0xff, 0xff, 0xff, 0xcf, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 
  0xf3, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xf9, 0xff, 
  0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xe7, 0xff, 0xff, 0xff, 0xf0, 0xff, 0xff, 
  0xff, 0xff, 0x9f, 0xff, 0x9f, 0xff, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xe7, 0xff, 0x3f, 
  0xff, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xfb, 0xfd, 0xff, 0xff, 0xff, 0xff, 0xf0, 0xff, 
  0xff, 0xff, 0xff, 0xfe, 0x07, 0xff, 0xff, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0
};

  
void setup() 
{
  pinMode(BLpin, OUTPUT);
  digitalWrite(BLpin, LOW); //backlight
  display.begin();          //Inicializa o Display
  display.setContrast(56);  //Define o contraste
}
 
void loop() 
{
  display.clearDisplay();                 //Limpa a tela
  display.drawBitmap(0, 0, img, 84, 48, BLACK);    //Exibe imagem Bitmap
  display.display();                      //Exibe no display
  delay(5000);
  display.clearDisplay();
  display.setTextSize(1);                 //Tamanho da fonte
  display.setTextColor(BLACK);            //Texto Preto
  //display.setTextColor(WHITE, BLACK);     //Texto Branco com fundo Preto
  display.setCursor(0,0);                 //Posição do Cursor
  display.print("Oficina de robotica");                 //Escreve o Texto
  display.setCursor(0,20);
  display.setTextSize(1);
  display.print("Display Nokia 5110");
 
  display.display();
 
  //Pisca a luz de fundo
  //static bool backLight = LOW;
  //backLight = !backLight;
  //digitalWrite(BLpin, backLight);
   
  delay(5000);
}
