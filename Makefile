INCDIRS = -Istubduino -Igit_submodules/PN532/PN532 -Igit_submodules/arduino-serial
SOURCE_FILES = cardreader.cpp stubduino/*.cpp PN532_TTY.cpp git_submodules/PN532/PN532/PN532.cpp git_submodules/arduino-serial/arduino-serial-lib.c

default: cardreader

cardreader: cardreader.cpp
	g++ -o cardreader ${INCDIRS} ${SOURCE_FILES}

clean:
		-rm -f cardreader
