#include "Init.h"
//Init::Init(){}
Init::Init(int32_t mosi, int32_t sck, int32_t cs, int32_t rst, int32_t dc){
  _mosi = mosi;
  _sck = sck;
  _cs = cs;
  _rst = rst;
  _dc = dc;
}

void Init::beginLC(){
pinMode(13, OUTPUT);
digitalWrite(13, HIGH);
digitalWrite(_cs, HIGH);
pinMode(_cs, OUTPUT);
pinMode(_sck, OUTPUT);
pinMode(_mosi, OUTPUT);
pinMode(_rst, OUTPUT);
pinMode(_dc, OUTPUT);

reset_pulse();
soft_reset();
uint32_t ID = readwrite8(0x04, 24, 1);
Serial.print("read reg(4):  ID = 0x");
Serial.println(ID, HEX);

read_7735();

chipEnable();
command(0x11); //Sleep Out, pg 182
chipDisable();
delay(120); //Delay 120ms, required

chipEnable();
command(0x36); //MADCTL
param(0x00);// normal orientation & RGB
chipToggle();

command(0x3A);  
param(0x06); //default// #06 //rgb interface selection = 18 bit, pg 106
//param(0x66); //rgb interface selection = 18 bit, pg 106
//param(0x55); //rgb interface selection = 16 bit, pg 106
//param(0x05); //rgb interface selection = 16 bit, pg 106
chipToggle();

//Shr added
//command(0xB0); //RAM control, see pg 256
//param(0x00); // Ram access from MCU
//param(0xF0);//CHange to (FF)

command(0x13);// Normal display mode on
chipToggle();

command(0x29);// Display On
chipToggle();

command(0xB2);  //Porch setting, pg 261; kept default
param(0x0C);  
param(0x0C);  
param(0x00);  
param(0x33);  
param(0x33);
chipToggle();

command(0xBB);  //VCOM Setting
param(0x1A); //1A = 0.75
chipToggle();

command(0xC0);  //LCM Control, pg 274
param(0x2C);//default, I guess its for parallel displays
chipToggle();

command(0xC2);  //VDV and VRH Command Enable
param(0x01); //=value comes from command write, if = 02, value comes from NVM
chipToggle();

command(0xC3); //VRH Set, pg 277, no idea
param(0x0F);// = 
chipToggle();

command(0xC4);  //VDV Set
param(0x20); //= -0.75
chipToggle();

command(0xC6);  //frame rate
param(0x0F); // 60Hz
chipToggle();

command(0xD0);  //Power Control 1**
param(0xA4);  //default, always needs to be there
param(0xA1);  //
chipToggle();

//ST7789S gamma setting
command(0xE0);  
param(0xD0);  
param(0x13);  
param(0x1A);  
param(0x0A);  
param(0x0A);  
param(0x26);  
param(0x3F);  
param(0x54);  
param(0x54);  
param(0x18);  
param(0x14);  
param(0x14);  
param(0x30);  
param(0x33);
chipToggle();

command(0xE1);  
param(0xD0);  
param(0x13);  
param(0x1A);  
param(0x0A);  
param(0x0A);  
param(0x26);  
param(0x3F);  
param(0x54);  
param(0x54);  
param(0x1A);  
param(0x16);  
param(0x16);  
param(0x32);  
param(0x35);
chipToggle();

command(0x29);//Display On
chipDisable();
delay(120); //ms

chipEnable();
//command(0x21);  //display inversion ON
command(0x20); // display inversion OFF
chipToggle();

command(0x2a); //Column Address Set//Caset
param(0x00);
param(0x00); //0
param(0x00);
param(0xEF); //239
chipToggle();

command(0x2b); //Row Address Set //Raset
param(0x00);
param(0x00); //0
param(0x00);
param(0xEF); //239
chipToggle();

command(0x2C);
for(int i = 0; i < 57600; i++){
  param(0xFF);
  param(0x9E);
  param(0x00);
  }
chipDisable(); 
//command(0x2C);
  //command(0x2C);
  //read_7735();

  Serial.println("DOne");
  
}

void Init:: command(uint8_t val){
    //pinMode(_cs, LOW);//addded now
    pinMode(_mosi, OUTPUT);  digitalWrite(_dc, 0);
        digitalWrite(_mosi, 0);  
        digitalWrite(_sck, HIGH);
        digitalWrite(_sck, LOW);
    
    for (int i = 0; i < 8; i++) {   //send command
        digitalWrite(_mosi, (val & 0x80) != 0);
        digitalWrite(_sck, HIGH);
        digitalWrite(_sck, LOW);
        val <<= 1;
    }
    //pinMode(_cs, HIGH);//addded now
}

void Init::chipEnable(){
  digitalWrite(_cs, LOW);
  }

void Init::chipDisable(){
  digitalWrite(_cs, HIGH);
  }
void Init::chipToggle(){
  digitalWrite(_cs, HIGH);
  delay(1);
  digitalWrite(_cs, LOW);
  
  }
void Init:: param(uint8_t val){
 //pinMode(_cs, LOW);//addded now
    pinMode(_mosi, OUTPUT);  digitalWrite(_dc, 1);
        digitalWrite(_mosi, 1);  
        digitalWrite(_sck, HIGH);
        digitalWrite(_sck, LOW);
    
    for (int i = 0; i < 8; i++) {   //send command
        digitalWrite(_mosi, (val & 0x80) != 0);
        digitalWrite(_sck, HIGH);
        digitalWrite(_sck, LOW);
        val <<= 1;
    }
    //pinMode(_cs, HIGH);//addded now  
  }

void Init::reset_pulse(void)
{
    digitalWrite(_rst, HIGH);
    digitalWrite(_rst, LOW);   //Hardware Reset
    delay(10);
    digitalWrite(_rst, HIGH);
    delay(10);
}

void Init::soft_reset(void)
{
    writeblock(0x01, NULL, 0);   //software reset
    delay(120);
}

void Init::writeblock(uint8_t cmd, uint8_t *block, int8_t N)
{
    uint8_t val = cmd;
    chipEnable();
    command(cmd);
    while (N-- > 0) param(*block++);
    chipDisable();
}

uint32_t Init::readwrite8(uint8_t cmd, uint8_t bits, uint8_t dummy)
{
    chipEnable();
    command(cmd);
    uint32_t ret = read8(bits, dummy);
    chipDisable();
    return ret;
}

uint32_t Init::read8(uint8_t bits, uint8_t dummy)
{
    uint32_t ret = 0;
    pinMode(_mosi, INPUT_PULLUP);  digitalWrite(_dc, 0);

    for (int i = 0; i < dummy; i++) {  //any dummy clocks
        digitalWrite(_sck, HIGH);
        delay(1);
        digitalWrite(_sck, LOW);
        delay(1);
    }
    for (int i = 0; i < bits; i++) {  // read results
        ret <<= 1;
        delay(1);
        if (digitalRead(_mosi)) ret |= 1;;
        digitalWrite(_sck, HIGH);
        delay(1);
        digitalWrite(_sck, LOW);
    }
    return ret;
}


void Init::read_7735()
{
    Serial.println("data sheet specific calls");
    show7735(0x04, 3, 1);  //RDDID
    show7735(0x09, 4, 1);  //RDDSTATUS
    show7735(0x0A, 1, 0);
    show7735(0x0B, 1, 0);   //RDDMADCTL
    show7735(0x0C, 1, 0);   //RDDCOLMOD
    show7735(0x0D, 1, 0);
    show7735(0x0E, 1, 0);
    show7735(0x0F, 1, 0);
    show7735(0xDA, 1, 0);   //RDID1
    show7735(0xDB, 1, 0);   //RDID2
    show7735(0xDC, 1, 0);   //RDID3
}

void Init::show7735(uint8_t reg, uint8_t bytes, uint8_t dummy)
{
    uint32_t val, mask = 0x10000000;
    uint8_t bits = bytes * 8;
    for (uint8_t wid = 32; wid > bits; wid -= 4) mask >>= 4;
    //    reset_pulse();    //ST7789V not happy with this
    soft_reset();
    val = readwrite8(reg, bits, dummy);
    Serial.print(" reg(0x");
    if (reg < 0x10) Serial.print("0");
    Serial.print(reg , HEX);
    Serial.print(") = 0x");
    while (val < mask) Serial.print("0"), mask >>= 4;
    if (val) Serial.println(val, HEX);
    else Serial.println();
}

