#!/usr/bin/env python3
"""Format C sources the human-first way: clang-format (.clang-format), then one
blank line between top-level blocks, then INCLUDE_ASM/INCLUDE_RODATA lines kept
whole so the build tools can read them. Formatting never changes the code the
compiler makes; `make elf` and `make overlays` prove it.

  python3 scripts/format.py [FILE...]          format in place (default: every tracked .c/.h)
  python3 scripts/format.py --check [FILE...]  list files that are not formatted (exit 1)
"""
import argparse, os, re, shutil, subprocess, sys, tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "scripts"))
import importlib.util
_spec = importlib.util.spec_from_file_location("blank_blocks", ROOT / "scripts" / "blank-blocks.py")
blank_blocks = importlib.util.module_from_spec(_spec)
_spec.loader.exec_module(blank_blocks)

CLANG_FORMAT = os.environ.get("CLANG_FORMAT", "clang-format")
WHOLE_LINE_MACROS = re.compile(r"^(INCLUDE_ASM|INCLUDE_RODATA)\((?:[^;]|\n)*?\);", re.M)


def tracked() -> list[str]:
    out = subprocess.run(["git", "ls-files", "src/*.c", "src/*.h", "include/*.h"],
                         cwd=ROOT, capture_output=True, text=True, check=True).stdout
    return [line for line in out.split("\n") if line]


def clang_format(paths: list[str], cwd: Path) -> None:
    for i in range(0, len(paths), 200):
        subprocess.run([CLANG_FORMAT, "-i", "--style=file:" + str(ROOT / ".clang-format"), *paths[i:i + 200]],
                       cwd=cwd, check=True)


# An assembler label or attribute stays on the line of its declaration, where
# the report and proof scripts look for it.
SPLIT_LABEL = re.compile(r"(__asm__\s*\(|__attribute__\s*\(\()\s*\n\s*")


def join_macros(text: str) -> str:
    text = SPLIT_LABEL.sub(r"\1", text)
    return WHOLE_LINE_MACROS.sub(lambda m: re.sub(r"\(\s+", "(", re.sub(r",\s*\n\s*", ", ", m.group(0))), text)


def format_tree(paths: list[str], cwd: Path) -> None:
    clang_format(paths, cwd)
    for rel in paths:
        p = cwd / rel
        text = p.read_text(encoding="latin-1")
        new = blank_blocks.fix(text)
        if new != text:
            p.write_text(new, encoding="latin-1")
    clang_format(paths, cwd)
    for rel in paths:
        p = cwd / rel
        text = p.read_text(encoding="latin-1")
        new = join_macros(text)
        if new != text:
            p.write_text(new, encoding="latin-1")


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--check", action="store_true")
    ap.add_argument("files", nargs="*")
    args = ap.parse_args()
    paths = [os.path.relpath(Path(f).resolve(), ROOT) for f in args.files] or tracked()
    paths = [p for p in paths if p.endswith((".c", ".h"))]
    if not args.check:
        format_tree(paths, ROOT)
        return 0
    with tempfile.TemporaryDirectory() as tmp:
        for rel in paths:
            (Path(tmp) / rel).parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(ROOT / rel, Path(tmp) / rel)
        format_tree(paths, Path(tmp))
        bad = [rel for rel in paths
               if (Path(tmp) / rel).read_bytes() != (ROOT / rel).read_bytes()]
    for rel in bad:
        print(f"{rel}: not formatted; run python3 scripts/format.py {rel}")
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
