#!/usr/bin/env python3
"""Build the GitHub Pages site: copy site/ to build/site and fill in the
progress numbers from report.json and decomp_map.svg (the `progress` branch).

usage: build_site.py PROGRESS_DIR [SITE_URL]
"""
import datetime
import json
import pathlib
import shutil
import sys

progress = pathlib.Path(sys.argv[1])
site = sys.argv[2] if len(sys.argv) > 2 else "https://mateuszklysz.github.io/Lombyte/"
out = pathlib.Path("build/site")

m = json.loads((progress / "report.json").read_text())["measures"]
values = {
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
shutil.copy(progress / "decomp_map.svg", out / "decomp_map.svg")
for name in ("index.html", "robots.txt", "sitemap.xml"):
    text = (out / name).read_text()
    for key, value in values.items():
        text = text.replace(key, value)
    (out / name).write_text(text)
(out / ".nojekyll").touch()
print(f"site built in {out}")
