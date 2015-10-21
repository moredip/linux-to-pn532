
#include "PN532_TTY.h"
#include "debug.h"
#include "errno.h"

#include "arduino-serial-lib.h"

#include <termios.h>
#include <stdio.h>

#define TTY_BAUD 115200

PN532_TTY::PN532_TTY(const std::string &deviceFile)
    : _deviceFile(deviceFile), command(0)
{}

void PN532_TTY::begin()
{
    D("PN532_TTY::begin()");
    _fd = serialport_init(_deviceFile.c_str(),TTY_BAUD);
    if( _fd < 0 ){
    	printf("!!!!!! INVALID TTY FILEPATH: %s", _deviceFile.c_str() );
    	_exit(10);
    }
}


void PN532_TTY::wakeup()
{
    D("PN532_TTY::wakeup()");

    const uint8_t longPreamble[27] = 
        "\x55\x55"
        "\x00\x00\x00\x00"
        "\x00\x00\x00\x00"
        "\x00\x00\x00\x00"
        "\x00\x00\x00\x00"
        "\xFF\x03\xFD\xD4"
        "\x14\x01\x17\x00";
    
    write(longPreamble,26);

    //hack
    sleep(1);
    flushInput();
}

int8_t PN532_TTY::writeCommand(const uint8_t *header, uint8_t hlen, const uint8_t *body, uint8_t blen)
{
    D("PN532_TTY::writeCommand()");

    flushInput();

    command = header[0];
    
    write(PN532_PREAMBLE);
    write(PN532_STARTCODE1);
    write(PN532_STARTCODE2);
    
    uint8_t length = hlen + blen + 1;   // length of data field: TFI + DATA
    write(length);
    write(~length + 1);         // checksum of length
    
    write(PN532_HOSTTOPN532);
    uint8_t sum = PN532_HOSTTOPN532;    // sum of TFI + DATA

    D("\nWrite: ");
    
    write(header, hlen);
    for (uint8_t i = 0; i < hlen; i++) {
        sum += header[i];

        D_HEX(header[i]);
    }

    write(body, blen);
    for (uint8_t i = 0; i < blen; i++) {
        sum += body[i];

        D_HEX(body[i]);
    }
    
    uint8_t checksum = ~sum + 1;            // checksum of TFI + DATA
    write(checksum);
    write(PN532_POSTAMBLE);

    return readAckFrame();
}

int16_t PN532_TTY::readResponse(uint8_t buf[], uint8_t len, uint16_t timeout)
{
    uint8_t tmp[3];
    
    D("\nRead:  ");

    /** Frame Preamble and Start Code */
    if(receive(tmp, 3, timeout)<=0){
        D("Timeout 1");
        return PN532_TIMEOUT;
    }
    if(0 != tmp[0] || 0!= tmp[1] || 0xFF != tmp[2]){
        D("Preamble error");
        return PN532_INVALID_FRAME;
    }
    
    /** receive length and check */
    uint8_t length[2];
    if(receive(length, 2, timeout) <= 0){
        D("Timeout 2");
        return PN532_TIMEOUT;
    }
    if( 0 != (uint8_t)(length[0] + length[1]) ){
        D("Length error");
        return PN532_INVALID_FRAME;
    }
    length[0] -= 2;
    if( length[0] > len){
        D("No space");
        return PN532_NO_SPACE;
    }
    
    /** receive command byte */
    uint8_t cmd = command + 1;               // response command
    if(receive(tmp, 2, timeout) <= 0){
        return PN532_TIMEOUT;
    }
    if( PN532_PN532TOHOST != tmp[0] || cmd != tmp[1]){
        D("Command error");
        return PN532_INVALID_FRAME;
    }
    
    if(receive(buf, length[0], timeout) != length[0]){
        return PN532_TIMEOUT;
    }
    uint8_t sum = PN532_PN532TOHOST + cmd;
    for(uint8_t i=0; i<length[0]; i++){
        sum += buf[i];
    }
    
    /** checksum and postamble */
    if(receive(tmp, 2, timeout) <= 0){
        return PN532_TIMEOUT;
    }
    if( 0 != (uint8_t)(sum + tmp[0]) || 0 != tmp[1] ){
        D("Checksum error");
        return PN532_INVALID_FRAME;
    }
    
    return length[0];
}

int8_t PN532_TTY::readAckFrame()
{
    const uint8_t PN532_ACK[] = {0, 0, 0xFF, 0, 0xFF, 0};
    uint8_t ackBuf[sizeof(PN532_ACK)];
    
    D("\nAck: ");
    
    if( receive(ackBuf, sizeof(PN532_ACK), 900) <= 0 ){
        D("Timeout\n");
        return PN532_TIMEOUT;
    }
    
    if( memcmp(ackBuf, PN532_ACK, sizeof(PN532_ACK)) ){
        D("Invalid\n");
        return PN532_INVALID_ACK;
    }

    D("...Ack Received!\n");
    return 0;
}

/**
    @brief receive data .
    @param buf --> return value buffer.
           len --> length expect to receive.
           timeout --> time of reveiving
    @retval number of received bytes, 0 means no data received.
*/
int8_t PN532_TTY::receive(uint8_t *buf, int len, uint16_t timeout)
{
    // per http://stackoverflow.com/a/10523146
    
    // Initialize file descriptor sets
    fd_set read_fds, write_fds, except_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    FD_ZERO(&except_fds);
    FD_SET(_fd, &read_fds);

    struct timeval tv;
    tv.tv_sec = (timeout/1000);
    tv.tv_usec = (timeout%1000)*1000;

    // Wait for input to become ready or until the time out; the first parameter is
    // 1 more than the largest file descriptor in any of the sets
    int result = select(_fd + 1, &read_fds, &write_fds, &except_fds, &tv);
    if(1 == result)
    {
      return ::read( _fd, buf, len );
    }
    else
    {
      printf("select failed: result was %i, err is %i\n",result, errno);
      printf("timeout was: %i\n", timeout);
      return PN532_TIMEOUT;
    }
}


void PN532_TTY::write(uint8_t c)
{
  ::write(_fd, &c,1);
}

void PN532_TTY::write(const uint8_t *buffer, size_t size)
{
  int n = ::write(_fd,buffer,size);
  if( n != size ){
    printf("HardwareSerial::write: failed to write");
  }
}

void PN532_TTY::flushInput()
{
  ::tcflush(_fd,TCIFLUSH);
}
