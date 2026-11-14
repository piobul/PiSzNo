# PiSzNo – weather station

PiSzNo is a single weather-station system built to satisfy the requirements of four university courses:
Network Programming, Computer Measurement Systems and Design Lab.
A microcontroller board measures temperature and pressure, a Linux server collects and shares the data,
and two clients (console and LabVIEW) display it and change the shared configuration.

TODO: add 1–2 sentences about the goal of the project in the team's own words.

## Courses

| Course | Part of the system |
|---|---|
| Network Programming (Programowanie sieciowe) | Server daemon and console client |
| Computer Measurement Systems (Komputerowe systemy pomiarowe) | LabVIEW measurement panel |
| Design Lab | Hardware: PCB designed in KiCad, firmware on the microcontroller |
| *Object-Oriented Programming (Programowanie obiektowe) | C++ library `pb`: sensors, ring buffer, TLV frames |

## Architecture

```
[ATmega328: DS18B20, BMP280, SD card, 7-seg display, LEDs, buttons]
        |  UART (TLV frames)
        v
[Linux server (daemon, C++)]  <--- multicast discovery ---  [console client (C++)]
        |  TCP unicast
        +--------------------------------------------------> [console client (C++)]
        |
        +--> [LabVIEW panel]    TODO: decide how LabVIEW connects (VISA/UART or TCP)
```

- **Firmware** reads the sensors and sends measurements over UART as binary TLV frames.
- **Server** runs as a daemon (logging to syslog), serves many clients concurrently and keeps
  a shared configuration that clients can change under synchronisation.
- **Clients** find the server with a multicast request and then talk to it over TCP.
- **TLV protocol** is one shared frame format used by every component.

TODO: describe the shared configuration (which values, e.g. measurement interval, alarm thresholds).

## Repository layout

| Folder | Content |
|---|---|
| `pb/` | C++ library (OOP course); `include/pb/`, `src/`, `examples/`, `tests/`, `docs/` |
| `firmware/` | Final microcontroller firmware (PlatformIO) |
| `network/` | `server/` (daemon), `client/` (console client), `docs/` |
| `labview/` | LabVIEW panel and its user manual |
| `hardware/` | KiCad schematic and PCB, `datasheets/` |
| `design-lab/` | Weekly Design Lab programs (`week-1/`, `week-2/`, ...) |
| `docs/` | Design decisions and the TLV protocol specification (to be added) |

`PiSzNo.code-workspace` opens `firmware/`, `pb/` and the whole repository
in one VS Code window.

## Status

Snapshot, see `PLAN.md` for the task list.

| Component | State |
|---|---|
| Firmware | Prototype from Design Lab: counter, LED bar, UART output (copy of `design-lab/week-2/`) |
| `pb` library | Folder skeleton and class diagram (`pb/docs/class_diagram.puml`) |
| Server and client | Not started |
| LabVIEW panel | Not started |
| Hardware | Partial KiCad schematic (counter and LEDs), datasheets collected |

## Building

- **Firmware:** open the `firmware/` folder in VS Code with the PlatformIO extension and run Build.
  The prototype is developed on an ATmega328PB Xplained Mini, the target PCB uses an ATmega328.
  `monitor_port` in `platformio.ini` is set for one computer, change it to match yours.
- **`pb` library and network programs:** TODO: add CMake commands.
- **LabVIEW panel:** TODO: add LabVIEW version and required packages.
- **Hardware:** TODO: add KiCad version.

## Documentation

- `docs/` – design decisions and TLV protocol (TODO: add links when the files exist)
- `pb/docs/` – class diagram and OOP report (TODO)
- `network/docs/` – Network Programming outline and documentation (TODO)
- `labview/` – user manual (TODO)

## Authors

TODO: names and roles.