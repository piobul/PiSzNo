# pb – C++ library for sensors, buffers and TLV frames

Project for the Object-Oriented Programming course. Author: Piotr B. (namespace `pb`).

The library is meant for embedded use (ATmega328) and also compiles on a PC. It models the sensors of
the PiSzNo weather station, stores measurements in a ring buffer and encodes them into binary TLV frames.

TODO: 1–2 sentences about possible uses outside this project.

## Classes (planned, not implemented yet)

| Class | Role |
|---|---|
| `Sensor` | Abstract sensor interface |
| `TemperatureSensor`, `PressureSensor` | Abstract classes for sensor kinds |
| `Ds18b20`, `Bmp280` | Concrete sensors |
| `OneWireBus`, `I2cBus` | Hardware access interfaces that can be replaced by test doubles |
| `Measurement` | One measurement value with optional timestamp (TODO: decide if it becomes a template) |
| `RingBuffer<T, N>` | Ring buffer with size known at compile time |
| `TlvFrame` | Encoding and decoding of TLV frames |
| `WeatherStation` | Collects sensors and holds the measurement buffer |

Class diagram: `docs/class_diagram.puml`.

## Design rules

- All classes live in `namespace pb`.
- Classes never print or read from standard input; output is done only in `examples/main.cpp`.
- No exceptions and no STL on the microcontroller; setters return `bool`.
- No microcontroller-specific headers inside the library, hardware is reached through the bus classes.
- Doxygen comments only in `.hpp` files.

## Building

- **On a PC:** TODO: add CMake commands.
- **On the microcontroller:** the firmware in `firmware/` uses the library through
  `lib_deps = symlink://../pb` in its `platformio.ini`.

## Usage example

TODO: short example, see `examples/main.cpp`.

## Tests

TODO: how to run the unit tests.

## Report

TODO: link to the PDF report in `docs/`.