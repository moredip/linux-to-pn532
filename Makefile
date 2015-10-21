INCDIRS = -Istubduino -Igit_submodules/PN532/PN532 -Igit_submodules/PN532/NDEF -Igit_submodules/arduino-serial
SOURCE_FILES = cardreader.cpp \
	stubduino/*.cpp \
	PN532_TTY.cpp \
	git_submodules/PN532/PN532/PN532.cpp \
	git_submodules/PN532/NDEF/NfcAdapter.cpp \
	git_submodules/PN532/NDEF/NfcTag.cpp \
	git_submodules/PN532/NDEF/Ndef.cpp \
	git_submodules/PN532/NDEF/NdefMessage.cpp \
	git_submodules/PN532/NDEF/NdefRecord.cpp \
	git_submodules/PN532/NDEF/MifareClassic.cpp \
	git_submodules/PN532/NDEF/MifareUltraLight.cpp \
	git_submodules/arduino-serial/arduino-serial-lib.c

default: cardreader

all: cardreader

cardreader: cardreader.cpp
#	g++ -g -o cardreader ${INCDIRS} ${SOURCE_FILES}
	g++ -g ${INCDIRS} ${SOURCE_FILES} -o cardreader

clean:
	-rm -f cardreader
