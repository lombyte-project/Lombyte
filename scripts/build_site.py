#!/usr/bin/env python3
"""Build the GitHub Pages site: copy site/ to build/site and fill in the
progress numbers from report.json (the `progress` branch);
images come from assets/ and the progress branch, as in the README.

usage: build_site.py PROGRESS_DIR [SITE_URL]
"""
import datetime
import html
import json
import pathlib
import shutil
import subprocess
import sys

progress = pathlib.Path(sys.argv[1])
site = sys.argv[2] if len(sys.argv) > 2 else "https://mateuszklysz.github.io/Lombyte/"
out = pathlib.Path("build/site")

m = json.loads((progress / "report.json").read_text())["measures"]
REPO = "https://github.com/mateuszklysz/Lombyte"


def commits_bar(count=5):
    """Announcement bar with the latest commits on the checked-out branch."""
    log = subprocess.run(
        ["git", "log", f"-{count}", "--format=%H%x09%cs%x09%s"],
        capture_output=True, text=True, check=True,
    ).stdout.splitlines()
    items = "".join(
        f'<li><a href="{REPO}/commit/{sha}"><time datetime="{day}">{day}</time>'
        f"{html.escape(subject)}</a></li>"
        for sha, day, subject in (line.split("\t", 2) for line in log)
    )
    return (f'<aside class="bar" aria-label="Latest commits"><strong>Latest on main</strong>'
            f"<ol>{items}</ol></aside>")


values = {
    "@COMMITS@": commits_bar(),
    "@SITE@": site,
    "@DATE@": datetime.date.today().isoformat(),
    "@C_EXACT@": f"{float(m['matched_code_percent']):.2f}",
    "@FUNCS_DONE@": f"{int(m['matched_functions']):,}",
    "@FUNCS@": f"{int(m['total_functions']):,}",
    "@UNITS_DONE@": f"{int(m['complete_units']):,}",
    "@UNITS@": f"{int(m['total_units']):,}",
}

shutil.rmtree(out, ignore_errors=True)
shutil.copytree("site", out)
shutil.copy("assets/lombyte-logo.png", out)
# Small logo for the page itself (the full one stays for icons and previews).
from PIL import Image  # noqa: E402
Image.open("assets/lombyte-logo.png").resize((256, 259), Image.LANCZOS).save(out / "logo-256.png", optimize=True)
for name in ("index.html", "robots.txt", "sitemap.xml"):
    text = (out / name).read_text()
    for key, value in values.items():
        text = text.replace(key, value)
    (out / name).write_text(text)
(out / ".nojekyll").touch()
print(f"site built in {out}")
