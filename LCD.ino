#include "OTA_ESP32.h" //server.handleClient();
#include "Init.h" 
#include "GFX.h"
#if defined(ESP32)
#define TFT_MOSI  23
#define TFT_SCK   18
#define TFT_SS    5
#define TFT_RESET 4   
#endif

GFX tft = GFX(TFT_MOSI, TFT_SCK, TFT_SS, TFT_RESET);

void setup() {
    Serial.begin(9600);
    intializeOTA();
    tft.beginLCD();
    tft.fillScreen(MAGENTA);
    
}

void loop(){
  server.handleClient();
  delay(1);   
  yield();
}
