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
    DDRC |= 0x3F; // PC0 - PC5 jako wyjścia (Segmenty A - F)
    DDRB |= 0x1F; // PB0 - PB3 (Cyfry D1-D4) oraz PB4 (Segment G) jako wyjścia
    
    PORTC &= ~0x3F; // Wyłączenie segmentów A-F
    PORTB |= 0x0F;  // Wyłączenie cyfr D1-D4 (Stan HIGH = wyłączona)
    PORTB &= ~(1 << PORTB4); // Wyłączenie segmentu G
    
    currentDigit = 0;
    setNumber(0);
}

void Display7Seg::setNumber(uint16_t value) {
    if (value > 9999) value = 9999;
    
    digits[0] = (value / 1000) % 10;
    digits[1] = (value / 100) % 10;
    digits[2] = (value / 10) % 10;
    digits[3] = value % 10;
}

void Display7Seg::update() {
    // 1. Wyłączenie WSZYSTKICH cyfr D1-D4 (Stan HIGH wyłącza cyfrę)
    PORTB |= 0x0F;

    // 2. Pobranie wzoru segmentów dla aktualnej cyfry
    uint8_t mask = segmentMap[digits[currentDigit]];

    // Ustawienie segmentów A-F (PC0..PC5) - Stan HIGH zapala segment
    PORTC = (PORTC & ~0x3F) | (mask & 0x3F);

    // Ustawienie segmentu G na PB4
    if (mask & (1 << 6)) {
        PORTB |= (1 << PORTB4);  // Zapal segment G
    } else {
        PORTB &= ~(1 << PORTB4); // Zgaś segment G
    }

    // 3. Włączenie TYLKO aktywnej cyfry (Stan LOW włącza wybraną cyfrę)
    PORTB &= ~(1 << currentDigit);

    // 4. Przejście do kolejnej cyfry
    currentDigit = (currentDigit + 1) % 4;
}