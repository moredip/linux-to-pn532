#ifndef STUBDUINO_ARDUINOSERIAL_H_
#define STUBDUINO_ARDUINOSERIAL_H_

#include "Print.h"

class ArduinoSerial : public Print {
public:
	ArduinoSerial();
	virtual ~ArduinoSerial();

	virtual size_t write(uint8_t);
};

extern ArduinoSerial Serial;

#endif /* STUBDUINO_ARDUINOSERIAL_H_ */
