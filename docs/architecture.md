# Architecture Overview

This project follows a modular architecture inspired by real-world web servers. The system is divided into clear layers to separate responsibilities, support parallel development, and keep the codebase maintainable.

The architecture is organized around three core layers:
- Transport and I/O (low-level engine)
- HTTP protocol handling
- Application-level behavior (configuration, routing, features)

Each layer communicates through explicit interfaces and does not depend on implementation details of the others.

---

## High-level flow

Request lifecycle, end to end:
1. A client connects to the server  
2. The I/O engine receives raw bytes from the socket  
3. The HTTP module parses the bytes into a `Request` object  
4. The routing layer selects the appropriate handler  
5. A handler generates a `Response`  
6. The HTTP module serializes the `Response` into bytes  
7. The I/O engine sends the response back to the client  

This strict flow prevents cross-module coupling and keeps responsibilities clear.

---

## Module 1 — I/O Engine (`srcs/io/`)

**Responsibility:** low-level networking and event management.

This layer is responsible for:
- Socket creation and management  
- Multiplexing (poll/epoll/kqueue depending on implementation)  
- Event loop  
- Accepting connections  
- Reading and writing to sockets  
- Connection lifecycle management  
- Timeouts  

This module does **not** understand HTTP.  
It only deals with bytes and connection state.

Typical abstractions in this layer:
- `EventLoop`
- `Connection`
- `Buffer`
- `Socket`

Its role is to say:
- "Here are the bytes received from this connection"
- "Send these bytes to this connection"

---

## Module 2 — HTTP Core (`srcs/http/`)

**Responsibility:** HTTP protocol implementation.

This layer handles:
- Parsing HTTP requests (request line, headers, body)  
- `Request` and `Response` data models  
- Status codes  
- Headers  
- Serialization of responses  
- Basic protocol rules (keep-alive, Content-Length, etc.)  

It receives raw bytes from the I/O module and transforms them into structured objects.

It does **not**:
- Know about routing rules  
- Know about filesystem structure  
- Decide business logic  

It only understands HTTP.

---

## Module 3 — Config, Routing and Features  
(`srcs/config/`, `srcs/routing/`, `srcs/handlers/`)

**Responsibility:** application behavior.

This layer includes:

### Configuration
- Parsing configuration files  
- Virtual servers  
- Locations  
- Limits  
- Root paths  

### Routing
- Matching requests to routes  
- Selecting which handler should process a request  
- Enforcing allowed methods  

### Handlers
- Static file serving  
- File upload  
- DELETE  
- CGI execution  
- Error handling  

This layer consumes structured `Request` objects and produces structured `Response` objects.

---

## Design principle: separation of concerns

Each layer follows strict responsibility boundaries:
- I/O does not know HTTP  
- HTTP does not know routing or filesystem  
- Routing does not know sockets or epoll  
- Handlers do not manage connections  

This avoids tight coupling and allows:
- Easier debugging  
- Parallel work between team members  
- Cleaner code reviews  
- Safer refactoring  

---

## Interfaces between modules

These interfaces define the contract between layers:

- **I/O → HTTP**  
  Raw bytes received for a specific connection  

- **HTTP → Routing/Handlers**  
  Parsed `Request` object (or parsing error)  

- **Handlers → HTTP**  
  Structured `Response` object (status, headers, body/stream)  

- **HTTP → I/O**  
  Serialized response bytes ready to be sent  

As long as these contracts are respected, internal implementations can evolve independently.

---

## Folder structure mapping

```text
srcs/
  io/        # I/O Engine (event loop, connections, sockets)
  http/      # HTTP parsing, Request/Response, serialization
  config/    # Configuration parsing and models
  routing/   # Route matching and dispatch logic
  handlers/  # Request handlers (static, upload, CGI, delete)
