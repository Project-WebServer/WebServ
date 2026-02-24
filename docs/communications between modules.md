A ↔ B ↔ C Integration Contract (copy/paste)

Ownership
- Track A (Networking/Event Loop) owns: sockets, non-blocking I/O, poll/epoll/kqueue, fd lifecycle, per-connection I/O state, read/write buffers, partial reads/writes, idle timeouts mechanics.
- Track B (HTTP Engine) owns: incremental HTTP parsing, request completion detection, HTTP validation, status codes for parse errors, HttpRequest/HttpResponse structures, response serialization, connection policy (keep-alive vs close) based on HTTP rules.

- Track C (Config/Routing/CGI) owns: config parsing, route/server selection, method rules per location, static/redirect/CGI decisions, CGI process/env/pipes/timeouts policy, mapping CGI output to HttpResponse.

Hard boundaries
- A must not implement HTTP rules (no 400/413 decisions, no header parsing).
- B must not touch sockets/poll/fds.
- C must not parse raw HTTP bytes or do socket I/O.

A → B: Incremental parse API
A accumulates raw bytes into in_buf and calls B repeatedly.
B returns one of:
- NEED_MORE_DATA
- REQUEST_COMPLETE (+ bytes_consumed, + HttpRequest)
- PARSE_ERROR (+ bytes_consumed, + ready HttpResponse error)
Also returned by B: ConnectionPolicy { should_close } (based on HTTP rules).

A action:
- erase bytes_consumed from in_buf
- if PARSE_ERROR: queue response bytes and send; close if should_close
- if REQUEST_COMPLETE: pass HttpRequest to C

B → A: Response serialization
B provides serialize(HttpResponse) -> std::string (full bytes).
A sends bytes with partial-write support (out_offset, send on writable readiness).

B → C: Routing API
C receives HttpRequest and returns either:
- IMMEDIATE_RESPONSE (+ HttpResponse), or
- ASYNC_CGI (+ CgiJob description: pid, stdin/stdout fds, timeout, etc.)
C may also return/override ConnectionPolicy for special cases.

A ↔ C: CGI I/O bridge (non-blocking)
If C returns ASYNC_CGI:
- A registers CGI stdout (readable) and stdin (writable if body streaming) in the event loop.
- A collects CGI stdout bytes until EOF.
- Then A calls B’s parseCgiOutput(cgi_raw) -> HttpResponse (or C provides mapping—pick one owner and stick to it).
- A serializes + sends response; close/keep based on policy.

Timeouts split
- A: tracks timestamps + triggers timeout events (mechanics).
- B/C: decide which HTTP response to generate on timeout (policy), or agree “close silently” and document it.

Keep-alive split
- B decides should_close from HTTP (Connection header, HTTP version, fatal errors).
- A executes it: after sending, either close fd or reset per-request state and continue reading next request on same connection.

Minimal integration flow
- A reads → append to in_buf
- A calls B.consume(in_buf)
- NEED_MORE → return to poll
- PARSE_ERROR → A sends B.error_response → close if policy says so
- REQUEST_COMPLETE → A calls C.route(req)
- Immediate → A sends response
- CGI → A manages CGI fds → on EOF builds response → sends