#pragma once

#include <stdio.h>
#include <inttypes.h>
#include <string.h>

#include <string>

class HardwareSerial {
public:
  HardwareSerial(const std::string &deviceFile);

  void begin(long baud);
  uint8_t available();
  int read();
  void flush();
  void write(uint8_t c);
  void write(const uint8_t *buffer, size_t size);

private:
  const std::string _deviceFile;
  int _fd;
};
