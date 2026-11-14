# Design decisions (decyzje projektowe)

Last update: 2026-11-14. Move an item to "Decided" when the team agrees.

## Decided

- One repository for the whole system. Folders: `pb/`, `firmware/`, `network/`, `labview/`, `hardware/`,
  `design-lab/`, `docs/`.
- Weekly Design Lab programs stay in `design-lab/week-N/`; the final firmware is in `firmware/`.
- Firmware is built with PlatformIO, opened from the `firmware/` folder.
- The firmware uses `pb` through `lib_deps = symlink://../pb` (tested on Windows).
- PC code (`pb`, server, client) is built with CMake, which generates Makefiles (root `CMakeLists.txt` still
  to be written). The Object-Oriented Programming instructor accepts either CMake or Make.
- `Measurement` in `pb` is a template (`Measurement<T>`).
- The project uses C++17. On the AVR toolchain it needs `build_unflags = -std=gnu++11` and
  `build_flags = -std=gnu++17` in `platformio.ini` (tested on a test project).
- The Object-Oriented Programming part (`pb/`) is a separate project with its own README.
- Server is written in C++ and runs on Linux.
- Board and server communicate over UART. The Ethernet (RJ45) module is dropped.
- LabVIEW is a TCP client of the server. It runs on the client computer; if time allows, also on the server
  computer (the same program with a different server address).
- Idea for LabVIEW: send configuration commands automatically (for example measurement interval).
- One TLV frame is used everywhere; the server and the console client use `pb::TlvFrame`.
- `monitor_port` in `platformio.ini` is set by hand for now; automatic selection is a later goal.
- Instructors (as reported by PB): a shared project is accepted by Object-Oriented Programming while that part
  is done by one person, and by Network Programming for a team of three. Using `pb` as a dependency of the
  server and client is accepted.

## To be decided during implementation

| Topic | Note |
|---|---|
| Shared server configuration | which values clients can change; decided while building the server |
| UART framing | start marker and checksum; decided while building firmware and server |
| Automatic serial port | later; options: no `monitor_port`, per-user file, USB identifiers |

## Open

| Topic | Options | Note |
|---|---|---|
| Type returned by sensors | `Measurement<int32_t>` (fixed point) / `Measurement<float>` | decide with the first sensor class |

## Questions still open

- **Network Programming:** confirm the exact deadlines on UPEL (outline and final submission; the final one
  is remembered as about 22 January 2027).

## If the TLV format changes, update

`pb::TlvFrame`, firmware, server, console client, LabVIEW parser, `docs/protocol.md`,
Network Programming documentation.