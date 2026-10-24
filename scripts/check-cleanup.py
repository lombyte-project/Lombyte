#!/usr/bin/env python3
"""Check a change against the rules in docs/contributing/code-cleanup.md.

Every check runs on the base tree and on the working tree.  Only problems the
change adds are reported, so older code on main never fails the check; a
problem that is fixed or left as it was is fine.  Rules that need a person
(is a name proven, does a comment explain why) stay with the reviewer.

usage:
  check-cleanup.py [--base REV] [--title TEXT] [--body-file FILE] [--list]

  --base       compare with this revision (default: merge base with origin/main)
  --title      also check a pull request title as a commit subject
  --body-file  also check a pull request description for forbidden mentions
  --list       print every problem of the working tree, old ones too
"""

from __future__ import annotations

import argparse
import ast
import bisect
import collections
import importlib.util
import re
import subprocess
import sys
import tarfile
import tempfile
from dataclasses import dataclass
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SCRIPTS = Path(__file__).resolve().parent

# Files read from the base revision.
TREE = ["src", "include", "configure.py", "config/us/rnc1.us.yaml",
        "config/us/symbol_addrs.txt", "config/overlays/us/functions.tsv"]
YAML = "config/us/rnc1.us.yaml"
TEXT_SUFFIXES = {".c", ".h", ".py", ".md", ".json", ".yaml", ".yml", ".sh", ".txt",
                 ".tsv", ".s", ".inc", ".ld", ".toml", ".cfg"}
# A line with this marker is skipped by the forbidden-mention scan (its own patterns, test data).
WAIVER = "cleanup-check: allow"

# —— Patterns ——

FUN_LABEL = r"FUN_[0-9A-Fa-f]{8}"
DATA_LABEL = r"D_(?:L\d\d_)?[0-9A-Fa-f]{8}"
ASM_LABEL = re.compile(r'__asm__\s*\(\s*"([^"]*)"\s*\)')
LABEL_STRING = re.compile(r"[A-Za-z_.$][\w.$]*(?:\+\d+)?")
# The only files that may hold instructions: the approved quadword idiom and the
# EE-kernel primitives (privileged instructions C cannot express).
ASM_HEADERS = {"include/qcopy.h", "include/qzero.h", "include/kernel.h"}
ASM_ANY = re.compile(r"\b(?:__asm__|__asm|asm)\b(\s*(?:volatile|__volatile__)\b)?\s*\(")
FUNC_DECL = re.compile(r"\b(\w+)\s*\(((?:[^;{}()]|\([^()]*\))*)\)\s*__asm__\s*\(\s*\"(" + FUN_LABEL + r")\"\s*\)")
DATA_DECL = re.compile(r"\b(\w+)\s*((?:\[[^\]]*\]\s*)*)__asm__\s*\(\s*\"(" + DATA_LABEL + r")\"\s*\)")
FUNC_DEF = re.compile(r"^(?!\s)(?!(?:if|for|while|switch|return|else|do)\b)"
                      r"[A-Za-z_][\w\s\*]*?\b([A-Za-z_]\w*)\s*\(((?:[^;{}()]|\([^()]*\))*)\)\s*\{", re.M)
PROTO = re.compile(r"^(?:extern\s+)?(?!(?:return|else|goto|case)\b)[A-Za-z_][\w\s\*]*?\b(\w+)\s*"
                   r"\(((?:[^;{}()]|\([^()]*\))*)\)\s*(?:__asm__\s*\(\s*\"([^\"]*)\"\s*\))?\s*"
                   r"(?:__attribute__\s*\(\((?:[^()]|\([^()]*\))*\)\)\s*)?;", re.M)
CONDITIONAL = re.compile(r"^[ \t]*#[ \t]*(if|ifdef|ifndef|elif)\b(.*)$", re.M)
ALLOWED_MACROS = {"NON_MATCHING", "NOT_SDA", "MACRO_ADDR"}
REGISTER_NAME = re.compile(r"^(?:(?:temp|var|phi|saved_reg)_\w+|sp(?=[0-9A-Fa-f]*\d)[0-9A-Fa-f]+|arg\d+"
                           r"|(?:v[01]|a[0-3]|t\d|s[0-8]|f(?:\d|[12]\d|3[01])|fp|ra|at|k[01])(?:_\d+)?)$")
TYPE_WORDS = ("s8|u8|s16|u16|s32|u32|s64|u64|s128|u128|f32|f64|b32|vs8|vu8|vs16|vu16|vs32|vu32|"
              "int|char|short|long|float|double|void|unsigned|signed|size_t|\\w+_t|"
              "(?:struct|union|enum)\\s+\\w+")
DECL_LINE = re.compile(r"^\s*(?:(?:register|volatile|const|static|extern)\s+)*(?:" + TYPE_WORDS + r")\b"
                       r"(?:\s+(?:" + TYPE_WORDS + r")\b)*\s*([^;(){}]*?)\s*;", re.M)
# A byte array defined with a list of numbers: raw bytes, which stay in assembly
# (docs/contributing/code-cleanup.md, Data). Strings and typed tables are fine.
RAW_BYTES = re.compile(r"^(?:const\s+)?(?:u8|s8|(?:un)?signed\s+char|char)\s+(\w+)\s*\[[^\]\n]*\]"
                       r"(?:\s*__asm__\s*\(\s*\"([^\"]*)\"\s*\))?[^=;\n]*=\s*\{([^;]*?)\}\s*;", re.M)
RAW_BYTES_MAX = 16
# Code that only steers the compiler (docs/contributing/code-cleanup.md, Fakematches). Each use
# needs a `Fakematch:` comment that ends at most FAKEMATCH_REACH lines above it.
OPERAND = r"\*?[A-Za-z_][\w.\[\]]*(?:->[\w.\[\]]+)*"
STEP = r"(?:(" + OPERAND + r")\s*(\+\+|--)|(\+\+|--)\s*(" + OPERAND + r"))"
FAKEMATCHES = [
    ("x++; x--", re.compile(r"(?<![\w.\]>])" + STEP + r"\s*;\s*" + STEP + r"\s*;")),
    ("x = x", re.compile(r"(?:^|[;{}:)])[ \t]*((?:\*\s*)?\(?" + OPERAND + r"\)?)\s*=\s*"
                         r"((?:\*\s*)?\(?" + OPERAND + r"\)?)\s*;", re.M)),
    ("do { } while (0)", re.compile(r"\bdo\s*\{")),
    ("if (0)", re.compile(r"\b(?:if|while)\s*\(\s*0\s*\)")),
]
FAKEMATCH_REACH = 3
SNAKE = re.compile(r"^_*[a-z][a-z0-9_]*$")
PLACEHOLDER = re.compile(r"^(?:fun|func|FUN|sub|D|D_L\d\d|FUN_L\d\d|jtbl|text_gap|lbl)_[0-9A-Fa-f]{8}\w*?$")
HEXPART = re.compile(r"^(?=[0-9A-Fa-f]*\d)[0-9A-Fa-f]{6,8}$")
LEVEL_PART = re.compile(r"^[lL]\d\d$")
ALIAS_SUFFIX = re.compile(r"_(?:gp|lui|macro)$")
SDK_NAME = re.compile(r"^(?:_|sce|Sce|Sif|sif|Sbrk|isce|Pad|Mc|Cd|Lib)")

FORBIDDEN = [
    (re.compile(r"(?:claude\.ai|chatgpt\.com|chat\.openai\.com)/\S*(?:session|chat|share|c/)", re.I),
     "link to a private AI session"),
    # A person who gave the change (a Naming Desk issue author) may be credited as
    # `Co-authored-by: login <id+login@users.noreply.github.com>`; any other
    # co-author line is an agent trailer.
    (re.compile(r"^\s*(?:claude[-_ ]?session\s*:|co[-_ ]?authored[-_ ]?by\s*:"
                r"(?!\s*[\w-]+ <\d+\+[\w-]+@users\.noreply\.github\.com>\s*$))", re.I | re.M),
     "agent trailer"),
    (re.compile(r"generated (?:with|by) \[?claude", re.I), "AI tool footer"),
    (re.compile(r"(?:/home/(?!user\b)[\w.-]+/|/Users/[\w.-]+/|[A-Za-z]:\\\\?Users\\\\?|/mnt/[a-z]/)"),
     "local path"),
]


@dataclass(frozen=True)
class Problem:
    check: str
    path: str
    message: str
    line: int = 0
    ident: str = ""  # what old and new problems are matched on, when not the message

    def __str__(self):
        where = f"{self.path}:{self.line}" if self.line else self.path
        return f"{where}: [{self.check}] {self.message}"


# —— Helpers ——

def git(*args: str, cwd: Path = ROOT) -> str:
    return subprocess.run(["git", *args], cwd=cwd, check=True, stdout=subprocess.PIPE,
                          stderr=subprocess.PIPE, text=True, errors="replace").stdout


def strip_comments(text: str) -> str:
    """Blank out comments, keeping line numbers and string literals."""
    def blank(m):
        s = m.group(0)
        return s if s[0] in "\"'" else re.sub(r"[^\n]", " ", s)
    return re.sub(r'"(?:\\.|[^"\\\n])*"|\'(?:\\.|[^\'\\\n])*\'|/\*.*?\*/|//[^\n]*', blank, text, flags=re.S)


def line_of(text: str, pos: int) -> int:
    return text.count("\n", 0, pos) + 1


def read(root: Path, rel: str) -> str:
    try:
        return (root / rel).read_text(encoding="latin-1")
    except OSError:
        return ""


def c_files(root: Path):
    for top, pattern in (("src", "*.c"), ("include", "*.h"), ("src", "*.h")):
        for path in sorted((root / top).rglob(pattern)):
            yield path.relative_to(root).as_posix()


def yaml_units(root: Path) -> dict[str, str]:
    units = {}
    for m in re.finditer(r"^\s*-\s*\[\s*(0x[0-9A-Fa-f]+|\d+)\s*,\s*(\w+)\s*,\s*([^\],]+?)\s*[\],]",
                         read(root, YAML), re.M):
        units[m.group(3)] = m.group(2)
    return units


def is_boot_source(rel: str) -> bool:
    return rel.startswith("src/") and rel.endswith(".c") and not rel.startswith(("src/overlays/", "src/check/"))


def is_sdk(rel: str) -> bool:
    return "/sdk/" in rel or rel.startswith("src/sdk/")


def declarator_names(decls: str):
    depth, cur, out = 0, "", []
    for ch in decls:
        if ch in "([{":
            depth += 1
        elif ch in ")]}":
            depth -= 1
        if ch == "," and depth == 0:
            out.append(cur)
            cur = ""
        else:
            cur += ch
    out.append(cur)
    for part in out:
        part = part.split("=", 1)[0]
        part = re.sub(r"\[[^\]]*\]", "", part)
        m = re.search(r"\(\s*\*+\s*(\w+)\s*\)", part) or re.search(r"(\w+)\s*$", part.strip())
        if m:
            yield m.group(1)


def param_names(params: str):
    for p in declarator_names(params):
        if p not in ("void",) and not re.fullmatch(TYPE_WORDS, p):
            yield p


# —— Per-file checks (run on changed files) ——

def matching_code(text: str) -> str:
    """Blank out preprocessor lines and NON_MATCHING drafts, keeping line numbers."""
    out, stack, cont = [], [], False
    for line in text.split("\n"):
        s = line.strip()
        if cont or s.startswith("#"):
            cont = line.rstrip().endswith("\\")
            m = re.match(r"#\s*(ifdef|ifndef|if|elif|else|endif)\b(.*)", s)
            if m and m.group(1) in ("ifdef", "ifndef", "if"):
                draft = "NON_MATCHING" in m.group(2)
                stack.append([draft, draft and m.group(1) == "ifdef"])
            elif m and m.group(1) in ("else", "elif") and stack and stack[-1][0]:
                stack[-1][1] = not stack[-1][1]
            elif m and m.group(1) == "endif" and stack:
                stack.pop()
            out.append(" " * len(line))
        else:
            out.append(" " * len(line) if any(on for _, on in stack) else line)
    return "\n".join(out)


def check_fakematches(rel: str, raw: str, text: str) -> list[Problem]:
    code = matching_code(text)
    starts = [0] + [m.end() for m in re.finditer("\n", raw)]
    line = lambda pos: bisect.bisect_right(starts, pos)
    marks = [(line(m.start()), line(m.end())) for m in re.finditer(r"/\*.*?\*/|//[^\n]*", raw, re.S)
             if "Fakematch:" in m.group(0)]
    out = []
    for kind, pattern in FAKEMATCHES:
        for m in pattern.finditer(code):
            if kind == "x++; x--":
                first, second = m.group(1) or m.group(4), m.group(5) or m.group(8)
                if re.sub(r"\s", "", first) != re.sub(r"\s", "", second) or \
                        (m.group(2) or m.group(3)) == (m.group(6) or m.group(7)):
                    continue
            elif kind == "x = x":
                if re.sub(r"\s", "", m.group(1)) != re.sub(r"\s", "", m.group(2)):
                    continue
            elif kind == "do { } while (0)":
                depth, i = 0, m.end() - 1
                for i in range(m.end() - 1, len(code)):
                    depth += {"{": 1, "}": -1}.get(code[i], 0)
                    if depth == 0:
                        break
                if not re.match(r"\}\s*while\s*\(\s*0\s*\)\s*;", code[i:i + 40]):
                    continue
            elif m.group(0).startswith("while") and re.search(r"\}\s*$", code[max(0, m.start() - 20):m.start()]):
                continue  # the end of a do { } while (0)
            pos = m.start() + len(m.group(0)) - len(m.group(0).lstrip(";{}:) \t\n"))
            at = line(pos)
            if any(start <= at and at - end <= FAKEMATCH_REACH for start, end in marks):
                continue
            snippet = re.sub(r"\s+", " ", code[pos:m.end()]).strip()[:60]
            out.append(Problem("fakematch", rel, f"{kind}: `{snippet}` only steers the compiler; "
                               "say why in a `Fakematch:` comment just above it", at,
                               f"{kind} {re.sub(chr(32), '', snippet)}"))
    return out


def check_c_file(rel: str, raw: str) -> list[Problem]:
    out: list[Problem] = []
    text = strip_comments(raw)
    # Inline assembly other than a name label.
    for m in ASM_ANY.finditer("" if rel in ASM_HEADERS else text):
        label = ASM_LABEL.match(text, m.start())
        if m.group(1) or not label or not LABEL_STRING.fullmatch(label.group(1)):
            snippet = text[m.start():m.start() + 60].split("\n")[0].strip()
            out.append(Problem("inline-asm", rel, f"inline assembly other than a name label: {snippet}",
                               line_of(text, m.start())))
    # Preprocessor tricks.
    if rel.endswith(".c") and not is_sdk(rel):
        for m in CONDITIONAL.finditer(text):
            macros = set(re.findall(r"[A-Za-z_]\w*", m.group(2))) - {"defined"}
            if not macros or not macros <= ALLOWED_MACROS:
                out.append(Problem("ifdef", rel, f"#{m.group(1)}{m.group(2).rstrip()}: only NON_MATCHING "
                                   "splits are allowed", line_of(text, m.start())))
    # Raw bytes moved into C.
    for m in RAW_BYTES.finditer(text):
        count = len([v for v in m.group(3).split(",") if v.strip()])
        if count > RAW_BYTES_MAX:
            out.append(Problem("raw-bytes", rel, f"{m.group(1)} is {count} raw bytes in C: raw bytes and code stay "
                               "in assembly until their type is known (never guess an array's size)",
                               line_of(text, m.start()), f"raw bytes {m.group(1)}"))
    # C names bound to a label.
    names = [(m.group(1), m.group(3), m.start(), "function") for m in FUNC_DECL.finditer(text)]
    for m in DATA_DECL.finditer(text):
        names.append((m.group(1), m.group(3), m.start(), "global"))
    for name, label, pos, kind in names:
        line = line_of(text, pos)
        if re.fullmatch(FUN_LABEL, name):
            out.append(Problem("label-as-name", rel, f"C name {name}: keep FUN_<addr> only in the "
                               "__asm__ label (use fun_<addr> or a real name)", line))
            continue
        msg = address_in_name(ALIAS_SUFFIX.sub("", name))
        if msg:
            out.append(Problem("address-in-name", rel, f"{kind} {name} ({label}): {msg}", line))
    # Definitions and prototypes: names and parameters.
    labels = {m.group(1): m.group(3) for m in PROTO.finditer(text) if m.group(3)}
    for m in FUNC_DEF.finditer(text):
        name = m.group(1)
        if (is_boot_source(rel) and not is_sdk(rel) and not SNAKE.match(name)
                and not SDK_NAME.match(name) and not re.fullmatch(r"(?:FUN|fun)_[0-9A-Fa-f]{8}", name)):
            out.append(Problem("function-name", rel, f"function {name} is not lower_snake_case",
                               line_of(text, m.start())))
    for m in list(PROTO.finditer(text)) + list(FUNC_DEF.finditer(text)):
        name = m.group(1)
        for p in param_names(m.group(2)):
            if REGISTER_NAME.match(p):
                out.append(Problem("register-name", rel, f"parameter '{p}' of {name} is named after a "
                                   "register or a decompiler temp; name it by what it holds",
                                   line_of(text, m.start()), f"parameter {p} of {function_id(name, labels)}"))
    # Locals.
    for m in DECL_LINE.finditer(text):
        if not raw[m.start():m.end()].startswith((" ", "\t")):
            continue
        for name in declarator_names(m.group(1)):
            if REGISTER_NAME.match(name):
                out.append(Problem("register-name", rel, f"local '{name}' is named after a register or a "
                                   "decompiler temp; name it by what it holds", line_of(text, m.start())))
    if rel.endswith(".c") and not is_sdk(rel):
        out += check_fakematches(rel, raw, text)
    return out


def function_id(name: str, labels: dict[str, str]) -> str:
    """The same id for a function under any C name: its address when known."""
    s = labels.get(name, name)
    m = re.fullmatch(r"(?:fun|func|FUN)_([0-9A-Fa-f]{8})", s)
    return m.group(1).upper() if m else s


def address_in_name(name: str) -> str:
    if PLACEHOLDER.fullmatch(name):
        if not re.search(r"[0-9A-Fa-f]{8}_\w", name):
            return ""
        return "keep the plain label or give a real name; suffixes on a label go"
    parts = name.split("_")
    if any(HEXPART.match(p) for p in parts):
        return "drop the address from the name (use a name by use, or the plain placeholder)"
    if any(LEVEL_PART.match(p) for p in parts[1:]) and not name.startswith(("D_L", "FUN_L")):
        return "drop the level suffix from the name"
    return ""


def check_file_name(rel: str, text: str) -> list[Problem]:
    stem = Path(rel).stem
    if rel.startswith("src/overlays/"):
        ok = SNAKE.match(stem) or re.fullmatch(r"[0-9a-f]{8}", stem)
    else:
        ok = SNAKE.match(stem) or (is_sdk(rel) and re.fullmatch(r"\w+", stem))
    out = []
    if not ok:
        out.append(Problem("file-name", rel, f"file name {stem} is not lower_snake_case"))
    elif not rel.startswith("src/overlays/") and address_in_name(stem) and not re.fullmatch(r"fun_[0-9a-f]{8}", stem):
        out.append(Problem("file-name", rel, f"file name {stem}: {address_in_name(stem)}"))
    if is_boot_source(rel) and not is_sdk(rel) and not re.fullmatch(r"fun_[0-9a-f]{8}", stem):
        defs = [m.group(1) for m in FUNC_DEF.finditer(strip_comments(text))]
        if len(defs) == 1 and snake_case(defs[0]) != stem:
            out.append(Problem("file-name", rel, f"file holds {defs[0]} but is named {stem}.c"))
    return out


def snake_case(name: str) -> str:
    return re.sub(r"(?<=[a-z0-9])(?=[A-Z])|(?<=[A-Z])(?=[A-Z][a-z])", "_", name).lower()


def check_added_text(rel: str, text: str) -> list[Problem]:
    out = []
    for n, line in enumerate(text.splitlines(), 1):
        if WAIVER in line:
            continue
        for pattern, what in FORBIDDEN:
            m = pattern.search(line)
            if m:
                out.append(Problem("forbidden", rel, f"{what}: {m.group(0).strip()}", n))
    return out


# —— Whole-tree checks ——

class Tree:
    def __init__(self, root: Path):
        self.root = root
        self.files = {rel: read(root, rel) for rel in c_files(root)}
        self.code = {rel: strip_comments(t) for rel, t in self.files.items()}
        self.units = yaml_units(root)


def check_label_names(tree: Tree, every_name: bool = False) -> list[Problem]:
    """Every file calls a function by the name its own file defines.

    every_name lists every name of a label that already had several, so those stay old."""
    decls = collections.defaultdict(lambda: collections.defaultdict(set))
    defined = {}
    for rel, text in tree.code.items():
        local = {}
        for m in FUNC_DECL.finditer(text):
            label = m.group(3).upper()
            local.setdefault(m.group(1), label)
            decls[label][m.group(1)].add(rel)
        for m in FUNC_DEF.finditer(text):
            if m.group(1) in local:
                defined.setdefault(local[m.group(1)], (m.group(1), rel))
    out = []
    for label, names in decls.items():
        if len(names) < 2:
            continue
        if every_name:
            out += [Problem("label-names", rel, "", ident=f"{label} {name}")
                    for name, files in names.items() for rel in files]
            continue
        if label not in defined:
            continue
        canon, home = defined[label]
        for name, files in names.items():
            if name != canon:
                for rel in files:
                    out.append(Problem("label-names", rel, f"{label} is called {name} here but {canon} "
                                       f"in {home}; use one name", ident=f"{label} {name}"))
    return out


def check_header_duplicates(tree: Tree, every_local: bool = False) -> list[Problem]:
    """A global declared in a header is not declared again in a source file; no label is in two headers.

    every_local lists every local declaration, so one that was already there stays old."""
    in_headers = collections.defaultdict(set)
    local = collections.defaultdict(set)
    defined = collections.defaultdict(set)
    for rel, text in tree.code.items():
        header = rel.startswith("include/")
        if header:
            for m in FUNC_DECL.finditer(text):
                in_headers[m.group(3).upper()].add((rel, m.group(1)))
        for m in DATA_DECL.finditer(text):
            if ALIAS_SUFFIX.search(m.group(1)):
                continue
            start = text.rfind("\n", 0, m.start()) + 1
            stmt = text[start:m.start()]
            if header or re.search(r"\bextern\b", stmt):
                (in_headers if header else local)[m.group(3).upper()].add((rel, m.group(1)))
        for m in FUNC_DEF.finditer(text):
            defined[rel].add(m.group(1))
    out = []
    for label, where in in_headers.items():
        headers = sorted({rel for rel, _ in where})
        for extra in headers[1:]:
            out.append(Problem("header-duplicates", extra, f"{label} is declared in {headers[0]} too; "
                               "declare it in one header", ident=label))
        for rel, name in local.get(label, ()):
            if name not in defined[rel]:
                out.append(Problem("header-duplicates", rel, f"{label} ({name}) is declared in "
                                   f"{headers[0]}; include the header instead of a local declaration",
                                   ident=label))
    if every_local:
        out += [Problem("header-duplicates", rel, "", ident=label)
                for label, where in local.items() for rel, _ in where]
    return out


def check_data_decls(tree: Tree) -> dict[str, tuple[set, set]]:
    spec = importlib.util.spec_from_file_location("data_decls", SCRIPTS / "data-decls.py")
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return {label: (set(types), set(names)) for label, (types, names, _) in module.conflicts(tree.root).items()}


def check_symbol_addrs(tree: Tree) -> list[Problem]:
    """A boot global defined in C is no longer listed in symbol_addrs.txt."""
    listed = set(re.findall(r"^\s*(D_[0-9A-Fa-f]{8})\s*=", read(tree.root, "config/us/symbol_addrs.txt"), re.M))
    out = []
    for rel, text in tree.code.items():
        if not is_boot_source(rel):
            continue
        for m in re.finditer(r"^(?!extern\b)(?!\s)[^;{}()\n]*?\b(\w+)\s*(?:\[[^\]]*\]\s*)*"
                             r"(?:__asm__\s*\(\s*\"(D_[0-9A-Fa-f]{8})\"\s*\))?\s*(?:__attribute__\s*\(\(.*?\)\)\s*)?[=;]",
                             re.sub(r"__attribute__\s*\(\((?:[^()]|\([^()]*\))*\)\)", "", text), re.M):
            label = (m.group(2) or (m.group(1) if re.fullmatch(r"D_[0-9A-Fa-f]{8}", m.group(1)) else "")).upper()
            if label and label in {x.upper() for x in listed}:
                out.append(Problem("symbol-addrs", rel, f"{label} is defined in C; remove it from "
                                   "config/us/symbol_addrs.txt"))
    return out


def check_config_paths(tree: Tree) -> list[Problem]:
    """Source files and their config rows move together."""
    out = []
    sources = {rel for rel in tree.files if is_boot_source(rel)}
    for unit, kind in tree.units.items():
        if kind == "c" and f"src/{unit}.c" not in sources:
            out.append(Problem("config-paths", YAML, f"unit {unit} has no src/{unit}.c"))
    configured = {f"src/{u}.c" for u, k in tree.units.items() if k == "c"}
    included = set()
    for text in tree.code.values():
        included.update(re.findall(r'#\s*include\s*"([^"]+\.c)"', text))
    for rel in sorted(sources - configured):
        if Path(rel).name not in {Path(i).name for i in included}:
            out.append(Problem("config-paths", rel, f"file is not a unit in {YAML}"))
    tsv = read(tree.root, "config/overlays/us/functions.tsv")
    for row in tsv.splitlines():
        cols = row.split("\t")
        if row.startswith("#") or len(cols) < 7 or not cols[6].strip():
            continue
        if cols[6].strip() not in tree.units:
            out.append(Problem("config-paths", "config/overlays/us/functions.tsv",
                               f"{cols[0]}: exe unit {cols[6].strip()} is not in {YAML}"))
    return out


def route_exceptions(root: Path) -> dict:
    try:
        module = ast.parse(read(root, "configure.py"))
    except SyntaxError:
        return {}
    for node in module.body:
        if isinstance(node, ast.Assign) and any(getattr(t, "id", "") == "ROUTE_EXCEPTIONS" for t in node.targets):
            try:
                return ast.literal_eval(node.value)
            except ValueError:
                return {}
    return {}


def tree_problems(tree: Tree, base: bool = False) -> list[Problem]:
    return (check_label_names(tree, base) + check_header_duplicates(tree, base) + check_symbol_addrs(tree)
            + check_config_paths(tree))


# —— Commit and pull request text ——

def commit_validator():
    spec = importlib.util.spec_from_file_location("commit_msg", SCRIPTS / "commit-msg.py")
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module.validate


def check_message(where: str, text: str, subject: bool, validate) -> list[Problem]:
    out = []
    if subject:
        first = next((line for line in text.splitlines() if line.strip()), "")
        for msg in validate(first):
            out.append(Problem("commit-message", where, f"{msg}: '{first.strip()}'"))
    for pattern, what in FORBIDDEN:
        m = pattern.search(text)
        if m:
            out.append(Problem("forbidden", where, f"{what}: {m.group(0).strip()}"))
    return out


# —— Driver ——

def default_base() -> str:
    for ref in ("origin/main", "main"):
        try:
            return git("merge-base", ref, "HEAD").strip()
        except subprocess.CalledProcessError:
            continue
    raise SystemExit("check-cleanup: no main branch to compare with; pass --base")


def export(rev: str, dest: Path) -> None:
    present = set(git("ls-tree", "-r", "-t", "--name-only", "-z", rev).split("\0"))
    wanted = [p for p in TREE if p in present]
    archive = subprocess.run(["git", "archive", "--format=tar", rev, "--", *wanted], cwd=ROOT,
                             check=True, stdout=subprocess.PIPE).stdout
    path = dest / "base.tar"
    path.write_bytes(archive)
    with tarfile.open(path) as tar:
        tar.extractall(dest, filter="data")


def changes(base: str) -> tuple[dict[str, str | None], set[str]]:
    """Map each changed working-tree path to its base path (None when new), plus deleted paths."""
    changed: dict[str, str | None] = {}
    deleted = set()
    out = git("diff", "-M", "--name-status", "-z", base, "--")
    fields = out.split("\0")
    i = 0
    while i < len(fields) and fields[i]:
        status = fields[i]
        if status[0] in "RC":
            changed[fields[i + 2]] = fields[i + 1] if status[0] == "R" else None
            i += 3
            continue
        path = fields[i + 1]
        if status[0] == "D":
            deleted.add(path)
        else:
            changed[path] = path if status[0] == "M" else None
        i += 2
    for path in git("ls-files", "--others", "--exclude-standard", "-z").split("\0"):
        if path:
            changed[path] = None
    return changed, deleted


def file_problems(rel: str, text: str) -> list[Problem]:
    out = []
    if Path(rel).suffix in (".c", ".h") and rel.startswith(("src/", "include/")):
        out += check_c_file(rel, text)
    if rel.startswith("src/") and rel.endswith(".c"):
        out += check_file_name(rel, text)
    if Path(rel).suffix in TEXT_SUFFIXES:
        out += check_added_text(rel, text)
    return out


def new_problems(head: list[Problem], base: list[Problem], rename: dict[str, str] | None) -> list[Problem]:
    """Problems in head that base does not have; rename None compares without paths."""
    def key(p, path):
        what = p.ident or p.message
        return (p.check, what) if rename is None else (p.check, path, what)
    def unique(problems):  # a function declared in several places counts once
        seen, out = set(), []
        for p in problems:
            k = key(p, p.path)
            if not p.ident.startswith("parameter ") or k not in seen:
                seen.add(k)
                out.append(p)
        return out
    left = collections.Counter(key(p, p.path) for p in unique(base))
    out = []
    for p in unique(head):
        key_ = key(p, rename.get(p.path, p.path) if rename else p.path)
        if left[key_] > 0:
            left[key_] -= 1
        else:
            out.append(p)
    return out


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--base")
    ap.add_argument("--title")
    ap.add_argument("--body-file", type=Path)
    ap.add_argument("--list", action="store_true")
    args = ap.parse_args()

    head_tree = Tree(ROOT)
    if args.list:
        problems = tree_problems(head_tree)
        for rel in c_files(ROOT):
            problems += file_problems(rel, read(ROOT, rel))
        for p in problems:
            print(p)
        print(f"{len(problems)} problems", file=sys.stderr)
        return 0

    base = args.base or default_base()
    changed, deleted = changes(base)
    problems: list[Problem] = []
    asm_edits = [p for p in sorted(set(changed) | deleted) if p.startswith("asm/") or "/asm/" in p]
    problems += [Problem("asm-files", p, "files in asm/ are generated; do not edit them") for p in asm_edits]

    with tempfile.TemporaryDirectory() as tmp:
        base_root = Path(tmp)
        export(base, base_root)
        base_tree = Tree(base_root)
        rename = {new: old for new, old in changed.items() if old}

        # Per-file problems are pooled over all changed files, so code moved between files stays old.
        head, before = [], []
        for rel in sorted(changed):
            if Path(rel).suffix in TEXT_SUFFIXES:
                head += file_problems(rel, read(ROOT, rel))
        for old in sorted({old for old in changed.values() if old} | deleted):
            if Path(old).suffix in TEXT_SUFFIXES:
                before += file_problems(old, git("show", f"{base}:{old}"))
        problems += new_problems(head, before, None)
        problems += new_problems(tree_problems(head_tree), tree_problems(base_tree, True), rename)

        before, after = check_data_decls(base_tree), check_data_decls(head_tree)
        for label, (types, names) in after.items():
            old_types, old_names = before.get(label, (set(), set()))
            new_types, new_names = (types, names) if label not in before else (types - old_types, names - old_names)
            if new_types or new_names:
                problems.append(Problem("data-decls", "src", f"{label} gets another type or name "
                                        f"({', '.join(sorted(new_types | new_names))}); one name and one type per global, "
                                        "placement aliases end in _gp, _lui or _macro"))

        old_routes, new_routes = route_exceptions(base_root), route_exceptions(ROOT)
        for unit, rule in new_routes.items():
            moved = rename.get(f"src/{unit}.c", "")[4:-2]
            if old_routes.get(unit, old_routes.get(moved)) != rule:
                problems.append(Problem("compiler", "configure.py", f"ROUTE_EXCEPTIONS: {unit} -> {rule}: "
                                        "do not add or change a unit's compiler"))

    validate = commit_validator()
    for line in git("log", "--format=%H", f"{base}..HEAD").split():
        problems += check_message(f"commit {line[:10]}", git("log", "-1", "--format=%B", line), True, validate)
    if args.title is not None:
        problems += check_message("pull request title", args.title, True, validate)
    if args.body_file:
        problems += check_message("pull request description", args.body_file.read_text(errors="replace"),
                                  False, validate)

    for p in problems:
        print(p)
    if problems:
        print(f"\n{len(problems)} new problems (docs/contributing/code-cleanup.md)", file=sys.stderr)
        return 1
    print("cleanup check: no new problems")
    return 0


if __name__ == "__main__":
    sys.exit(main())
