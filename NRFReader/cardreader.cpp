#include <stdio.h>

#include "PN532_TTY.h"
#include "PN532.h"

void scanForCard(PN532 &nfc){

  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
    
  printf("looking for a ISO14443A card...\n");
  // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  if (success) {
    printf("Found an ISO14443A card\n");
    printf("UID Length: %i bytes\n",uidLength);
    printf("UID Value: \n");
    for (int i = 0; i < uidLength; i ++) {
        printf(" %02x", uid[i]);
    }
    printf("\n");
  }else{
    printf("Couldn't find a card\n");
  }
}

int main(int argc, char *argv[])
{
  if (argc==1) {
    printf("please specify a device file\n");
    return 10;
  }

  PN532_TTY tty(argv[1]);
  PN532 nfc(tty);

  printf("initialising PN532...\n");
  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    printf("Didn't find PN53x board :( \n");
    return 1;
  }
  printf("Found chip PN5%x\n",(versiondata>>24) & 0xFF);
  printf("Firmware ver. %i.%i\n",(versiondata>>16) & 0xFF,(versiondata>>8) & 0xFF);
  printf("Supports %x\n",versiondata & 0xFF);

  // configure board to read RFID tags
  nfc.SAMConfig();

  while(1){
    scanForCard(nfc);
    sleep(1);
  }

}

