#!/usr/bin/env python3
"""Writes a build-time copy of src/ and include/ under build/spike/tree/ in which every
address-named function has one spelling. The recovered tree refers to the same function as
`FUN_001f9ab0` in one file and `func_001F9AB0` in another (two labels the PS2 linker script
maps to one address). ELF aliases cover that for the matching build; wasm objects need one name.
Only the copy is rewritten: src/ and include/ are read, never modified.

Rewrites: (FUN|func)_[Lnn_]XXXXXXXX -> FUN_[Lnn_]xxxxxxxx (lower-case hex), then drops alias
declarations that would now alias a function to itself.

Named labels get the same treatment: `__asm__("CalculateDmaTransferAddress")` is rewritten to the
address label config/us/symbol_addrs.txt gives that name (FUN_002335a0), because other files bind
the same function by address.

Also widens `long` to `long long`. The EE ABI's `long` is 64 bits and wasm32's is 32, so a
recovered `unsigned long` parameter or field would otherwise change size, layout and call
signature. `long long` and `long double` are left as they are; comments are not touched.

Also maps the scratchpad. The console has 16 KiB of fast RAM at 0x70000000-0x70003FFF, which the
game addresses with literals. Linear memory does not reach that address, so each such literal
becomes 0x000Fxxxx: the same offset inside 0x000F0000-0x000F3FFF, which is kernel memory on the
console (never touched by game code) and lies just below the pinned RAM image here.
link-closure.sh keeps the wasm stack below 0x80000, clear of it.

Then makes declarations agree with definitions (unify_with_definitions below): WebAssembly checks
a call's signature against the callee's, where the EE only cared which registers were loaded.

Then applies browser/spike/source-fixes.json, each entry with its reason:
  - "replacements": exact, per-file text replacements for constructs that depend on the EE calling
    convention or GCC 2.95 syntax. One whose text is not found exactly once stops the build, so a
    fix cannot silently stop applying.
  - "empty_functions": address-named functions whose retail body is empty (decoded with
    ee-disasm.py). Each declaration becomes a file-local empty definition with the prototype that
    file declares, because callers disagree on the arguments.
  - "prototypes": for functions with no C definition, one agreed declaration for a function that recovered callers declare
    inconsistently. Every `extern` declaration of that symbol in the copy is rewritten to it; a
    caller that no longer type-checks then fails to compile instead of trapping at run time.
"""
import json
import os
import re
import shutil
import sys

ROOT = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..')
OUT = os.path.join(ROOT, 'build', 'spike', 'tree')
LABEL = re.compile(r'\b(?:FUN|func)_(L\d\d_)?([0-9A-Fa-f]{8})\b')
# Both forms: `extern __typeof__(f) NAME __attribute__((alias("NAME")));` and
# `extern T NAME(params) __attribute__((alias("NAME")));`. No `;` may sit between the name and the
# attribute, so a match never spans two declarations.
# A bare `long` type word: not already `long long`, not `long double`. Applied outside comments
# and string literals only.
EE_LONG = re.compile(r'(?<!long )\blong\b(?!\s+(?:long|double)\b)')
CODE_OR_SKIP = re.compile(r'(/\*.*?\*/|//[^\n]*|"(?:\\.|[^"\\\n])*"|\'(?:\\.|[^\'\\\n])*\')', re.S)


def widen_long(text):
    parts = CODE_OR_SKIP.split(text)
    for index in range(0, len(parts), 2):  # even parts are code; odd parts are comments/literals
        parts[index] = EE_LONG.sub('long long', parts[index])
    return ''.join(parts)


SCRATCHPAD = re.compile(r'\b0x7000([0-3][0-9A-Fa-f]{3})\b')
SELF_ALIAS = re.compile(
    r'extern\b[^;{}]*?\b(\w+)\s*(?:\([^;{}]*?\))?\s*__attribute__\(\(alias\("\1"\)\)\);[ \t]*\n?')


BASIC = {'void', 'char', 'short', 'int', 'long', 'float', 'double', 'signed', 'unsigned',
         's8', 'u8', 's16', 'u16', 's32', 'u32', 's64', 'u64', 'f32', 'f64', 'const', 'volatile'}
FLOATS = {'float', 'double', 'f32', 'f64'}
NOT_A_TYPE = {'return', 'else', 'goto', 'case', 'do', 'sizeof', 'typedef', 'static', 'inline', 'if', 'while'}
DEFINITION = re.compile(r'^([A-Za-z_][\w \t\*]*?)\b(\w+)\(([^;{}()]*)\)\s*\{', re.M)
DECLARATION = re.compile(
    r'^([ \t]*(?:extern[ \t]+)?)([A-Za-z_][\w \t\*]*?)\b(\w+)[ \t]*\(([^;{}()]*)\)(\s*__asm__\("([^"]+)"\))?[ \t]*;',
    re.M)


def portable_type(text):
    """A parameter or return type reduced to something every file can name: pointers become
    `void *`; scalars must be built-in or types.h names. Returns None when it cannot be reduced."""
    if '*' in text or '[' in text:
        return 'void *'
    words = text.split()
    if len(words) > 1 and words[-1] not in BASIC:
        words = words[:-1]  # drop the parameter name
    if not words or any(word not in BASIC for word in words):
        return None
    return ' '.join(word for word in words if word not in ('const', 'volatile'))


def portable_parameters(text):
    text = text.strip()
    if text in ('', 'void'):
        return [] if text == 'void' else None  # `()` says nothing about the parameters
    types = [portable_type(part) for part in text.split(',')]
    return None if any(kind is None for kind in types) else types


def wasm_type(kind):
    if kind == 'void':
        return 'void'
    if kind in ('float', 'f32'):
        return 'f32'
    if kind in ('double', 'f64'):
        return 'f64'
    words = kind.split()
    if 's64' in words or 'u64' in words or words.count('long') >= 2:
        return 'i64'
    return 'i32'  # pointers and every narrower integer


def classes(types):
    return ''.join('f' if kind in FLOATS else 'i' for kind in types)


def unify_with_definitions():
    """Rewrites prototypes of a function, in files other than the one that defines it, to the
    definition's types, changing only parameters and results whose WebAssembly type differs.
    Skipped, and listed in build/spike/prototype-disagreements.txt, when the declaration has
    unspecified parameters, a different parameter count, a different integer/float order (the EE
    passes integers and floats in separate registers, so those callers pass arguments in an order
    that only a per-call fix can translate), or reads a result the definition does not declare."""
    sources = {}
    for folder, _, names in os.walk(OUT):
        for name in names:
            path = os.path.join(folder, name)
            with open(path, encoding='utf-8', errors='surrogateescape') as handle:
                sources[path] = handle.read()
    canon = {}
    for path, text in sources.items():
        if not path.endswith('.c'):
            continue
        for match in DEFINITION.finditer(text):
            returns, name, parameters = match.group(1).strip(), match.group(2), match.group(3)
            if not returns or returns.split()[0] in NOT_A_TYPE:
                continue
            label = re.search(r'\b' + re.escape(name) + r'\s*\([^;{}]*?\)\s*__asm__\("([^"]+)"\)', text)
            symbol = label.group(1) if label else name
            kinds = portable_parameters(parameters)
            result = portable_type(returns)
            if kinds is None or result is None:
                continue
            if symbol in canon and canon[symbol][:2] != (result, kinds):
                canon[symbol] = None  # defined differently in two files (per-level copies); leave alone
            elif symbol not in canon:
                canon[symbol] = (result, kinds, path)
    rewritten = 0
    disagreements = []

    def note(symbol, path, why):
        disagreements.append(f"{symbol}\t{os.path.relpath(path, OUT)}\t{' '.join(why.split())}")

    for path, text in sources.items():
        def replace(match):
            nonlocal rewritten
            lead, returns, name, parameters, asm, label = match.groups()
            if not returns.strip() or returns.split()[0] in NOT_A_TYPE:
                return match.group(0)
            symbol = label or name
            entry = canon.get(symbol)
            if not entry or entry[2] == path:
                return match.group(0)
            result, kinds, _ = entry
            declared = portable_parameters(parameters)
            declared_result = portable_type(returns.strip())
            if declared is None:  # `()` or types this pass cannot read: calls decide the signature
                if parameters.strip() == '':
                    note(symbol, path, f"declared with unspecified parameters; definition ({', '.join(kinds) or 'void'})")
                return match.group(0)
            if len(declared) != len(kinds) or classes(declared) != classes(kinds):
                note(symbol, path, f"({parameters}) vs definition ({', '.join(kinds) or 'void'})")
                return match.group(0)
            # Keep each declared parameter whose WebAssembly type already matches (so pointer
            # types stay as written); replace only the ones that would change the signature.
            pieces = [part.strip() for part in parameters.split(',')] if kinds else []
            for index, kind in enumerate(kinds):
                if wasm_type(declared[index]) != wasm_type(kind):
                    pieces[index] = kind
            new_returns = returns
            if declared_result is not None and wasm_type(declared_result) != wasm_type(result):
                if result == 'void':
                    # The caller reads a result the definition does not declare (a value left in
                    # $v0). Rewriting would not compile; it stays a link-reported mismatch.
                    note(symbol, path, f"declared to return {declared_result}; definition returns void")
                else:
                    new_returns = result + ' '
            new = f"{lead}{new_returns}{name}({', '.join(pieces) or 'void'}){asm or ''};"
            if ' '.join(new.split()) != ' '.join(match.group(0).split()):
                rewritten += 1
                return new
            return match.group(0)
        new_text = DECLARATION.sub(replace, text)
        if new_text != text:
            with open(path, 'w', encoding='utf-8', errors='surrogateescape') as handle:
                handle.write(new_text)
    with open(os.path.join(OUT, '..', 'prototype-disagreements.txt'), 'w', encoding='utf-8') as handle:
        handle.write('\n'.join(sorted(disagreements)) + '\n')
    known = sum(1 for entry in canon.values() if entry)
    print(f'prototypes: {known} definitions read, {rewritten} declarations rewritten to match, '
          f'{len(disagreements)} left for per-call fixes -> build/spike/prototype-disagreements.txt')


def label_unlabelled_declarations():
    """Some files declare a boot function by name only (`extern char *create_moby(int);`) where
    the rest of the tree binds that name to its address label (`__asm__("FUN_0020c4f8")`). The EE
    link resolves both through the linker script; here the name-only form would be a separate,
    undefined symbol. When a name is bound to exactly one label everywhere else, add that label to
    the name-only declarations. Files that define the name themselves are left alone."""
    labelled = re.compile(r'\b(\w+)\s*\([^;{}()]*\)\s*__asm__\("((?:FUN|D)_[^"]+)"\)')
    sources = {}
    labels = {}
    for folder, _, names in os.walk(OUT):
        for name in names:
            path = os.path.join(folder, name)
            with open(path, encoding='utf-8', errors='surrogateescape') as handle:
                sources[path] = handle.read()
            for match in labelled.finditer(sources[path]):
                labels.setdefault(match.group(1), set()).add(match.group(2))
    # Names whose definition carries no label (the symbol is the plain name): other files that
    # bind such a name to an address label get the label removed instead.
    plain = set()
    for path, text in sources.items():
        if not path.endswith('.c'):
            continue
        bound_here = {match.group(1) for match in labelled.finditer(text)}
        for match in DEFINITION.finditer(text):
            returns = match.group(1).strip()
            if returns and returns.split()[0] not in NOT_A_TYPE and match.group(2) not in bound_here:
                plain.add(match.group(2))
    unique = {name: next(iter(found)) for name, found in labels.items() if len(found) == 1 and name not in plain}
    added = 0
    removed = 0
    for path, text in sources.items():
        defined = {match.group(2) for match in DEFINITION.finditer(text)}

        def replace(match):
            nonlocal added
            nonlocal removed
            lead, returns, name, parameters, asm, _ = match.groups()
            if asm and name in plain and name not in defined and returns.strip() \
                    and returns.split()[0] not in NOT_A_TYPE:
                removed += 1
                return f'{lead}{returns}{name}({parameters});'
            if asm or name not in unique or name in defined:
                return match.group(0)
            if not returns.strip() or returns.split()[0] in NOT_A_TYPE:
                return match.group(0)
            if re.search(r'\b' + re.escape(name) + r'\s*\([^;{}()]*\)\s*__asm__', text):
                return match.group(0)  # this file already binds it
            added += 1
            return f'{lead}{returns}{name}({parameters}) __asm__("{unique[name]}");'
        new_text = DECLARATION.sub(replace, text)
        if new_text != text:
            with open(path, 'w', encoding='utf-8', errors='surrogateescape') as handle:
                handle.write(new_text)
    print(f'labels: {added} name-only declarations bound to their address label, '
          f'{removed} labels removed where the definition is by name')


def named_labels():
    """name -> FUN_xxxxxxxx for every named entry of config/us/symbol_addrs.txt."""
    table = {}
    with open(os.path.join(ROOT, 'config', 'us', 'symbol_addrs.txt'), encoding='utf-8') as handle:
        for line in handle:
            match = re.match(r'(\w+)\s*=\s*0x([0-9A-Fa-f]+);', line)
            if match and not re.match(r'(?:D|FUN|func|jtbl|jpt|lbl)_', match.group(1)):
                table[match.group(1)] = f'FUN_{int(match.group(2), 16):08x}'
    return table


def main():
    shutil.rmtree(OUT, ignore_errors=True)
    by_name = named_labels()
    named = re.compile(r'__asm__\("(' + '|'.join(sorted(map(re.escape, by_name), key=len, reverse=True)) + r')"\)')
    files = changed = dropped = 0
    for top in ('src', 'include'):
        for folder, _, names in os.walk(os.path.join(ROOT, top)):
            target = os.path.join(OUT, os.path.relpath(folder, ROOT))
            os.makedirs(target, exist_ok=True)
            for name in names:
                if not name.endswith(('.c', '.h', '.inc')):
                    continue
                with open(os.path.join(folder, name), encoding='utf-8', errors='surrogateescape') as handle:
                    text = handle.read()
                new = LABEL.sub(lambda m: f"FUN_{m.group(1) or ''}{m.group(2).lower()}", text)
                new = named.sub(lambda m: f'__asm__("{by_name[m.group(1)]}")', new)
                new, removed = SELF_ALIAS.subn('', new)
                new = widen_long(new)
                new = SCRATCHPAD.sub(r'0x000F\1', new)
                files += 1
                changed += new != text
                dropped += removed
                with open(os.path.join(target, name), 'w', encoding='utf-8', errors='surrogateescape') as handle:
                    handle.write(new)
    label_unlabelled_declarations()
    unify_with_definitions()
    with open(os.path.join(ROOT, 'browser', 'spike', 'source-fixes.json'), encoding='utf-8') as handle:
        fixes = json.load(handle)
    for fix in fixes['replacements']:
        path = os.path.join(OUT, fix['file'])
        with open(path, encoding='utf-8', errors='surrogateescape') as handle:
            text = handle.read()
        if text.count(fix['find']) != 1:
            sys.exit(f"source fix for {fix['file']} matched {text.count(fix['find'])} times, expected 1: {fix['why']}")
        with open(path, 'w', encoding='utf-8', errors='surrogateescape') as handle:
            handle.write(text.replace(fix['find'], fix['replace']))
    print(f"applied {len(fixes['replacements'])} source replacements")
    for rule in fixes.get('empty_functions', []):
        total = 0
        for folder, _, names in os.walk(OUT):
            for name in names:
                path = os.path.join(folder, name)
                with open(path, encoding='utf-8', errors='surrogateescape') as handle:
                    text = handle.read()
                if rule['label'] not in text:
                    continue

                def define(match, rule=rule):
                    lead, returns, name, parameters, asm, label = match.groups()
                    if (label or name) != rule['label'] or not returns.strip() \
                            or returns.split()[0] in NOT_A_TYPE:
                        return match.group(0)
                    pieces = [part.strip() for part in parameters.split(',')] if parameters.strip() not in ('', 'void') else []
                    named = []
                    for index, piece in enumerate(pieces):
                        last = re.search(r'(\w+)\s*$', piece)
                        has_name = last and last.group(1) not in BASIC and not piece.rstrip().endswith('*') \
                            and len(piece.split()) > 1
                        named.append(piece if has_name or piece == '...' else f'{piece} unused_{index}')
                    result = returns.replace('extern', '').strip()
                    body = '{}' if result == 'void' else f"{{ return {rule['result'] or '0'}; }}"
                    return f"{lead.replace('extern', '').rstrip()}static {result} {name}({', '.join(named) if pieces else parameters}) {body}"
                new = DECLARATION.sub(define, text)
                if new != text:
                    total += 1
                    with open(path, 'w', encoding='utf-8', errors='surrogateescape') as handle:
                        handle.write(new)
        print(f"{rule['label']}: empty definition placed in {total} files")
    for rule in fixes['prototypes']:
        # Declarations only: `extern <type> NAME(<params>);`, optionally with the same asm label.
        pattern = re.compile(
            r'^[ \t]*(?:extern[ \t]+)?(?!return\b)[A-Za-z_]\w*[\w \t]*?[ \t\*]+' + re.escape(rule['symbol'])
            + r'\s*\([^;{}()]*?\)(?:\s*__asm__\("[^"]*"\))?\s*;', re.M)
        label = f' __asm__("{rule["label"]}")' if rule.get('label') else ''
        canonical = f"extern {rule['returns']} {rule['symbol']}({rule['parameters']}){label};"
        total = 0
        for folder, _, names in os.walk(OUT):
            for name in names:
                path = os.path.join(folder, name)
                with open(path, encoding='utf-8', errors='surrogateescape') as handle:
                    text = handle.read()
                if rule['symbol'] not in text:
                    continue
                new, count = pattern.subn(canonical, text)
                if count:
                    total += count
                    with open(path, 'w', encoding='utf-8', errors='surrogateescape') as handle:
                        handle.write(new)
        if total == 0:
            sys.exit(f"prototype rule for {rule['symbol']} matched no declaration")
        print(f"unified {total} declarations of {rule['symbol']}")
    print(f'normalized copy: {files} files, {changed} rewritten, {dropped} self-aliases dropped -> build/spike/tree')


if __name__ == '__main__':
    sys.exit(main())
