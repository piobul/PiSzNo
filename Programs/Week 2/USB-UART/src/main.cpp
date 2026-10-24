#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include "Display7Seg.hpp"
#include "LedBar.hpp"
#include "Uart.hpp"

int main(void) {
    Display7Seg display;
    LedBar ledBar;
    Uart uart;

    display.init();
    ledBar.init();
    uart.init(9600);

    uart.sendString("\r\n--- START LICZNIKA ATmega328PB ---\r\n");

    uint16_t counter = 0;       // Główny wartość licznika (0 - 9999)
    uint16_t led_counter = 0;   // Wartość licznika diod do zapalenia
    uint16_t timerMs = 0;       // Zmienna czasowa do odmierzenia tempa zliczania
    bool led_up = true;

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
            if (counter > 100) {
                counter = 0;

                if(led_up == true) {
                    led_counter++;
                } else {
                    led_counter--;
                }

                uart.sendString("Counted 100\r\n");
            }

            // Aktualizacja cyfr na 7-segmentach
            display.setNumber(counter);

            // Wyznaczenie liczby diod LED:
            // Każda setka dodaje 1 diodę. Po osiągnięciu 8 diod cykl się powtarza (0..8)
            ledBar.setCount(led_counter);

            if(led_counter == 8) {
                led_up = false;
            } else if(led_counter == 0) {
                led_up = true;
            }
        }
    }
}

/*#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>

void uart0_init(unsigned long baudrate) {
    uint16_t ubrr = (F_CPU / (16UL * baudrate)) - 1;
    UBRR0H = (uint8_t)(ubrr >> 8);
    UBRR0L = (uint8_t)(ubrr);
    UCSR0B = (1 << TXEN0) | (1 << RXEN0); // Włączenie TX0 i RX0 na PORTD
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8N1
}

void uart0_sendChar(char c) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = c;
}

void uart0_sendString(const char* str) {
    while (*str) uart0_sendChar(*str++);
}

int main(void) {
    uart0_init(9600);

    while (1) {
        uart0_sendString("TEST WBUDOWANEGO UART (mEDBG)\r\n");
        _delay_ms(1000);
    }
}*/