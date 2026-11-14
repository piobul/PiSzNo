#ifndef UART_HPP
#define UART_HPP

#include <avr/io.h>
#include <stdlib.h>

class Uart {
public:
    void init(unsigned long baudrate = 9600);
    void sendChar(char c);
    void sendString(const char* str);
    void sendNumber(uint16_t num);
};

#endif