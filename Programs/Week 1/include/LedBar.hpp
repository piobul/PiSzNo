#ifndef LED_BAR_HPP
#define LED_BAR_HPP

#include <avr/io.h>

class LedBar {
public:
    void init();
    void setCount(uint8_t count); // Zapala od 0 do 8 diod
    void clear();
};

#endif