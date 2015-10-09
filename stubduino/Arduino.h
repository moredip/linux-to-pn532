#pragma once

#include "HardwareSerial.h"

#include <sys/time.h>

inline unsigned long millis(){
  timeval tv;
  gettimeofday(&tv, NULL);
  return (tv.tv_sec*1000) + (tv.tv_usec/1000);
}
