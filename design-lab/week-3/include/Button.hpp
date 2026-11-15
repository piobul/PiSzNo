#pragma once
#include <avr/io.h>
#include "Timer.hpp"

class Button {
private:
    bool lastReading = true;
    bool stableState = true;
    uint32_t lastDebounceTime = 0;

public:
    void init() {
        DDRB  &= ~(1 << PB7); // PB7 (przycisk SW0 na płytce) jako wejście
        PORTB |= (1 << PB7);  // Wewnętrzny rezystor pull-up
    }

    bool wasPressed() {
        bool reading = (PINB & (1 << PB7)) != 0;
        bool triggered = false;

        if (reading != lastReading) {
            lastDebounceTime = Timer::millis();
        }

        if ((Timer::millis() - lastDebounceTime) > 50) {
            if (reading != stableState) {
                stableState = reading;
                if (!stableState) { // Stan niski = SW0 wciśnięty do GND
                    triggered = true;
                }
            }
        }

        lastReading = reading;
        return triggered;
    }
};