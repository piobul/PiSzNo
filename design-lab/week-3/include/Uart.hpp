#pragma once
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdlib.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

class Uart {
public:
    void init(uint32_t baud = 9600) {
        uint16_t ubrr = (F_CPU / (16UL * baud)) - 1;
        UBRR0H = (uint8_t)(ubrr >> 8);
        UBRR0L = (uint8_t)(ubrr);
        UCSR0B = (1 << RXEN0) | (1 << TXEN0);
        UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
    }

    void putChar(char c) {
        while (!(UCSR0A & (1 << UDRE0)));
        UDR0 = c;
    }

    void print(const char* str) {
        while (*str) putChar(*str++);
    }

    void println(const char* str = "") {
        print(str);
        print_P(PSTR("\r\n"));
    }

    // Czyta napis prosto z Flasha (zero zużycia RAM!)
    void print_P(const char* progmemStr) {
        char c;
        while ((c = pgm_read_byte(progmemStr++))) {
            putChar(c);
        }
    }

    void println_P(const char* progmemStr) {
        print_P(progmemStr);
        print_P(PSTR("\r\n"));
    }

    void printInt(int32_t val) {
        char buf[12];
        ltoa(val, buf, 10);
        print(buf);
    }

    void printFloat(float val, uint8_t decimals = 1) {
        char buf[16];
        dtostrf(val, 1, decimals, buf);
        print(buf);
    }
};