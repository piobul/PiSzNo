#ifndef F_CPU
#define F_CPU 16000000UL // Zegar 16 MHz płytki Xplained Mini
#endif

#include <avr/io.h>
#include <util/delay.h>

// Klasa reprezentująca diodę LED podłączoną w trybie Active-LOW
class Led {
private:
    volatile uint8_t* ddrReg;
    volatile uint8_t* portReg;
    uint8_t pinBit;

public:
    Led(volatile uint8_t* ddr, volatile uint8_t* port, uint8_t bit)
        : ddrReg(ddr), portReg(port), pinBit(bit) {
        *ddrReg |= (1 << pinBit); // Ustawienie pinu jako wyjście
        off();                     // Domyślnie wyłączona
    }

    void on() { *portReg |= (1 << pinBit); }   // Stan LOW = świeci
    void off() { *portReg &= ~(1 << pinBit); }   // Stan HIGH = zgaszona
    void toggle() { *portReg ^= (1 << pinBit); } // Przełączenie stanu
};

// Klasa reprezentująca przycisk z włączonym podciąganiem (Pull-up)
class Button {
private:
    volatile uint8_t* ddrReg;
    volatile uint8_t* portReg;
    volatile uint8_t* pinReg;
    uint8_t pinBit;

public:
    Button(volatile uint8_t* ddr, volatile uint8_t* port, volatile uint8_t* pin, uint8_t bit)
        : ddrReg(ddr), portReg(port), pinReg(pin), pinBit(bit) {
        *ddrReg &= ~(1 << pinBit); // Ustawienie pinu jako wejście
        *portReg |= (1 << pinBit);  // Włączenie wewnętrznego rezystora Pull-up
    }

    bool isPressed() const {
        // Po naciśnięciu przycisk zwiera pin do masy (stan LOW)
        return !(*pinReg & (1 << pinBit));
    }
};

int main(void) {
    // Inicjalizacja obiektów sprzętowych (LED0 = PB5, SW0 = PB7)
    Led statusLed(&DDRB, &PORTB, PORTB5);
    Button userButton(&DDRB, &PORTB, &PINB, PINB7);

    while (1) {
        if (userButton.isPressed()) {
            statusLed.on(); // Dioda świeci, gdy trzymasz przycisk
        } else {
            statusLed.off(); // Dioda gaśnie po zwolnieniu
        }
    }
}