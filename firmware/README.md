# Firmware

Final microcontroller firmware of the weather station (PlatformIO).

## Open and build

1. Open this folder (`firmware/`) in VS Code with the PlatformIO extension.
   PlatformIO finds `platformio.ini` only when it is in the opened folder.
2. Run Build. The `pb` library is taken from `../pb` (`lib_deps = symlink://../pb`).

## Notes

- The prototype is developed on an ATmega328PB Xplained Mini, the target PCB uses an ATmega328.
- `monitor_port` in `platformio.ini` is set for one computer, change it to match yours.
- Current state: copy of `design-lab/week-2/` (counter, LED bar, UART output).
  TODO: switch to `pb` classes, add DS18B20, BMP280, SD card and TLV frames.