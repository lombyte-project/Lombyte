#!/usr/bin/env python3
"""README Naming Desk card with the community count, as the website shows it.

Counts the names people gave through `naming` issues (one ```yaml block per
name; closed as completed = given, open = waiting for review) and writes
assets/naming-desk-card.svg with the line "N names from the community so far"
under its code window. The ``progress`` workflow publishes the result on the
``progress`` branch, which the README shows.

Usage::

    python3 scripts/naming_desk_card.py OUTPUT_SVG

GITHUB_TOKEN, when set, is sent with the GitHub API calls. Without network
access the card is written without the line.
"""

from __future__ import annotations

import json
import os
import re
import sys
import urllib.request
from pathlib import Path

REPO = "lombyte-project/lombyte"
CARD = Path(__file__).resolve().parent.parent / "assets" / "naming-desk-card.svg"


def names(body: str | None) -> int:
    return len(re.findall(r"```yaml", body or "")) or 1


def count() -> tuple[int, int] | None:
    done = waiting = 0
    for page in range(1, 11):
        request = urllib.request.Request(
            f"https://api.github.com/repos/{REPO}/issues?labels=naming&state=all&per_page=100&page={page}",
            headers={"Accept": "application/vnd.github+json"},
        )
        if os.environ.get("GITHUB_TOKEN"):
            request.add_header("Authorization", f"Bearer {os.environ['GITHUB_TOKEN']}")
        try:
            with urllib.request.urlopen(request, timeout=30) as response:
                issues = json.load(response)
        except OSError as error:
            print(f"naming issues not read: {error}", file=sys.stderr)
            return None
        for issue in issues:
            if "pull_request" in issue:
                continue
            if issue["state"] == "open":
                waiting += names(issue.get("body"))
            elif issue.get("state_reason") == "completed":
                done += names(issue.get("body"))
        if len(issues) < 100:
            break
    return done, waiting


def line(done: int, waiting: int) -> str:
    bold = '<tspan fill="#de812f" font-weight="700">{}</tspan>'
    text = f'{bold.format(done)} {"name" if done == 1 else "names"} from the community so far'
    if waiting:
        text += f' &#183; {bold.format(waiting)} waiting for review'
    return (f'<text class="ui" x="848" y="285" font-size="13" fill="#8b949e" '
            f'text-anchor="end">{text}</text>\n')


def main(argv: list[str]) -> int:
    if len(argv) != 2:
        print(__doc__, file=sys.stderr)
        return 2
    svg = CARD.read_text(encoding="utf-8")
    counts = count()
    if counts:
        # 24 px taller, so the line under the code window keeps the card's margin.
        svg = (svg.replace('height="300" viewBox="0 0 880 300"', 'height="324" viewBox="0 0 880 324"')
                  .replace('<rect width="880" height="300"', '<rect width="880" height="324"')
                  .replace("</svg>", line(*counts) + "</svg>"))
    out = Path(argv[1])
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text(svg, encoding="utf-8")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
