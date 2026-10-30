#!/usr/bin/env python3
"""FILE...: one blank line between top-level blocks of a C file: after the
include list, around every struct/union/enum/function body, and between a run
of declarations and the next different kind of item. Whitespace only."""
import re, sys

def kind(line):
    s = line.strip()
    if s.startswith('#include'): return 'include'
    if s.startswith('#'): return 'pp'
    if s.startswith(('/*', '*', '//')): return 'comment'
    return 'code'

def fix(text):
    lines = text.split('\n')
    out = []
    depth = 0
    prev = None          # kind of the last top-level item ('include', 'pp', 'decl', 'block')
    pending_comment = [] # comment lines waiting for the item they describe
    in_comment = False
    continued = False    # inside a multi-line #define
    for line in lines:
        s = line.strip()
        if continued:
            out.append(line)
            continued = line.rstrip().endswith('\\')
            continue
        if s.startswith('#') and not in_comment:
            continued = line.rstrip().endswith('\\')
            if depth > 0:
                out.append(line)
                continue
        if depth == 0 and not in_comment:
            if s == '':
                out.extend(pending_comment); pending_comment = []
                if out and out[-1].strip() != '': out.append('')
                continue
            k = kind(line)
            if k == 'comment' or (s.startswith('/*') and '*/' not in s):
                pending_comment.append(line)
                if s.startswith('/*') and '*/' not in s: in_comment = True
                continue
            if k == 'code':
                body = re.sub(r'"(?:\\.|[^"\\])*"', '""', line)
                body = re.sub(r'/\*.*?\*/|//.*', '', body)
                opens, closes = body.count('{'), body.count('}')
                cur = 'block' if opens > closes else 'decl'
            else:
                cur = k
            if prev is not None and (cur != prev or cur == 'block' or prev == 'block') \
                    and not (prev == 'pp' or cur == 'pp' and not s.startswith('#if')):
                if out and out[-1].strip() != '': out.append('')
            out.extend(pending_comment); pending_comment = []
            out.append(line)
            if k == 'code':
                depth += opens - closes
                prev = 'block' if depth > 0 else ('block' if opens else 'decl')
            else:
                prev = cur
            continue
        if in_comment:
            pending_comment.append(line) if depth == 0 else out.append(line)
            if '*/' in s: in_comment = False
            continue
        body = re.sub(r'"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'', '""', line)
        body = re.sub(r'/\*.*?\*/|//.*', '', body)
        if '/*' in body and '*/' not in body[body.index('/*'):]: in_comment = True; body = body[:body.index('/*')]
        depth += body.count('{') - body.count('}')
        out.append(line)
        if depth == 0: prev = 'block'
    out.extend(pending_comment)
    res = '\n'.join(out)
    return re.sub(r'\n{3,}', '\n\n', res)

if __name__ == "__main__":
    for f in sys.argv[1:]:
        t = open(f, encoding='latin-1').read()
        n = fix(t)
        if n != t:
            open(f, 'w', encoding='latin-1').write(n)
