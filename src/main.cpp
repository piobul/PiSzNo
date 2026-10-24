#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include "Display7Seg.hpp"
#include "LedBar.hpp"

int main(void) {
    Display7Seg display;
    LedBar ledBar;

    display.init();
    ledBar.init();

    uint16_t counter = 0;   // Główny wartość licznika (0 - 9999)
    uint16_t timerMs = 0;   // Zmienna czasowa do odmierzenia tempa zliczania

    while (1) {
        // 1. Odświeżenie jednej cyfry wyświetlacza (multipleksowanie co 3 ms)
        display.update();
        _delay_ms(3);
        timerMs += 3;

        // 2. Co 50 ms zwiększamy licznik (zmień tę wartość, aby przyspieszyć/spowolnić)
        if (timerMs >= 50) {
            timerMs = 0;
            counter++;

            // Nieskończona pętla: po 9999 wracamy do 0
            if (counter > 9999) {
                counter = 0;
            }

            // Aktualizacja cyfr na 7-segmentach
            display.setNumber(counter);

            // Wyznaczenie liczby diod LED:
            // Każda setka dodaje 1 diodę. Po osiągnięciu 8 diod cykl się powtarza (0..8)
            uint8_t ledsToLight = (counter / 100) % 9;
            ledBar.setCount(ledsToLight);
        }
    }
}