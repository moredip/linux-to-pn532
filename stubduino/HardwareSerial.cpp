#include "Arduino.h"
#include "arduino-serial-lib.h"

#include <stdio.h>


HardwareSerial::HardwareSerial(const std::string &deviceFile)
  : _deviceFile(deviceFile)
{
}

void HardwareSerial::begin(long baud)
{
  _fd = serialport_init(_deviceFile.c_str(),baud);
}

// BADLY WRONG implementation of available(). This will eat bytes and throw them away.
uint8_t HardwareSerial::available(){
  //printf("HardwareSerial::available()\n");

  int buffer;
  int n = ::read(_fd,&buffer,1);
  if( n == -1 || n == 0 ){
    return 0;
  }else{
    return 1;
  }
}

int HardwareSerial::read()
{
  printf("r");

  int buffer;
  int n = ::read(_fd,&buffer,1);

  if( n == -1 || n == 0 ){
    return -1;
  }else{
    return buffer;
  }
}

void HardwareSerial::flush()
{
  //printf("HardwareSerial::flush()\n");
  serialport_flush(_fd);
}

void HardwareSerial::write(uint8_t c)
{
  //printf("HardwareSerial::write()\n");

  int n = ::write(_fd,&c,1);
  if( n != 1 ){
    printf("HardwareSerial::write: failed to write");
  }
}

void HardwareSerial::write(const uint8_t *buffer, size_t size)
{
}
