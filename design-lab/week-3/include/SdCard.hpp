#pragma once
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include "Sensors.hpp"
#include "Timer.hpp"

class SdCard {
private:
    bool initialized = false;
    bool isSdhc = true;
    uint8_t lastError = 0;

    // Emulacja Open-Drain (bezpieczne ~3.4V dla czytnika bez bufora)
    void csLow()    { PORTE &= ~(1 << PE2); DDRE |= (1 << PE2); }
    void csHigh()   { DDRE &= ~(1 << PE2); PORTE |= (1 << PE2); }

    void mosiLow()  { PORTE &= ~(1 << PE3); DDRE |= (1 << PE3); }
    void mosiHigh() { DDRE &= ~(1 << PE3); PORTE |= (1 << PE3); }

    void sckLow()   { PORTC &= ~(1 << PC1); DDRC |= (1 << PC1); }
    void sckHigh()  { DDRC &= ~(1 << PC1); PORTC |= (1 << PC1); }

    bool misoRead() { return (PINC & (1 << PC0)) != 0; }

    uint8_t spiTransfer(uint8_t data) {
        uint8_t rx = 0;
        for (uint8_t i = 0; i < 8; i++) {
            if (data & 0x80) mosiHigh();
            else             mosiLow();
            data <<= 1;

            _delay_us(3);
            sckHigh();
            _delay_us(3);

            rx <<= 1;
            if (misoRead()) rx |= 0x01;

            sckLow();
        }
        mosiHigh();
        return rx;
    }

    uint8_t sendCommand(uint8_t cmd, uint32_t arg, uint8_t crc) {
        csHigh();
        spiTransfer(0xFF);
        csLow();
        spiTransfer(0xFF);

        spiTransfer(0x40 | cmd);
        spiTransfer((uint8_t)(arg >> 24));
        spiTransfer((uint8_t)(arg >> 16));
        spiTransfer((uint8_t)(arg >> 8));
        spiTransfer((uint8_t)(arg));
        spiTransfer(crc);

        uint8_t res = 0xFF;
        for (uint8_t i = 0; i < 16; i++) {
            res = spiTransfer(0xFF);
            if (!(res & 0x80)) break;
        }
        return res;
    }

public:
    bool isReady() const { return initialized; }
    uint8_t getLastError() const { return lastError; }

    bool init() {
        lastError = 0;
        SPCR1 = 0;

        DDRC  &= ~(1 << PC0);
        PORTC |= (1 << PC0);

        csHigh();
        mosiHigh();
        sckLow();
        _delay_ms(10);

        for (uint8_t i = 0; i < 10; i++) spiTransfer(0xFF);

        uint8_t cmd0Res = 0xFF;
        for (uint8_t attempt = 0; attempt < 20; attempt++) {
            cmd0Res = sendCommand(0, 0, 0x95);
            if (cmd0Res == 0x01) break;
            _delay_ms(10);
        }
        if (cmd0Res != 0x01) {
            csHigh();
            initialized = false;
            lastError = 1;
            return false;
        }

        uint8_t r7 = sendCommand(8, 0x000001AA, 0x87);
        uint32_t acmd41Arg = 0;
        if (r7 == 0x01) {
            for (uint8_t i = 0; i < 4; i++) spiTransfer(0xFF);
            acmd41Arg = 0x40000000;
        }

        uint16_t retry = 0;
        uint8_t res;
        do {
            sendCommand(55, 0, 0x65);
            res = sendCommand(41, acmd41Arg, 0x77);
            _delay_ms(5);
        } while (res != 0x00 && ++retry < 400);

        if (res != 0x00) {
            csHigh();
            initialized = false;
            lastError = 2;
            return false;
        }

        isSdhc = true;
        if (sendCommand(58, 0, 0xFF) == 0x00) {
            uint8_t ocr0 = spiTransfer(0xFF);
            spiTransfer(0xFF);
            spiTransfer(0xFF);
            spiTransfer(0xFF);
            isSdhc = (ocr0 & 0x40) != 0;
        }

        csHigh();
        spiTransfer(0xFF);

        initialized = true;
        return true;
    }

    // Odczyt jednego sektora 512 B (wymagane przez FatFs do odczytu tablicy FAT32)
    bool readBlock(uint32_t sector, uint8_t* buffer) {
        if (!initialized && !init()) return false;

        uint32_t addr = isSdhc ? sector : (sector * 512UL);
        if (sendCommand(17, addr, 0xFF) != 0x00) {
            csHigh();
            return false;
        }

        // Oczekiwanie na token startu pakietu danych (0xFE)
        uint16_t timeout = 0;
        uint8_t token = 0xFF;
        while ((token = spiTransfer(0xFF)) == 0xFF && ++timeout < 20000);

        if (token != 0xFE) {
            csHigh();
            return false;
        }

        for (uint16_t i = 0; i < 512; i++) {
            buffer[i] = spiTransfer(0xFF);
        }

        // 2 bajty CRC
        spiTransfer(0xFF);
        spiTransfer(0xFF);

        csHigh();
        spiTransfer(0xFF);
        return true;
    }

    // Zapis jednego sektora 512 B (wymagane przez FatFs do zapisu plików)
    bool writeBlock(uint32_t sector, const uint8_t* buffer) {
        if (!initialized && !init()) return false;

        uint32_t addr = isSdhc ? sector : (sector * 512UL);
        if (sendCommand(24, addr, 0xFF) != 0x00) {
            csHigh();
            return false;
        }

        spiTransfer(0xFF);
        spiTransfer(0xFE);

        for (uint16_t i = 0; i < 512; i++) {
            spiTransfer(buffer[i]);
        }

        spiTransfer(0xFF);
        spiTransfer(0xFF);

        uint8_t dataResp = 0xFF;
        for (uint8_t i = 0; i < 100; i++) {
            dataResp = spiTransfer(0xFF);
            if (dataResp != 0xFF) break;
        }

        if ((dataResp & 0x1F) != 0x05) {
            csHigh();
            return false;
        }

        uint32_t busyTimeout = 0;
        while (!misoRead() && ++busyTimeout < 100000UL) {
            spiTransfer(0xFF);
        }

        csHigh();
        spiTransfer(0xFF);
        return true;
    }
};