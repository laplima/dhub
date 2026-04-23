---
language: C++26
libraries: acetao spdlog
author: Luiz Lima Jr.
date: 2025-01-13
---
# Distributed HUB

This design is used to allow connections among an arbitrary number of servers as they become available, creating a complete graph without prior mutual knowledge.

The hub component must be run first and have a well-known network address (or IOR, found using the name service; default name = "*hub*"). Then, each server that is launched announces itself to the hub, which forwards the missing information to the other servers so that, in the end, all servers are notified of each other. Servers must implement the Hub interface in order to get notified.

## Compilation

```bash
cmake -S . -B build -DCMAKE_PREFIX_PATH=$COLIBRY/lib64/cmake
cmake --build build
```

Then, run the hub and the servers (providing an id for each server).

