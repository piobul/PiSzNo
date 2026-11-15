#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdlib.h>
#include "Timer.hpp"
#include "Uart.hpp"
#include "Lcd.hpp"
#include "Button.hpp"
#include "Sensors.hpp"
#include "SdCard.hpp"
#include "ff.h"

Uart    uart;
Lcd     lcd;
Button  button;
Sensors sensors;
SdCard  sdCard;

FATFS   fs;
bool    fsMounted = false;
FRESULT lastFatErr = FR_OK;

SensorData currentData;
uint32_t lastReadTime   = 0;
uint32_t alertStartTime = 0;
bool showingAlert       = false;

void sendReportToPC(const SensorData& data) {
    char dateTime[22];
    Timer::getDateTime(dateTime, sizeof(dateTime));

    uart.putChar('[');
    uart.print(dateTime);
    uart.print_P(PSTR("] DHT11: "));
    if (data.dhtOk) {
        uart.printFloat(data.tempAir, 1);
        uart.print_P(PSTR(" C, "));
        uart.printInt(data.humidity);
        uart.putChar('%');
    } else {
        uart.print_P(PSTR("BLAD"));
    }

    uart.print_P(PSTR(" | Sonda: "));
    if (data.probeOk) {
        uart.printFloat(data.tempProbe, 2);
        uart.println_P(PSTR(" C"));
    } else {
        uart.println_P(PSTR("BLAD"));
    }
}

void updateLcdDisplay(const SensorData& data) {
    if (showingAlert) {
        if (Timer::millis() - alertStartTime < 1500) {
            return;
        }
        showingAlert = false;
        lcd.clear();
    }

    lcd.setCursor(0, 0);
    if (data.dhtOk) {
        lcd.print("Pow:");
        lcd.printFloat(data.tempAir, 1);
        lcd.putChar(223);
        lcd.print("C ");
        lcd.printInt(data.humidity);
        lcd.print("%  ");
    } else {
        lcd.print("DHT11: Blad!    ");
    }

    lcd.setCursor(0, 1);
    if (data.probeOk) {
        lcd.print("Sonda: ");
        lcd.printFloat(data.tempProbe, 2);
        lcd.putChar(223);
        lcd.print("C  ");
    } else {
        lcd.print("Sonda: Brak!    ");
    }
}

void showLcdAlert(const char* line1, const char* line2) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(line1);
    lcd.setCursor(0, 1);
    lcd.print(line2);
    showingAlert = true;
    alertStartTime = Timer::millis();
}

bool appendMeasurementToTxt(const SensorData& data) {
    if (!fsMounted) {
        lastFatErr = f_mount(&fs, "", 1);
        if (lastFatErr != FR_OK) {
            return false;
        }
        fsMounted = true;
    }

    FIL file;
    lastFatErr = f_open(&file, "pomiary.txt", FA_OPEN_APPEND | FA_WRITE);
    if (lastFatErr != FR_OK) {
        fsMounted = false;
        return false;
    }

    char buf[24];
    Timer::getDateTime(buf, sizeof(buf));
    f_puts("[", &file);
    f_puts(buf, &file);
    f_puts("] DHT11: ", &file);

    if (data.dhtOk) {
        dtostrf(data.tempAir, 1, 1, buf);
        f_puts(buf, &file);
        f_puts(" C, ", &file);
        itoa(data.humidity, buf, 10);
        f_puts(buf, &file);
        f_puts("%", &file);
    } else {
        f_puts("ERR", &file);
    }

    f_puts(" | Sonda: ", &file);
    if (data.probeOk) {
        dtostrf(data.tempProbe, 1, 2, buf);
        f_puts(buf, &file);
        f_puts(" C\r\n", &file);
    } else {
        f_puts("ERR\r\n", &file);
    }

    lastFatErr = f_close(&file);
    return (lastFatErr == FR_OK);
}

ISR(TIMER0_COMPA_vect) {
    Timer::counter()++;
}

int main() {
    Timer::init();
    uart.init(9600);
    lcd.init();
    button.init();
    sensors.init();

    uart.println_P(PSTR("=== Start systemu (ATmega328PB + FatFs) ==="));
    lcd.setCursor(0, 0);
    lcd.print("ATmega328PB AVR");
    lcd.setCursor(0, 1);
    lcd.print("Start systemu...");

    lastFatErr = f_mount(&fs, "", 1);
    if (lastFatErr == FR_OK) {
        fsMounted = true;
        uart.println_P(PSTR("[SD] Karta FAT32 OK."));
    } else {
        fsMounted = false;
        uart.print_P(PSTR("[SD] Blad FAT32 przy starcie: "));
        uart.printInt((int32_t)lastFatErr);
        uart.println();
    }

    currentData = sensors.readAll();
    sendReportToPC(currentData);
    lcd.clear();
    updateLcdDisplay(currentData);
    lastReadTime = Timer::millis();

    while (true) {
        uint32_t now = Timer::millis();

        if (now - lastReadTime >= 2000) {
            lastReadTime = now;
            currentData = sensors.readAll();
            sendReportToPC(currentData);
            updateLcdDisplay(currentData);
        } else if (showingAlert) {
            updateLcdDisplay(currentData);
        }

        if (button.wasPressed()) {
            uart.println_P(PSTR("[BTN] Zapis do pomiary.txt..."));

            if (appendMeasurementToTxt(currentData)) {
                uart.println_P(PSTR("[SD] Zapisano pomyslnie!"));
                showLcdAlert("Zapisano!", "Plik:pomiary.txt");
            } else {
                uart.print_P(PSTR("[SD] Blad! FAT: "));
                uart.printInt((int32_t)lastFatErr);
                uart.print_P(PSTR(" SPI: "));
                uart.printInt(sdCard.getLastError());
                uart.println();

                char errBuf[17];
                snprintf(errBuf, sizeof(errBuf), "Err FAT:%d SPI:%u", (int)lastFatErr, sdCard.getLastError());
                showLcdAlert("Blad zapisu SD!", errBuf);
            }
        }
    }
}