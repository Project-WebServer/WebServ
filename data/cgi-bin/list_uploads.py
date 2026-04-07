#!/usr/bin/python3
import os

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
UPLOAD_DIR = os.path.join(SCRIPT_DIR, "../upload")

def format_size(size):
    if size < 1024:
        return f"{size} B"
    elif size < 1024 * 1024:
        return f"{size / 1024:.1f} KB"
    else:
        return f"{size / (1024 * 1024):.1f} MB"

def get_icon(name):
    ext = name.rsplit('.', 1)[-1].lower() if '.' in name else ''
    if ext in ('png', 'jpg', 'jpeg', 'webp'):
        return '🖼', 'image'
    if ext == 'txt':
        return '📄', 'lyrics'
    if ext in ('mp3', 'wav', 'flac', 'ogg'):
        return '🎵', 'audio'
    return '📄', 'file'

IGNORE_EXT = {'html', 'htm'}

def list_files():
    files = []
    try:
        for entry in os.scandir(UPLOAD_DIR):
            if not entry.is_file():
                continue
            ext = entry.name.rsplit('.', 1)[-1].lower() if '.' in entry.name else ''
            if ext in IGNORE_EXT:
                continue
            stat = entry.stat()
            icon, kind = get_icon(entry.name)
            files.append({
                "name": entry.name,
                "size": format_size(stat.st_size),
                "kind": kind,
                "icon": icon,
                "mtime": stat.st_mtime
            })
    except FileNotFoundError:
        pass
    return sorted(files, key=lambda f: f["mtime"], reverse=True)

files = list_files()

rows = ""
for f in files:
    rows += f"""<tr>
        <td class="td-icon">{f['icon']}</td>
        <td class="td-name">{f['name']}</td>
        <td class="td-kind">{f['kind']}</td>
        <td class="td-size">{f['size']}</td>
        <td class="td-del"><button class="del-btn" onclick="deleteFile('{f['name']}', this)">✕</button></td>
    </tr>"""

if not rows:
    rows = "<tr><td colspan='5' class='empty'>no uploads yet</td></tr>"

count = len(files)

html = f"""<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Tracks — Soundwave</title>
  <link href="https://fonts.googleapis.com/css2?family=Playfair+Display:ital,wght@0,700;0,900;1,400&family=DM+Mono:wght@300;400&display=swap" rel="stylesheet">
  <style>
    * {{ margin:0; padding:0; box-sizing:border-box; }}
    :root {{
      --gold: #e8c547; --orange: #ff6b35;
      --dark: #080808; --dark2: #111; --dark3: #161616;
      --muted: #555; --text: #e8e0d0;
    }}
    body {{ background:var(--dark); color:var(--text); font-family:'DM Mono',monospace; min-height:100vh; padding:6rem 2rem 4rem; }}
    .status-bar {{ position:fixed; top:0; left:0; right:0; padding:0.9rem 2rem; display:flex; justify-content:space-between; align-items:center; border-bottom:1px solid rgba(255,255,255,0.05); background:rgba(8,8,8,0.95); backdrop-filter:blur(10px); z-index:100; }}
    .status-left {{ display:flex; align-items:center; gap:1.5rem; }}
    .logo {{ font-family:'Playfair Display',serif; font-size:1rem; font-weight:900; letter-spacing:0.15em; color:var(--gold); text-decoration:none; }}
    .logo span {{ color:var(--orange); }}
    .back-link {{ font-size:0.58rem; letter-spacing:0.12em; color:var(--muted); text-decoration:none; transition:color 0.2s; }}
    .back-link:hover {{ color:var(--gold); }}
    .status-right {{ font-size:0.55rem; color:var(--muted); letter-spacing:0.1em; }}
    .container {{ max-width:860px; margin:0 auto; }}
    .tag {{ font-size:0.58rem; letter-spacing:0.25em; text-transform:uppercase; color:var(--orange); display:flex; align-items:center; gap:0.7rem; margin-bottom:1rem; }}
    .tag::before {{ content:''; width:20px; height:1px; background:var(--orange); }}
    h1 {{ font-family:'Playfair Display',serif; font-size:clamp(1.8rem,4vw,2.8rem); font-weight:900; line-height:1; margin-bottom:0.6rem; }}
    h1 em {{ font-style:italic; color:var(--gold); }}
    .page-sub {{ font-size:0.68rem; color:var(--muted); line-height:1.7; margin-bottom:2.5rem; }}
    .count-badge {{ display:inline-block; font-size:0.58rem; letter-spacing:0.1em; color:var(--gold); border:1px solid rgba(232,197,71,0.3); padding:0.2rem 0.6rem; margin-left:0.8rem; vertical-align:middle; font-family:'DM Mono',monospace; }}
    .table-wrap {{ background:var(--dark2); border:1px solid #1a1a1a; }}
    table {{ width:100%; border-collapse:collapse; }}
    thead tr {{ border-bottom:1px solid #1a1a1a; }}
    th {{ font-size:0.55rem; letter-spacing:0.2em; text-transform:uppercase; color:var(--muted); padding:1rem 1.2rem; text-align:left; font-weight:400; }}
    td {{ padding:0.8rem 1.2rem; font-size:0.68rem; border-bottom:1px solid #161616; }}
    tr:last-child td {{ border-bottom:none; }}
    tr:hover td {{ background:rgba(232,197,71,0.02); }}
    .td-icon {{ width:2rem; font-size:1rem; }}
    .td-name {{ color:var(--text); }}
    .td-kind {{ color:var(--muted); font-size:0.6rem; letter-spacing:0.1em; }}
    .td-size {{ color:var(--muted); text-align:right; }}
    .td-del  {{ text-align:right; width:2.5rem; }}
    .del-btn {{ background:none; border:none; color:var(--muted); cursor:pointer; font-size:0.75rem; padding:0.2rem 0.4rem; transition:color 0.2s; }}
    .del-btn:hover {{ color:var(--orange); }}
    .empty {{ text-align:center; color:var(--muted); font-size:0.65rem; padding:3rem; letter-spacing:0.1em; }}
    .toast {{ position:fixed; bottom:2rem; left:50%; transform:translateX(-50%); background:var(--dark2); border:1px solid rgba(232,197,71,0.2); color:var(--text); font-size:0.65rem; letter-spacing:0.08em; padding:0.7rem 1.4rem; opacity:0; transition:opacity 0.3s; pointer-events:none; }}
    .toast.show {{ opacity:1; }}
    .toast.error {{ border-color:rgba(255,107,53,0.4); color:var(--orange); }}
  </style>
</head>
<body>
  <div class="status-bar">
    <div class="status-left">
      <a class="logo" href="/">SOUND<span>WAVE</span></a>
      <a class="back-link" href="/upload">← upload</a>
    </div>
    <div class="status-right">GET · /cgi-bin/list_uploads.py</div>
  </div>

  <div class="container">
    <div class="tag">Library</div>
    <h1>All <em>Uploads</em><span class="count-badge" id="count">{count} files</span></h1>
    <p class="page-sub">Files stored in /upload — click ✕ to delete.</p>
    <div class="table-wrap">
      <table>
        <thead><tr><th></th><th>Filename</th><th>Type</th><th style="text-align:right">Size</th><th></th></tr></thead>
        <tbody id="tbody">{rows}</tbody>
      </table>
    </div>
  </div>

  <div class="toast" id="toast"></div>

  <script>
    function deleteFile(name, btn) {{
      fetch('/upload/' + encodeURIComponent(name), {{ method: 'DELETE' }})
        .then(r => {{
          if (r.ok || r.status === 204) {{
            btn.closest('tr').remove();
            showToast(name + ' deleted.');
            const rows = document.querySelectorAll('#tbody tr').length;
            document.getElementById('count').textContent = rows + ' files';
          }} else {{
            showToast('Delete failed: ' + r.status, true);
          }}
        }})
        .catch(() => showToast('Server unreachable.', true));
    }}

    function showToast(msg, isError = false) {{
      const t = document.getElementById('toast');
      t.textContent = msg;
      t.className = 'toast show' + (isError ? ' error' : '');
      setTimeout(() => t.className = 'toast', 3000);
    }}
  </script>
</body>
</html>"""

print("Content-Type: text/html")
print("")
print(html)