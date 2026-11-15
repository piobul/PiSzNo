#pragma once
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

namespace Timer {
    // Domyślny czas startu: 2026-11-15 12:00:00
    constexpr uint16_t START_YEAR  = 2026;
    constexpr uint8_t  START_MONTH = 11;
    constexpr uint8_t  START_DAY   = 15;
    constexpr uint8_t  START_HOUR  = 12;
    constexpr uint8_t  START_MIN   = 0;
    constexpr uint8_t  START_SEC   = 0;

    // Bezpieczny licznik bez zmiennych globalnych C++17
    inline volatile uint32_t& counter() {
        static volatile uint32_t ms_counter = 0;
        return ms_counter;
    }

    inline void init() {
        // Tryb CTC, preskaler 64 -> 16 MHz / 64 = 250 kHz (1 tick = 4 us)
        TCCR0A = (1 << WGM01);
        TCCR0B = (1 << CS01) | (1 << CS00);
        OCR0A  = 249; // 250 ticków * 4 us = 1000 us (1 ms)
        TIMSK0 = (1 << OCIE0A);
        sei();
    }

    inline uint32_t millis() {
        uint32_t m;
        uint8_t sreg = SREG;
        cli();
        m = counter();
        SREG = sreg;
        return m;
    }

    inline bool isLeapYear(uint16_t y) {
        return ((y % 4 == 0) && (y % 100 != 0)) || (y % 400 == 0);
    }

    inline uint8_t daysInMonth(uint16_t y, uint8_t m) {
        const uint8_t days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if (m == 2 && isLeapYear(y)) return 29;
        return days[m - 1];
    }

    // Zapisuje datę i godzinę do bufora (min. 20 bajtów): "2026-11-15 12:00:00"
    inline void getDateTime(char* buf, size_t bufSize) {
        uint32_t elapsedSec = millis() / 1000UL;
        uint32_t totalSec   = (uint32_t)START_HOUR * 3600UL +
                              (uint32_t)START_MIN  * 60UL +
                              (uint32_t)START_SEC  + elapsedSec;

        uint32_t extraDays = totalSec / 86400UL;
        uint32_t daySec    = totalSec % 86400UL;

        uint8_t hour = daySec / 3600UL;
        uint8_t min  = (daySec % 3600UL) / 60UL;
        uint8_t sec  = daySec % 60UL;

        uint16_t year  = START_YEAR;
        uint8_t  month = START_MONTH;
        uint8_t  day   = START_DAY;

        while (extraDays > 0) {
            uint8_t dim = daysInMonth(year, month);
            if (day < dim) {
                day++;
            } else {
                day = 1;
                if (month < 12) {
                    month++;
                } else {
                    month = 1;
                    year++;
                }
            }
            extraDays--;
        }

        snprintf(buf, bufSize, "%04u-%02u-%02u %02u:%02u:%02u",
                 year, month, day, hour, min, sec);
    }
}