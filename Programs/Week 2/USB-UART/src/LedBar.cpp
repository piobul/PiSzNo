#include "LedBar.hpp"

void LedBar::init() {
    DDRD |= 0xFC; // Piny PD2-PD7 jako wyjścia (bity 2..7)
    DDRC |= 0x06; // Piny PC1-PC2 jako wyjścia (bity 1..2)
    clear();
}

void LedBar::setCount(uint8_t count) {
    clear();
    if (count > 8) count = 8;

    // Zapalenie odpowiedniej liczby diod (0 do 8)
    for (uint8_t i = 0; i < count; i++) {
        if (i == 0) {
            PORTC |= (1 << PORTC1); // Dioda 1 -> PC1
        } else if (i == 1) {
            PORTC |= (1 << PORTC2); // Dioda 2 -> PC2
        } else {
            PORTD |= (1 << i);      // Diody 3..8 -> PD2..PD7 (i=2 to PD2, i=7 to PD7)
        }
    }
}

void LedBar::clear() {
    PORTD &= ~0xFC; // Zgaszenie diod na PORTD
    PORTC &= ~0x06; // Zgaszenie diod na PORTC
}