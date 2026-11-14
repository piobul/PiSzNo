#include "LedBar.hpp"

void LedBar::init() {
    DDRD = 0xFF; // Wszystkie piny PORTD jako wyjścia (PD0 - PD7)
    clear();
}

void LedBar::setCount(uint8_t count) {
    if (count > 8) count = 8;
    
    if (count == 0) {
        PORTD = 0x00;
    } else {
        // Tworzy maskę bitową, np. dla count=3: (1<<3)-1 = 0b00000111 (zapala PD0, PD1, PD2)
        PORTD = (1 << count) - 1;
    }
}

void LedBar::clear() {
    PORTD = 0x00;
}