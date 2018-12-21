#ifndef _GFX_
#define _GFX_
#include "Init.h"

//Eventhough colors are defined as 32 bits, the last two 4 bits are not sent, 
//they are there just for the format sake
#define RED     0xFF000000  //0xF800  
#define ORANGE  0xFF9E0000  //0xFC00
#define YELLOW  0xFFFC0000  //0xFFE0
#define GREEN   0x00FB0C00  //0x07E0
#define CYAN    0x00FFF600  //0x07FF
#define BLUE    0x007BFF00  //0x001F
#define MAGENTA 0xF600FF00  //111101100000111111//0xF81F
#define BLACK   0x00000000
#define WHITE   0xFFFFFF00

#define CASET   0x2A
#define RASET   0x2B
#define RAMWR   0x2C
#define RAMRD   0x2E

//GFX is a subclass of Init
class GFX: public Init{
 public:

GFX(int32_t mosi, int32_t sck, int32_t cs, int32_t rst):Init(mosi, sck, cs, rst){}
void beginLCD(){beginLC();}
void fillScreen(uint32_t color);
void setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
  

//Write pixel color RGB = 24 bit to a pixel (should have add set already)
void writePixelColor(uint32_t color);

//Write 32 bit data for the caset and raset
void write32(uint32_t para);


 private:

};
#endif
