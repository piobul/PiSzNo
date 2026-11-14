#include "Display7Seg.hpp"

// Kody segmentów A-G dla cyfr 0-9
const uint8_t Display7Seg::segmentMap[10] = {
    0b11000000, // 0 (A,B,C,D,E,F)
    0b11111001, // 1 (B,C)
    0b10100100, // 2 (A,B,D,E,G)
    0b10110000, // 3 (A,B,C,D,G)
    0b10011001, // 4 (B,C,F,G)
    0b10010010, // 5 (A,C,D,F,G)
    0b10000010, // 6 (A,C,D,E,F,G)
    0b11111000, // 7 (A,B,C)
    0b10000000, // 8 (A,B,C,D,E,F,G)
    0b10010000  // 9 (A,B,C,D,F,G)
};

void Display7Seg::init() {
    DDRE |= 0x0F; // PE0 - PE3 jako wyjścia (Cyfry D1 - D4)
    DDRC |= 0x01; // PC0 jako wyjście (Segment A)
    DDRB |= 0x3F; // PB0 - PB5 jako wyjścia (Segmenty B - G)

    PORTE |= 0x0F;  // Wyłączenie cyfr D1-D4 (Stan HIGH wyłącza cyfrę)
    PORTC &= ~0x01; // Wyłączenie segmentu A
    PORTB &= ~0x3F; // Wyłączenie segmentów B-G

    currentDigit = 0;
    setNumber(0);
}

void Display7Seg::setNumber(uint16_t value) {
    if (value > 9999) value = 9999;

    digits[0] = value % 10;
    digits[1] = (value / 10) % 10;
    digits[2] = (value / 100) % 10;
    digits[3] = (value / 1000) % 10;
}

void Display7Seg::update() {
    // 1. Wyłączenie WSZYSTKICH cyfr na PORTE (Stan HIGH = OFF dla cyfry)
    PORTE |= 0x0F;

    // 2. Pobranie maski segmentów dla aktualnej cyfry (0b0GFEDCBA)
    uint8_t mask = segmentMap[digits[currentDigit]];

    // --- Segment A na PC0 ---
    if (mask & (1 << 0)) {
        PORTC |= (1 << PORTC0);  // Zapal segment A
    } else {
        PORTC &= ~(1 << PORTC0); // Zgaś segment A
    }

    // --- Segmenty B-G na PB0..PB5 ---
    uint8_t bgSegments = (mask >> 1) & 0x3F;
    PORTB = (PORTB & ~0x3F) | bgSegments;

    // 3. Włączenie TYLKO aktywnej cyfry (Stan LOW = ON dla cyfry)
    PORTE &= ~(1 << (3 - currentDigit));

    // 4. Przejście do kolejnej cyfry
    currentDigit = (currentDigit + 1) % 4;
}