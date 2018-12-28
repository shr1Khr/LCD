#include "GFX.h"
#include "Init.h"

void GFX::fillScreen(uint32_t color){
setAddrWindow(0,0, 240, 240);
writePixelColor(0xFFFFFFFF);//White
Serial.println("Done");
  }


//Last 8 bits are never pushed
void GFX::writePixelColor(uint32_t color){
  uint8_t R = color >> 16;
  uint8_t G = color >> 8;
  //uint8_t G = color >> 16;
  uint8_t B = color;
  //uint8_t B = color >> 8;
  param(0xFF);
  param(0xFF);
  param(0xFF);
  command(RAMWR);
  }

void GFX::write32(uint32_t para){
  uint8_t R = para >> 24;
  uint8_t G = para >> 16;
  uint8_t B = para >> 8;
  uint8_t e = para; //& 0xFF;
  param(R);
  param(G);
  param(B);
  param(e);
  }

//x,y = start point of frame (16 bit each)
//w, h = width and lenght of frame (16 bit each)
void GFX::setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
 /* //x += _xstart;
  //y += _ystart;
  uint32_t xa = ((uint32_t)x << 16) | (x+w-1);
  uint32_t ya = ((uint32_t)y << 16) | (y+h-1); 

  command(CASET); // Column addr set
  write32(xa);

  command(RASET); // Row addr set
  write32(ya);

  command(RAMWR); // write to RAM
*/

command(CASET);
param(0x00);
param(0x00); //0
param(0x00);
param(0xEF); //239

command(RASET);
param(0x00);
param(0x00); //0
param(0x00);
param(0xEF); //239
command(RAMWR);
}

