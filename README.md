# PiSzNo – weather station

PiSzNo is a single weather-station system built to satisfy the requirements of three courses:
Network Programming, Computer Measurement Systems and Design Lab.
A microcontroller board measures temperature and pressure, a Linux server collects and shares the data,
and two clients (console and LabVIEW) display it and change the shared configuration.

The repository also contains `pb`, a standalone C++ library for sensors, a ring buffer and TLV frames.
It is a separate project for the Object-Oriented Programming course and is described in `pb/README.md`.
The firmware is planned to use it.

TODO: add 1–2 sentences about the goal of the project in the team's own words.

## Courses

| Course | Part of the system |
|---|---|
| Network Programming (Programowanie sieciowe) | Server daemon and console client |
| Computer Measurement Systems (Komputerowe systemy pomiarowe) | LabVIEW measurement panel |
| Design Lab | Hardware: PCB designed in KiCad, firmware on the microcontroller |

## Architecture

```
[ATmega328: DS18B20, BMP280, SD card, 7-seg display, LEDs, buttons]
        |  TLV frames over UART
        v
[Linux server (daemon, C++)]
        ^   multicast discovery + TCP (TLV frames)
        |
        +-- [console client (C++)]
        +-- [LabVIEW panel (TCP client)]
```

- **Firmware** reads the sensors and sends measurements as binary TLV frames over UART.
- **Server** runs as a daemon (logging to syslog), serves many clients concurrently and keeps
  a shared configuration that clients can change under synchronisation.
- **Clients** find the server with a multicast request and then talk to it over TCP.
  The LabVIEW panel is planned to send configuration commands automatically
  (for example to change the measurement interval).
- **TLV protocol** is one shared frame format used by every component (`docs/protocol.md`).

TODO: decide which computer runs the LabVIEW panel.
TODO: describe the shared configuration (which values clients can change).

## Repository layout

| Folder | Content |
|---|---|
| `pb/` | Standalone C++ library (see `pb/README.md`) |
| `firmware/` | Final microcontroller firmware (PlatformIO) |
| `network/` | `server/` (daemon), `client/` (console client), `docs/` |
| `labview/` | LabVIEW panel and its user manual |
| `hardware/` | KiCad schematic and PCB, `datasheets/` |
| `design-lab/` | Weekly Design Lab programs (`week-1/`, `week-2/`, ...) and their notes |
| `docs/` | Design decisions and the TLV protocol specification |

`PiSzNo.code-workspace` opens `firmware/`, `pb/`, `network/` and the whole repository
in one VS Code window. `PLAN.md` holds the task list.

## Status

Snapshot, see `PLAN.md` for the task list.

| Component | State |
|---|---|
| Firmware | Prototype from Design Lab: counter, LED bar, UART output (copy of `design-lab/week-2/`) |
| Server and client | Not started |
| LabVIEW panel | Not started |
| Hardware | Partial KiCad schematic (counter and LEDs), datasheets collected |

## Building

- **Firmware:** open the `firmware/` folder in VS Code with the PlatformIO extension and run Build.
  See `firmware/README.md`.
- **Network programs:** TODO: add CMake commands (the server needs Linux).
- **LabVIEW panel:** TODO: add LabVIEW version and required packages.
- **Hardware:** TODO: add KiCad version.
- **`pb` library:** see `pb/README.md`.

## Documentation

- `docs/decyzje_projektowe.md` – design decisions, open questions
- `docs/protocol.md` – TLV protocol (draft)
- `network/docs/` – Network Programming outline and documentation (TODO)
- `labview/` – user manual (TODO)
- `pb/README.md` – the `pb` library

## Authors

Piotr B. - programming
Szczepan P. - repositorium & docs
Norbert S. - schematics & PCB