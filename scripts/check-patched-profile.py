#!/usr/bin/env python3
"""Report whether an installed patched EE-GCC profile matches the repository patch.

Usage:
  python3 scripts/check-patched-profile.py [--profile DIR] [--patch FILE]

``--profile`` defaults to ``$EE_GCC_PATCHED_ROOT``.  A profile built by
``scripts/build-patched-toolchain.py`` records the patch it came from in
``provenance.json``; when that hash no longer matches
``patches/ee-gcc-2.9-991111-01/patched-ee-gcc.patch``, patched-profile units
would compile with an old compiler and the full-ELF gate would fail for a non-obvious reason.

Exit codes: 0 match, 1 stale, 2 unverifiable (no provenance or patch file).
"""

from __future__ import annotations

import argparse
import hashlib
import json
import os
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
DEFAULT_PATCH = ROOT / "patches" / "ee-gcc-2.9-991111-01" / "patched-ee-gcc.patch"


def parse_args(argv=None):
    parser = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter
    )
    parser.add_argument(
        "--profile",
        type=Path,
        default=os.environ.get("EE_GCC_PATCHED_ROOT") or None,
        help="installed profile directory (default: $EE_GCC_PATCHED_ROOT)",
    )
    parser.add_argument(
        "--patch",
        type=Path,
        default=None,
        help=f"repository patch to compare against (default: {DEFAULT_PATCH})",
    )
    return parser.parse_args(argv)


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as handle:
        for chunk in iter(lambda: handle.read(1 << 16), b""):
            digest.update(chunk)
    return digest.hexdigest()


def check(profile: Path, patch: Path) -> int:
    """Compare the profile's recorded patch hash with the repository patch."""
    provenance = profile / "provenance.json"
    if not provenance.is_file():
        print(
            f"check-patched-profile: note: no provenance.json under {profile}; "
            "cannot tell which patch built this profile"
        )
        return 2
    try:
        built = json.loads(provenance.read_text()).get("patch_sha256")
    except (OSError, json.JSONDecodeError) as error:
        print(f"check-patched-profile: note: unreadable provenance.json: {error}")
        return 2
    if not patch.is_file():
        print(f"check-patched-profile: note: repository patch not found: {patch}")
        return 2
    if not built:
        print("check-patched-profile: note: provenance records no patch_sha256")
        return 2
    current = sha256(patch)
    if built == current:
        print(f"check-patched-profile: ok (patch {current[:12]}...)")
        return 0
    print(
        f"check-patched-profile: warning: this profile was built from patch "
        f"{built[:12]}..., but the repository patch is {current[:12]}..."
    )
    print(
        "check-patched-profile: warning: rebuild it with "
        "scripts/build-patched-toolchain.py before trusting patched-profile C"
    )
    return 1


def main(argv=None) -> int:
    args = parse_args(argv)
    if args.profile is None:
        print(
            "check-patched-profile: error: pass --profile or set EE_GCC_PATCHED_ROOT",
            file=sys.stderr,
        )
        return 2
    patch = args.patch if args.patch is not None else DEFAULT_PATCH
    return check(args.profile.expanduser(), patch)


if __name__ == "__main__":
    raise SystemExit(main())
