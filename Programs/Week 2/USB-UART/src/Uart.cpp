#include "Uart.hpp"

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

void Uart::init(unsigned long baudrate) {
    uint16_t ubrr = (F_CPU / (16UL * baudrate)) - 1;

    // Konfiguracja rejestrów USART0 (piny PD0/PD1)
    UBRR0H = (uint8_t)(ubrr >> 8);
    UBRR0L = (uint8_t)(ubrr);

    // Włączenie nadajnika (TX0) i odbiornika (RX0)
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);

    // Format ramki: 8 bitów danych, 1 bit stopu, brak parzystości (8N1)
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);

    sendString("\r\n--- UART START ---\r\n");
}

void Uart::sendChar(char c) {
    // Oczekiwanie na zwolnienie bufora nadawczego
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = c;
}

void Uart::sendString(const char* str) {
    while (*str) {
        sendChar(*str++);
    }
}

void Uart::sendNumber(uint16_t num) {
    char buffer[10];
    itoa(num, buffer, 10);
    sendString(buffer);
}