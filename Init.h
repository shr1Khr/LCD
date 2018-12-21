#ifndef _init_
#define _init_

#if ARDUINO >= 100
 #include "Arduino.h"
 #include "Print.h"
#else
 #include "WProgram.h"
#endif

class Init{
 public:

 Init(int32_t mosi, int32_t sck, int32_t cs, int32_t rst); // Constructor
  void chipEnable(); //_cs = low = enable the LCD
  void chipDisable();//_cs = high = disable the LCD
  void reset_pulse();// Reset pin toggle
  void soft_reset(); // Software reset
  void beginLC();//Initialize LCD, Pin status and read IDs
  void command(uint8_t val); // 8 bit command, with DC = 0
  void param(uint8_t val); // 8 bit Parameter, with DC = 1
  void read_7735(); // Read standard buffers
  uint32_t read8(uint8_t bits, uint8_t dummy); // read 8 bit on MOSI
  uint32_t readwrite8(uint8_t cmd, uint8_t bits, uint8_t dummy);// (doesnt print) all cmd are command; bits = read length; dummy = dummy clock in read 
  void show7735(uint8_t reg, uint8_t bytes, uint8_t dummy); //prints: same as above, except in bytes
  void writeblock(uint8_t cmd, uint8_t *block, int8_t N);  

 private:
int32_t _mosi, _sck, _cs, _rst;


};
#endif
