# Server

Concurrent server running as a daemon on Linux, written in C++ (Network Programming).

Planned role: read measurements from the board over UART, serve many clients over TCP, answer multicast
discovery requests and keep a shared configuration that clients can change under synchronised access.
Messages are TLV frames (`docs/protocol.md`).

Status: not started. TODO: design details go into the Network Programming outline.

Build: TODO (CMake, needs Linux).