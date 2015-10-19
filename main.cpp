#include <stdio.h>

#include "PN532_TTY.h"
#include "PN532.h"

int main(int argc, char *argv[])
{
  if (argc==1) {
    printf("please specify a device file");
    return 10;
  }

  PN532_TTY tty(argv[1]);
  PN532 nfc(tty);

  printf("initializing PN532...\n");
  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    printf("Didn't find PN53x board :( \n");
    return 1;
  }

  printf("version information: %u\n", versiondata);

}

