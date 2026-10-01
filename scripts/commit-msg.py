#!/usr/bin/env python3
"""Git hook: validate the commit subject against docs/commit-messages.md.

Install into the repository (idempotent):

    ln -sf ../../scripts/commit-msg.py .git/hooks/commit-msg

The standard is one lowercase `<type>: <summary>` prefix per subject
(`decomp`, `overlay`, `docs`, `chore`, `fix`, `config`), with no repeated scope token.

The hook also rewrites the message file before validation: every
`co-authored-by:` and `claude-session:` trailer line (case-insensitive) is
dropped, so agent trailers and links to private AI sessions never enter the
history.  A message that still contains a link to an AI chat or agent session
anywhere else is rejected.  Messages without such lines are left untouched,
byte for byte.

Commits are authored and committed by the maintainer, under the same name and
e-mail as the existing history; the hook rejects an AI agent identity (an
anthropic.com or openai.com address) as author or committer.
"""

import importlib.util
import os
import re
import subprocess
import sys
from pathlib import Path

TYPES = {"decomp", "overlay", "docs", "chore", "fix", "config"}
SUBJECT_RE = re.compile(r"^(?P<type>[a-z]+): (?P<summary>\S.*)$")
NON_IMPERATIVE_RE = re.compile(
    r"^(?:added|updated|fixed|changed|removed|moved)\b", re.IGNORECASE
)

# Prefer the shared validator from a tooling checkout; fall back to the local rule.
NAMING = Path(os.environ.get("RNC_COMMIT_NAMING", ""))
problems: list[str] = []

# Trailer form only (line starts with the token + ":"): prose that merely
# mentions the term, e.g. in a commit about this hook, must survive.
TRAILER_RE = re.compile(r"^\s*(?:co[-_]?authored[-_]?by|claude[-_]?session)\s*:",
                        re.IGNORECASE | re.MULTILINE)
# Links to private AI chat/agent sessions must never be published.
SESSION_LINK_RE = re.compile(r"(?:claude\.ai|chatgpt\.com|chat\.openai\.com)/\S*(?:session|chat|share|c/)",
                             re.IGNORECASE)
# Author and committer must be the maintainer, never an AI agent identity.
AGENT_IDENT_RE = re.compile(r"<[^>]*@(?:[\w.-]+\.)?(?:anthropic|openai)\.com>", re.IGNORECASE)


def agent_identities() -> list[str]:
    found = []
    for role in ("GIT_AUTHOR_IDENT", "GIT_COMMITTER_IDENT"):
        ident = subprocess.run(["git", "var", role], stdout=subprocess.PIPE,
                               stderr=subprocess.DEVNULL, text=True).stdout
        if AGENT_IDENT_RE.search(ident):
            found.append(f"{role.split('_')[1].lower()} is an AI agent identity "
                         f"({ident.rsplit('>', 1)[0]}>); commit as the maintainer")
    return found


def strip_trailers(path: Path) -> bool:
    """Drop co-authored-by lines from the message file; True if it was rewritten.

    git lets this hook edit the message file.  Blank-line runs caused by the
    removal are collapsed and trailing blanks are dropped; a message that does
    not mention co-authored-by is never rewritten.
    """
    raw = path.read_text(errors="replace")
    if not TRAILER_RE.search(raw):
        return False
    kept = [line for line in raw.split("\n") if not TRAILER_RE.search(line)]
    out: list[str] = []
    blank = False
    for line in kept:
        if not line.strip():
            if blank:
                continue
            blank = True
        else:
            blank = False
        out.append(line)
    while out and not out[-1].strip():
        out.pop()
    path.write_text("\n".join(out) + ("\n" if out else ""))
    return True


def validate(subject: str) -> list[str]:
    found: list[str] = []
    # Subjects git writes itself (merges, reverts, autosquash) keep their form.
    if re.match(r'^(?:Merge |Revert "|fixup! |squash! |amend! )', subject.strip()):
        return found
    match = SUBJECT_RE.match(subject.strip())
    if not match:
        return ["expected '<type>: <summary>' with a lowercase type prefix"]
    kind, summary = match.group("type"), match.group("summary")
    if kind not in TYPES:
        found.append(f"unknown type '{kind}' (use one of: {', '.join(sorted(TYPES))})")
    if len(subject) > 120:
        found.append(f"subject is {len(subject)} characters (limit 120)")
    if summary[:1].isupper() and not summary.split(" ", 1)[0].isupper():
        found.append("summary should be lowercase")
    if NON_IMPERATIVE_RE.match(summary):
        found.append("summary should use the imperative mood (e.g. 'promote ...')")
    if re.match(r"^(?:decomp|overlay|docs|chore|fix|config):", summary):
        found.append("type prefix must not be repeated in the summary")
    if re.match(r"^[\w/.-]+:\s", summary):
        found.append("scope token (e.g. 'textbin:', 'sdk:') must be dropped")
    if summary.endswith("."):
        found.append("summary should not end with a period")
    return found


if NAMING.is_file():
    spec = importlib.util.spec_from_file_location("commit_naming", NAMING)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    validate = module.validate_subject

MSG = Path(sys.argv[1]) if len(sys.argv) > 1 else None
if MSG is not None and strip_trailers(MSG):
    print("commit-msg: dropped co-authored-by line(s) from the message", file=sys.stderr)
text = MSG.read_text(errors="replace") if MSG else ""
subject = ""
for line in text.splitlines():
    if line.strip() and not line.lstrip().startswith("#"):
        subject = line
        break
problems = validate(subject)
if SESSION_LINK_RE.search(text):
    problems.append("message links to a private AI session; remove the link")
if MSG is not None:
    problems += agent_identities()
if problems:
    for problem in problems:
        print(f"commit-msg: {problem}", file=sys.stderr)
    print(f"commit-msg: got '{subject.strip()}'", file=sys.stderr)
    print("commit-msg: see docs/commit-messages.md", file=sys.stderr)
    sys.exit(1)
