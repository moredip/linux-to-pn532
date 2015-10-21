#pragma once

#include "PN532Interface.h"

#include <string>

#define PN532_TTY_READ_TIMEOUT (900)

class PN532_TTY : public PN532Interface {
public:
    PN532_TTY(const std::string &deviceFile);
    
    void begin();
    void wakeup();
    virtual int8_t writeCommand(const uint8_t *header, uint8_t hlen, const uint8_t *body = 0, uint8_t blen = 0);
    int16_t readResponse(uint8_t buf[], uint8_t len, uint16_t timeout);
    
private:
    const std::string _deviceFile;
    int _fd;

    uint8_t command;
    
    int8_t readAckFrame();
    
    int8_t receive(uint8_t *buf, int len, uint16_t timeout=PN532_TTY_READ_TIMEOUT);
    void write(uint8_t c);
    void write(const uint8_t *buffer, size_t size);
    void flushInput();
};
