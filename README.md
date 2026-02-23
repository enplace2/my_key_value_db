# my_key_value_db

A simple key-value database built in C++17. This is a small practice project focused on writing modern C++, working with Protocol Buffers for binary serialization, and building a persistent storage engine from scratch.

## What It Does

Stores typed key-value pairs (string, int, bool, double, uint, and nested maps) in memory and persists them to disk using Protocol Buffers. Protobuf provides compact binary storage and generates C++ serialization code from a schema, avoiding the need to write a custom format. Includes an interactive CLI for reading and writing data.

```
> SET username alice string
Stored: username = alice (string)

> SET age 30 int
Stored: age = 30 (int)

> GET username
alice

> LIST
username: alice (string)
age: 30 (int)

> DELETE username
Deleted: username
```

## Setup

**Prerequisites:** CMake 3.27+, a C++17 compiler, and Protocol Buffers installed.

```bash
# Clone (with submodules for Google Test)
git clone --recurse-submodules <repo-url>

# Build
cmake -S . -B build
cmake --build build

# Run
./build/my_key_value_db

# Run tests
ctest --test-dir build
```

Data is stored at `~/.kv_db/default/store.kv`.

## Limitations

- The entire database is serialized to disk on every write (no incremental writes)
- No concurrent access support — single process only
- Nested maps are supported programmatically but not through the CLI
- No authentication or encryption
- No configurable storage location (hardcoded to `~/.kv_db/`)

## TODOs

- [ ] Write-ahead log (WAL) for crash recovery instead of full serialization on every write
- [ ] Batch write support to reduce disk I/O
- [ ] Nested map support in CLI (e.g. JSON input)
- [ ] Multiple named databases selectable from CLI
- [ ] Configurable storage directory
