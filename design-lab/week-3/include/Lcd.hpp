#pragma once
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

class Lcd {
private:
    void pulseEnable() {
        PORTB &= ~(1 << PB0);
        _delay_us(1);
        PORTB |= (1 << PB0);
        _delay_us(1);
        PORTB &= ~(1 << PB0);
        _delay_us(50);
    }

    void writeNibble(uint8_t nibble) {
        // Mapowanie: bit0 -> PD7 (D4), bit1 -> PD6 (D5), bit2 -> PD5 (D6), bit3 -> PD4 (D7)
        PORTD &= ~((1 << PD4) | (1 << PD5) | (1 << PD6) | (1 << PD7));
        if (nibble & 0x01) PORTD |= (1 << PD7);
        if (nibble & 0x02) PORTD |= (1 << PD6);
        if (nibble & 0x04) PORTD |= (1 << PD5);
        if (nibble & 0x08) PORTD |= (1 << PD4);
        pulseEnable();
    }

    void sendByte(uint8_t val, bool isData) {
        if (isData) PORTB |= (1 << PB1);  // RS = 1 (dane)
        else        PORTB &= ~(1 << PB1); // RS = 0 (komenda)

        writeNibble(val >> 4);
        writeNibble(val & 0x0F);
    }

public:
    void init() {
        DDRB |= (1 << PB0) | (1 << PB1);
        DDRD |= (1 << PD4) | (1 << PD5) | (1 << PD6) | (1 << PD7);

        _delay_ms(50);
        PORTB &= ~(1 << PB1);

        // Sekwencja inicjalizacji 4-bitowej HD44780
        writeNibble(0x03); _delay_ms(5);
        writeNibble(0x03); _delay_us(150);
        writeNibble(0x03); _delay_us(150);
        writeNibble(0x02); _delay_us(150);

        command(0x28); // 4-bit, 2 linie, czcionka 5x8
        command(0x0C); // Ekran włączony, kursor wyłączony
        command(0x06); // Auto-inkrementacja kursora
        clear();
    }

    void command(uint8_t cmd) {
        sendByte(cmd, false);
        if (cmd == 0x01 || cmd == 0x02) _delay_ms(2);
    }

    void clear() {
        command(0x01);
    }

    void setCursor(uint8_t col, uint8_t row) {
        uint8_t offset = (row == 0) ? 0x00 : 0x40;
        command(0x80 | (col + offset));
    }

    void putChar(char c) {
        sendByte((uint8_t)c, true);
    }

    void print(const char* str) {
        while (*str) putChar(*str++);
    }

    void printFloat(float val, uint8_t decimals = 1) {
        char buf[16];
        dtostrf(val, 1, decimals, buf);
        print(buf);
    }

    void printInt(int32_t val) {
        char buf[12];
        ltoa(val, buf, 10);
        print(buf);
    }
};