#!/usr/bin/env python3
"""`make overlays`: prove every level function in C against retail.

usage: overlay-check.py [--write]

--write rewrites config/us/levels/compiler_data.tsv and exe_data.tsv from the
build (scripts/lib/overlay_unit.py does the work).
"""
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent / "lib"))
import overlay_unit  # noqa: E402

if __name__ == "__main__":
    raise SystemExit(overlay_unit.verify_all(write="--write" in sys.argv[1:]))
