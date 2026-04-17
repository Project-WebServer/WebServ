#!/usr/bin/python3
import os, sys

length = int(os.environ.get("CONTENT_LENGTH", 0))
body = sys.stdin.buffer.read(length)

os.makedirs("../upload", exist_ok=True)
with open("../upload/form.txt", "wb") as f:
    f.write(body)

sys.stdout.write("HTTP/1.1 201 Created\r\nContent-Type: text/plain\r\n\r\nOK\n")