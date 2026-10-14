#!/usr/bin/env python3
"""List data labels (D_...) that src/ and include/ declare with more than one type
or more than one name.

A file that needs another placement keeps its own alias, named <name>_gp
(sda), <name>_lui (NOT_SDA) or <name>_macro (MACRO_ADDR); those count as the
same name. Everything else is a conflict to fix by hand: find what the
object really is (docs/code-cleanup.md, Data) and give every declaration that type.

usage: data-decls.py [--root DIR] [--count]
"""

import argparse
import collections
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]

BASE = {"s8": "signed char", "u8": "unsigned char", "s16": "short", "u16": "unsigned short",
        "s32": "int", "u32": "unsigned int", "s64": "long long", "u64": "unsigned long long",
        "f32": "float", "f64": "double", "b32": "int", "vs32": "volatile int", "vu32": "volatile unsigned int",
        "vs16": "volatile short", "vu16": "volatile unsigned short", "vs8": "volatile signed char",
        "vu8": "volatile unsigned char", "vs64": "volatile long long", "vu64": "volatile unsigned long long"}
ALIAS = re.compile(r"_(gp|lui|macro)$")
DECL = re.compile(r"^[ \t]*(?:extern\s+|__attribute__\s*\(\(\s*sda\s*\)\)\s+extern\s+)"
                  r"([^;(){}=]*?[\s*])(\w+)\s*((?:\[[^\]]*\]\s*)*)"
                  r"(?:__asm__\s*\(\s*\"(D_\w+)\"\s*\))?[^;{}=]*;", re.M)


def norm(t: str) -> str:
    t = re.sub(r"\b(?:extern|static|const)\b", " ", t)
    t = re.sub(r"\b\w+\b", lambda m: BASE.get(m.group(0), m.group(0)), t)
    t = re.sub(r"\bsigned (short|int)\b", r"\1", t)
    return re.sub(r"\s+", " ", t).replace(" *", "*").strip()


def declarations(root: Path):
    for path in sorted(list((root / "src").rglob("*.c")) + list((root / "include").rglob("*.h"))):
        text = path.read_text(encoding="latin-1")
        text = re.sub(r"/\*.*?\*/", lambda m: " " * len(m.group(0)), text, flags=re.S)
        for m in DECL.finditer(text):
            type_, name, dims, label = m.groups()
            label = label or (name if re.fullmatch(r"D_(?:L\d\d_)?[0-9A-Fa-f]{8}", name) else None)
            if not label:
                continue
            dims = re.sub(r"\[\s*\w*\s*\]", "[]", dims.replace(" ", "")) if dims else ""
            yield label, ALIAS.sub("", name), norm(type_) + dims, path.relative_to(root)


def conflicts(root: Path):
    types, names = collections.defaultdict(set), collections.defaultdict(set)
    where = collections.defaultdict(set)
    for label, name, type_, path in declarations(root):
        types[label].add(type_)
        names[label].add(name)
        where[label].add(str(path))
    out = {}
    for label in sorted(types):
        if len(types[label]) > 1 or len(names[label]) > 1:
            out[label] = (sorted(types[label]), sorted(names[label]), sorted(where[label]))
    return out


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--root", type=Path, default=ROOT)
    ap.add_argument("--count", action="store_true", help="print only the number of conflicting labels")
    args = ap.parse_args()
    found = conflicts(args.root)
    if args.count:
        print(len(found))
        return 0
    for label, (types, names, files) in found.items():
        print(f"{label}: types {' | '.join(types)}; names {', '.join(names)}; in {', '.join(files)}")
    print(f"{len(found)} labels with more than one type or name", file=sys.stderr)
    return 0


if __name__ == "__main__":
    sys.exit(main())
