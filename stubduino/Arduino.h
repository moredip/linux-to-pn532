#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "HardwareSerial.h"
#include "ArduinoString.h"
#include <sys/time.h>
#include "ArduinoSerial.h"

#define F(x) (x)

inline unsigned long millis(){
  timeval tv;
  gettimeofday(&tv, NULL);
  return (tv.tv_sec*1000) + (tv.tv_usec/1000);
}

typedef uint8_t byte;

typedef bool boolean;
