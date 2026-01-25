## Module layout

### MODULE 1 — I/O Engine
This module is the foundation of the server. It handles multiplexing (poll/epoll/kqueue), the event loop, connection lifecycle, timeouts, and low-level socket operations (accept/read/write). It does not understand HTTP; it only receives and sends raw bytes.

Typical abstractions exposed by this module include:
- `Connection`
- `Buffer`
- `EventLoop`
- callbacks

### MODULE 2 — HTTP Core
This module is responsible for the HTTP protocol itself. It includes request parsing (request line, headers, body), `Request`/`Response` models, serialization, and core protocol rules (status codes, keep-alive, Content-Length vs chunked if implemented). It consumes raw bytes from the I/O module and produces structured HTTP objects.

### MODULE 3 — Config, Routing and Features
This module handles server configuration parsing, virtual hosts, route/location matching, allowed methods, and mapping URIs to handlers (static files, upload, DELETE, CGI). It uses the `Request`/`Response` abstractions from the HTTP module and delegates final response transmission to the I/O module.

### Design note
For better maintainability, routing and handlers are logically separated:
- Routing decides which handler should process a request.
- Handlers implement the actual behavior (e.g. `StaticHandler`, `UploadHandler`, `CgiHandler`, `DeleteHandler`).

## Module boundaries (interfaces)

Defining clear interfaces between modules early helps avoid tight coupling and simplifies parallel development.

- **I/O → HTTP:** raw bytes received for a given connection
- **HTTP → Features:** fully parsed `Request` (or parsing error)
- **Features → HTTP:** structured `Response` (status, headers, body/stream)
- **HTTP → I/O:** serialized bytes ready to be sent

## Folder structure mapping

```text
srcs/
  io/        # Module 1 (I/O Engine)
  http/      # Module 2 (HTTP Core)
  config/    # Module 3 (Configuration)
  routing/   # Module 3 (Routing logic)
  handlers/  # Module 3 (Request handlers)
