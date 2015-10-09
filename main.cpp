#include <stdio.h>

#include "../PN532_HSU/PN532_HSU.h"
#include "PN532.h"

int main()
{
  HardwareSerial serial;
  PN532_HSU hsu(serial);
  PN532 nfc(hsu);

  printf("initializing PN532...\n");
  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    printf("Didn't find PN53x board :( \n");
    return 1;
  }

}

