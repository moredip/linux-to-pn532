#pragma once

#include <stdio.h>
#include <inttypes.h>
#include <string.h>

class HardwareSerial {
public:
  void begin(long);
  uint8_t available(void);
  int read(void);
  void flush(void);
  void write(uint8_t);
  void write(const uint8_t *buffer, size_t size);
};

inline unsigned long millis(){
  return 0;
}
