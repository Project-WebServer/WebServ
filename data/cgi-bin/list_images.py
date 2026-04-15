#!/opt/pyenv/shims/python3
import os
import json

UPLOAD_DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'upload')
IMAGE_EXTS = {'.jpg','.png'}

files = []
try:
    for f in sorted(os.listdir(UPLOAD_DIR)):
        if os.path.splitext(f)[1].lower() in IMAGE_EXTS:
            files.append(f)
except Exception:
    pass

body = json.dumps(files)
print("Content-Type: application/json")
print()
print(body)