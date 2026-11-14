# TLV protocol (DRAFT, not agreed yet)

One frame format for all components. Every item marked TODO needs a team decision.

## Frame

| Field | Size | Meaning |
|---|---|---|
| Type | 1 byte | message type |
| Length | 2 bytes | number of bytes in Value |
| Value | Length bytes | payload |

All multi-byte numbers are in network byte order (big-endian).

## Transports

- **UART (board to server):** UART is a stream of bytes without message boundaries, so frames need
  a start marker and a checksum. TODO: choose marker and checksum.
- **TCP (server and clients):** frames follow each other; the Length field marks the end of a frame.
  The receiver rejects frames longer than a fixed limit. TODO: choose the limit.
- **UDP multicast (discovery):** one frame per datagram.

## Message types (draft)

| Type | Name | Value | Transport |
|---|---|---|---|
| 0x01 | MEASUREMENT | sensor id (u8), value (i32, fixed point), timestamp (u32) | UART, TCP |
| 0x02 | CONFIG_GET | empty | TCP |
| 0x03 | CONFIG_SET | key (u8), value (u32) | TCP |
| 0x04 | CONFIG_REPLY | key (u8), value (u32) | TCP |
| 0x05 | ERROR | code (u8) | TCP |
| 0x10 | DISCOVER | empty | UDP multicast |
| 0x11 | ANNOUNCE | TCP port (u16) | UDP multicast |

## Values

- Measurement value is a fixed-point integer (value times 100), for example 21.37 degrees is 2137.
  This avoids differences in `float` format between the microcontroller and the PC.
- TODO: sensor ids.
- TODO: timestamp source (the board has no real-time clock in the current component list).
- TODO: configuration keys (for example measurement interval, alarm thresholds).
- TODO: error codes.

## Checklist after a change

Update `pb::TlvFrame`, firmware, server, console client, LabVIEW parser and this file.