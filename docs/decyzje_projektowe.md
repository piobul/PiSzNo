# Design decisions (decyzje projektowe)

Last update: 2026-11-14. Move an item from "Open" to "Decided" when the team agrees.

## Decided

- One repository for the whole system. Folders: `pb/`, `firmware/`, `network/`, `labview/`, `hardware/`,
  `design-lab/`, `docs/`.
- Weekly Design Lab programs stay in `design-lab/week-N/`; the final firmware is in `firmware/`.
- Firmware is built with PlatformIO, opened from the `firmware/` folder.
- The firmware uses `pb` through `lib_deps = symlink://../pb` (tested on Windows).
- PC code (`pb`, server, client) is built with CMake (root `CMakeLists.txt` still to be written).
- The Object-Oriented Programming part (`pb/`) is a separate project with its own README.
- Server is written in C++ and runs on Linux.
- Board and server communicate over UART. The Ethernet (RJ45) module is dropped.
- LabVIEW works on the client side and connects to the server as a TCP client.
- Idea for LabVIEW: send configuration commands automatically (for example measurement interval).

## Open

| Topic | Options | Note |
|---|---|---|
| Computer running LabVIEW | with the server / with a client | |
| `Measurement` as template | `Measurement<T>` or plain class | affects the class diagram |
| TLV implementation | server and client use `pb::TlvFrame` / own code | ask the Network Programming instructor |
| Shared server configuration | which values (interval, alarm thresholds, ...) | needed for the README and protocol |
| UART framing | start marker, checksum | UART has no message boundaries |
| C++ standard on AVR | C++17 / C++14 | check with the AVR toolchain |
| "CMake and Make" bonus | CMake only / plus a Makefile | ask the OOP instructor |
| Serial port in `platformio.ini` | fixed port / auto-detect / per-user file | `monitor_port` is now fixed |

## Questions for the instructors

- **Object-Oriented Programming:** is code without exceptions and STL on the microcontroller accepted
  (setters return `bool`)? What form of demo on the microcontroller is expected? Is a shared project
  with other courses allowed? Does "CMake and Make" need a separate Makefile?
- **Network Programming:** is a shared project with other courses allowed? Is a library from another course
  acceptable as a dependency? What are the current deadlines (the file lists dates from 2025)?
- **Computer Measurement Systems:** is a LabVIEW panel connecting to a server over TCP accepted as
  communication with an additional device?

## If the TLV format changes, update

`pb::TlvFrame`, firmware, server, console client, LabVIEW parser, `docs/protocol.md`,
Network Programming documentation.