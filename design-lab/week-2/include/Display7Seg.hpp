#ifndef DISPLAY_7SEG_HPP
#define DISPLAY_7SEG_HPP

#include <avr/io.h>

class Display7Seg {
private:
    uint8_t digits[4];
    uint8_t currentDigit;
    static const uint8_t segmentMap[10];

public:
    void init();
    void setNumber(uint16_t value);
    void update(); // Wywoływana w pętli do przełączania cyfr (multipleksowanie)
};

#endif