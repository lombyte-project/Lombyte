#!/usr/bin/env python3
"""Build src/overlays and prove every function in C against the level text.

    python3 scripts/verify-overlays.py      (what `make overlays` runs)

Ends with PASS when every overlay function written in C is byte-exact.
"""
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import overlay_unit  # noqa: E402

if __name__ == "__main__":
    raise SystemExit(overlay_unit.verify_all())
