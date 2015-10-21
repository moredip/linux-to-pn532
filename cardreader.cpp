#include <stdio.h>
#include "NfcAdapter.h"

#include "PN532_TTY.h"
#include "PN532.h"


int main(int argc, char *argv[])
{
  if (argc==1) {
    printf("please specify a device file\n");
    return 10;
  }

  PN532_TTY tty(argv[1]);

  NfcAdapter adapter(tty);

  printf("initialising card reader...\n");

  adapter.begin();

  printf("...initialised\n");

  while(1){
	  if( adapter.tagPresent(1000) ){
		  adapter.read().print();
	  }
  }

}

