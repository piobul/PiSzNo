#pragma once
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

struct SensorData {
    float tempAir;
    uint8_t humidity;
    float tempProbe;
    bool dhtOk;
    bool probeOk;
};

class Sensors {
private:
    // --- PROTOKÓŁ 1-WIRE DLA DS18B20 (PC0) ---
    // Fragment do podmiany w include/Sensors.hpp (wszędzie PC2 zamiast PC0):
    bool oneWireReset() {
        DDRC  |= (1 << PC2);
        PORTC &= ~(1 << PC2);
        _delay_us(480);

        DDRC  &= ~(1 << PC2);
        PORTC |= (1 << PC2);
        _delay_us(70);

        bool presence = !(PINC & (1 << PC2));
        _delay_us(410);
        return presence;
    }

    void oneWireWriteBit(uint8_t bit) {
        cli();
        DDRC  |= (1 << PC2);
        PORTC &= ~(1 << PC2);
        if (bit) {
            _delay_us(6);
            DDRC  &= ~(1 << PC2);
            PORTC |= (1 << PC2);
            sei();
            _delay_us(64);
        } else {
            _delay_us(60);
            DDRC  &= ~(1 << PC2);
            PORTC |= (1 << PC2);
            sei();
            _delay_us(10);
        }
    }

    uint8_t oneWireReadBit() {
        uint8_t bit = 0;
        cli();
        DDRC  |= (1 << PC2);
        PORTC &= ~(1 << PC2);
        _delay_us(6);

        DDRC  &= ~(1 << PC2);
        PORTC |= (1 << PC2);
        _delay_us(9);

        if (PINC & (1 << PC2)) bit = 1;
        sei();
        _delay_us(55);
        return bit;
    }

    void oneWireWriteByte(uint8_t data) {
        for (uint8_t i = 0; i < 8; i++) {
            oneWireWriteBit(data & 0x01);
            data >>= 1;
        }
    }

    uint8_t oneWireReadByte() {
        uint8_t data = 0;
        for (uint8_t i = 0; i < 8; i++) {
            data >>= 1;
            if (oneWireReadBit()) data |= 0x80;
        }
        return data;
    }

    bool readDS18B20(float& tempOut) {
        if (!oneWireReset()) return false;
        oneWireWriteByte(0xCC); // Skip ROM (jeden czujnik na linii)
        oneWireWriteByte(0x44); // Convert T

        // Czekamy na koniec konwersji (lub max 750 ms)
        for (uint16_t i = 0; i < 750; i++) {
            _delay_ms(1);
            if (oneWireReadBit()) break;
        }

        if (!oneWireReset()) return false;
        oneWireWriteByte(0xCC); // Skip ROM
        oneWireWriteByte(0xBE); // Read Scratchpad

        uint8_t lsb = oneWireReadByte();
        uint8_t msb = oneWireReadByte();
        int16_t raw = (msb << 8) | lsb;

        tempOut = (float)raw * 0.0625f;
        return (tempOut > -55.0f && tempOut < 125.0f);
    }

    // --- PROTOKÓŁ DHT11 (PD3) ---
    bool readDHT11(float& tempOut, uint8_t& humOut) {
        uint8_t bytes[5] = {0};

        // Sygnał startu z mikrokontrolera (min. 18 ms stanu niskiego)
        DDRD  |= (1 << PD3);
        PORTD &= ~(1 << PD3);
        _delay_ms(20);

        PORTD |= (1 << PD3);
        _delay_us(30);
        DDRD  &= ~(1 << PD3); // Przełączenie na wejście
        PORTD |= (1 << PD3);  // Pull-up

        cli(); // Krytyczny czasowo odczyt 40 bitów

        // Oczekiwanie na odpowiedź DHT11 (80us LOW -> 80us HIGH)
        uint16_t timeout = 0;
        while (PIND & (1 << PD3))  { if (++timeout > 1000) { sei(); return false; } _delay_us(1); }
        timeout = 0;
        while (!(PIND & (1 << PD3))) { if (++timeout > 1000) { sei(); return false; } _delay_us(1); }
        timeout = 0;
        while (PIND & (1 << PD3))  { if (++timeout > 1000) { sei(); return false; } _delay_us(1); }

        // Odczyt 40 bitów danych
        for (uint8_t i = 0; i < 40; i++) {
            timeout = 0;
            while (!(PIND & (1 << PD3))) { if (++timeout > 1000) { sei(); return false; } _delay_us(1); }

            _delay_us(35); // Po 35 us: stan 0 = bit '0', stan 1 = bit '1'
            if (PIND & (1 << PD3)) {
                bytes[i / 8] |= (1 << (7 - (i % 8)));
            }

            timeout = 0;
            while (PIND & (1 << PD3)) { if (++timeout > 1000) { sei(); return false; } _delay_us(1); }
        }

        sei();

        // Weryfikacja sumy kontrolnej
        if ((uint8_t)(bytes[0] + bytes[1] + bytes[2] + bytes[3]) != bytes[4]) {
            return false;
        }

        humOut  = bytes[0];
        tempOut = (float)bytes[2] + (bytes[3] * 0.1f);
        return true;
    }

public:
    void init() {
        DDRC  &= ~(1 << PC0);
        PORTC |= (1 << PC0); // Pull-up dla DS18B20
        DDRD  &= ~(1 << PD3);
        PORTD |= (1 << PD3); // Pull-up dla DHT11
    }

    SensorData readAll() {
        SensorData data;
        data.dhtOk   = readDHT11(data.tempAir, data.humidity);
        data.probeOk = readDS18B20(data.tempProbe);
        return data;
    }
};