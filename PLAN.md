# PiSzNo – project plan

Update the checkboxes as work is done. Deadlines for Network Programming, Computer Measurement Systems and
Object-Oriented Programming are in January. TODO: write down the exact dates.

## Phase 0 – decisions and foundation

- [x] Repository restructured (`firmware/`, `pb/`, `network/`, `labview/`, `hardware/`, `design-lab/`, `docs/`)
- [x] New root README and `pb/README.md`
- [x] PlatformIO firmware builds the library from `../pb` (tested on Windows)
- [x] Decision: board talks to the server over UART (no Ethernet module)
- [ ] `docs/decyzje_projektowe.md` and `docs/protocol.md` added to the repo
- [ ] Root `.gitignore` cleaned up
- [ ] Decide: is `Measurement` a template (`Measurement<T>`)
- [ ] Decide: server and client use `pb::TlvFrame` for TLV
- [ ] Decide: which computer runs the LabVIEW panel
- [ ] Decide: which values form the shared server configuration
- [ ] Questions sent to the instructors (list in `docs/decyzje_projektowe.md`)

## Phase 1 – `pb` library (Object-Oriented Programming)

- [ ] Skeleton: `CMakeLists.txt`, `.clang-format` check, `examples/main.cpp`
- [ ] `Measurement` (several constructors, comparison operators)
- [ ] `RingBuffer<T, N>`
- [ ] `Sensor`, `TemperatureSensor`, `PressureSensor`
- [ ] Bus interfaces (`OneWireBus`, `I2cBus`) and test doubles
- [ ] `Ds18b20`, `Bmp280`
- [ ] `TlvFrame` (encode, decode, checksum)
- [ ] `WeatherStation`
- [ ] Unit tests on PC
- [ ] Doxygen comments in headers, lines up to 120 characters
- [ ] Class diagram matches the code (`pb/docs/class_diagram.puml`)
- [ ] Library runs on the microcontroller (live demo or recording)
- [ ] Report (PDF, two minutes to read)

## Phase 2 – firmware

- [ ] Replace the Design Lab classes with `pb` classes
- [ ] Read DS18B20 and BMP280
- [ ] Write measurements to the SD card
- [ ] Send TLV frames over UART
- [ ] 7-segment display, status LEDs, buttons

## Phase 3 – server (Network Programming)

- [ ] Outline (1 page A4) uploaded to UPEL and approved
- [ ] Daemon skeleton (background process, PID file, syslog)
- [ ] Read UART and parse TLV
- [ ] Shared data with synchronised access
- [ ] Concurrent TCP server (child processes, signals, timeouts)
- [ ] Multicast discovery
- [ ] Name resolution with `getaddrinfo`
- [ ] Documentation (1–2 pages A4, contribution of each person)

## Phase 4 – console client

- [ ] Find the server by multicast
- [ ] Show current measurements
- [ ] Change the shared configuration

## Phase 5 – LabVIEW panel (Computer Measurement Systems)

- [ ] TCP connection to the server, TLV parsing
- [ ] Charts of temperature and pressure
- [ ] Automatic configuration commands (for example measurement interval)
- [ ] User manual (`.pdf` or `.docx`)

## Phase 6 – hardware

- [ ] Complete KiCad schematic of the whole station
- [ ] PCB layout

## Phase 7 – handover

- [ ] Repository without executables and build output
- [ ] Instructors have access to the repository
- [ ] Documentation of each course submitted