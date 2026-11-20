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

Also drops register pins (`register T x __asm__("$f4")` keeps `register T x`).

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
  - "signatures": for functions with no C definition whose signature was decoded from their
    instructions, the canonical (returns, parameters) that unify_with_definitions treats as the
    definition, so mismatched declarations are rewritten or get reordering adapters.
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


# A local pinned to an EE register for the matching build (`register f32 x __asm__("$f4")`, a
# fakematch): the pin only chooses the register, so the copy drops it and keeps the variable.
REGISTER_PIN = re.compile(r'(\bregister\b[^;=(){}]*?)\s*__asm__\("\$\w+"\)')
def map_scratchpad(text):
    """0x7000xxxx literals -> 0x000Fxxxx, outside comments and strings, except where the value is
    not an address: OR-ed into a colour (`|= 0x70000000`), or stored whole as a DMA END tag
    (`*write = 0x70000000;`, `tag->tag = 0x70000000;`, vu1_send_chain_render_buffer.c and
    vu1_chain.c)."""
    parts = CODE_OR_SKIP.split(text)
    for index in range(0, len(parts), 2):
        code = parts[index]

        def replace(m):
            before = code[max(0, m.start() - 80):m.start()]
            line = before.split('\n')[-1]
            if re.search(r'\|=?\s*$', before):
                return m.group(0)
            if m.group(1) == '0000' and re.search(r'(^\s*\*\s*\w+|->tag|\.tag)\s*=\s*$', line):
                return m.group(0)
            return '0x000F' + m.group(1)
        parts[index] = SCRATCHPAD.sub(replace, code)
    return ''.join(parts)


SCRATCHPAD = re.compile(r'\b0x7000([0-3][0-9A-Fa-f]{3})\b')
SELF_ALIAS = re.compile(
    r'extern\b[^;{}]*?\b(\w+)\s*(?:\([^;{}]*?\))?\s*__attribute__\(\(alias\("\1"\)\)\);[ \t]*\n?')


BASIC = {'void', 'char', 'short', 'int', 'long', 'float', 'double', 'signed', 'unsigned',
         's8', 'u8', 's16', 'u16', 's32', 'u32', 's64', 'u64', 'f32', 'f64', 'const', 'volatile'}
FLOATS = {'float', 'double', 'f32', 'f64'}
NOT_A_TYPE = {'return', 'else', 'goto', 'case', 'do', 'sizeof', 'typedef', 'static', 'inline', 'if', 'while'}
DEFINITION = re.compile(r'^([A-Za-z_][\w \t\*]*?)\b(\w+)\(([^;{}()]*)\)\s*\{', re.M)
# The return type may also sit alone on the line after `extern`, with the name starting the next
# line, optionally indented (`extern void\nname(...)`), a layout the recovered tree uses; nothing looser.
DECLARATION = re.compile(
    r'^([ \t]*(?:extern[ \t]+)?)((?<=extern )[A-Za-z_][\w \t\*]*?\n[ \t]*|[A-Za-z_][\w \t\*]*?)\b(\w+)[ \t]*\(([^;{}()]*)\)'
    r'(\s*__asm__\("([^"]+)"\))?[ \t]*;',
    re.M)


def portable_type(text):
    """A parameter or return type reduced to something every file can name: pointers become
    `void *`; scalars must be built-in or types.h names. Returns None when it cannot be reduced."""
    if '[' in text and '*' not in text and len(text.split('[')[0].split()) < 2:
        return None  # `name[6]`: an array element in an expression, not a parameter
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


PORTED_HELPERS = ('browser/level-math.c', 'browser/spike/ee-helpers.c', 'browser/collision.c',
                  'browser/spike/services.c', 'browser/moby-animation-advance.c', 'browser/moby-joint-matrices.c')


def trailing_unused_parameters(text, match, parameters):
    """How many trailing parameters of the definition at `match` its body never names."""
    depth, at = 0, match.end() - 1
    while at < len(text):
        if text[at] == '{':
            depth += 1
        elif text[at] == '}':
            depth -= 1
            if depth == 0:
                break
        at += 1
    body = text[match.end():at]
    pieces = [piece.strip() for piece in parameters.split(',')] if parameters.strip() not in ('', 'void') else []
    count = 0
    for piece in reversed(pieces):
        last = re.search(r'(\w+)\s*$', piece)
        if not last or last.group(1) in BASIC or len(piece.split()) < 2 and not piece.endswith('*'):
            break  # unnamed: cannot tell
        if re.search(r'\b' + re.escape(last.group(1)) + r'\b', body):
            break
        count += 1
    return count


def call_argument_counts(text, name):
    """Argument counts of the calls `name(...)` in text (the declaration excluded: it ends in
    `;` right after its parentheses and has no expression arguments to count)."""
    counts = []
    for m in re.finditer(r'\b' + re.escape(name) + r'\s*\(', text):
        depth, at, commas, empty = 1, m.end(), 0, True
        while at < len(text) and depth:
            ch = text[at]
            if ch in '([{':
                depth += 1
            elif ch in ')]}':
                depth -= 1
            elif ch == ',' and depth == 1:
                commas += 1
            elif not ch.isspace() and depth >= 1:
                empty = False
            at += 1
        rest = text[at:at + 40].lstrip()
        if rest.startswith('__asm__') or (rest.startswith(';') and re.match(r'[\w \t\*]*$', text[max(0, m.start() - 60):m.start()].split('\n')[-1]) and 'extern' in text[max(0, m.start() - 60):m.start()].split('\n')[-1]):
            continue  # the declaration itself
        counts.append(0 if empty else commas + 1)
    return counts


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
    unused_tail = {}  # symbol -> how many trailing parameters its body never names
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
                canon[symbol] = (result, kinds, path, name)
                unused_tail[symbol] = trailing_unused_parameters(text, match, parameters)
    # Assembly-only functions ported as browser helpers (linked by link-closure.sh) define the
    # label too. Only non-static definitions bound to an original label count.
    for helper in PORTED_HELPERS:
        with open(os.path.join(ROOT, helper), encoding='utf-8') as handle:
            text = handle.read()
        for match in DEFINITION.finditer(text):
            returns, name, parameters = match.group(1).strip(), match.group(2), match.group(3)
            if not returns or returns.split()[0] in NOT_A_TYPE or 'static' in returns.split():
                continue
            label = re.search(r'\b' + re.escape(name) + r'\s*\([^;{}]*?\)\s*__asm__\("([^"]+)"\)', text)
            kinds, result = portable_parameters(parameters), portable_type(returns)
            if not label or kinds is None or result is None:
                continue
            canon.setdefault(label.group(1), (result, kinds, helper, name))  # a recovered definition wins
    decoded_signatures = set()
    # A function with no C definition can still have a known signature, decoded from its
    # instructions (source-fixes.json "signatures"). Declarations are then made to agree with it
    # the same way, reordering adapters included.
    with open(os.path.join(ROOT, 'browser', 'spike', 'source-fixes.json'), encoding='utf-8') as handle:
        for rule in json.load(handle).get('signatures', []):
            decoded_signatures.add(rule['label'])
            if rule['label'] in canon:
                sys.exit(f"signature rule for {rule['label']} names a function that has a C definition")
            canon[rule['label']] = (rule['returns'], portable_parameters(rule['parameters']), None, None)
    rewritten = 0
    disagreements = []

    def note(symbol, path, why):
        disagreements.append(f"{symbol}\t{os.path.relpath(path, OUT)}\t{' '.join(why.split())}")

    adapted = {}  # (path, C name) that already has an adapter in that file -> its parameter text

    def result_never_read(path, text, name):
        """True when every use of `name` in a .c file, other than its declarations, is a call that
        starts a statement, so no caller reads the value it is declared to return. Headers are
        excluded: the files that include them use the name."""
        if not path.endswith('.c'):
            return False
        declared_at = {m.start(3) for m in DECLARATION.finditer(text) if m.group(3) == name}
        for use in re.finditer(r'\b' + re.escape(name) + r'\b', text):
            if use.start() in declared_at:
                continue
            if not re.match(r'\s*\(', text[use.end():]):
                return False  # taken as a function pointer
            # The previous token, across lines: a statement starts after `{`, `}`, `;`, `else` or
            # a `(void)` cast. Anything else (an `=`, an operator, an `if (...)`) counts as a read.
            before = text[max(0, use.start() - 200):use.start()].rstrip()
            if before and not re.search(r'(?:[{};]|\belse|\(void\))$', before):
                return False
        return True

    def unused_unprototyped(path, text, name, parameters):
        """A `name()` declaration in a .c file whose name appears nowhere else in that file: no
        call depends on what it leaves unspecified, so it can take the definition's prototype.
        Headers are excluded: the files that include them use the name."""
        return path.endswith('.c') and parameters.strip() == '' and len(re.findall(r'\b' + re.escape(name) + r'\b', text)) == 1

    def reorder_adapter(path, text, lead, returns, name, parameters, declared, declared_result,
                        result, kinds, symbol, defined_as=None):
        """The EE passes integer and float arguments in separate register files, so a caller that
        lists the same arguments with the integers and floats interleaved differently from the
        definition still loads the right registers. WebAssembly arguments are positional. For a
        file-scope declaration in a .c file with the same number of integer and of float
        parameters as the definition, the declaration becomes a static inline function with the
        caller's parameter order that calls the definition with the n-th integer in the n-th
        integer position and the n-th float in the n-th float position. Returns None when that
        translation does not apply; the declaration then stays a listed disagreement."""
        if not path.endswith('.c') or lead.strip() not in ('', 'extern') or lead[:1] in (' ', '\t'):
            return None
        if len(declared) != len(kinds) or sorted(classes(declared)) != sorted(classes(kinds)):
            return None
        if declared_result is None:
            return None
        # A file that binds the same label under two C names would give one symbol two types in
        # one object, which links into a module that fails validation. Leave those listed.
        # Several names are fine when every declaration of each one also becomes an adapter (or
        # already has the definition's classes): all of them then call one extern with one type.
        bound = set(re.findall(r'\b(\w+)\s*\([^;{}()]*\)\s*__asm__\("' + re.escape(symbol) + r'"\)', text))
        for other in bound - {name, defined_as}:
            for match in DECLARATION.finditer(text):
                o_lead, o_returns, o_name, o_parameters, _, o_label = match.groups()
                if o_name != other or (o_label or o_name) != symbol:
                    continue
                if unused_unprototyped(path, text, o_name, o_parameters):
                    continue  # becomes the definition's prototype below
                o_declared = portable_parameters(o_parameters)
                o_result = portable_type(o_returns.strip())
                if o_declared is None or o_result is None or o_lead.strip() not in ('', 'extern') \
                        or o_lead[:1] in (' ', '\t') or len(o_declared) != len(kinds) \
                        or sorted(classes(o_declared)) != sorted(classes(kinds)):
                    return None
                if o_result != 'void' and (result == 'void' or classes([o_result]) != classes([result])):
                    return None
            if not any(match.group(3) == other for match in DECLARATION.finditer(text)):
                return None  # bound somewhere this pass cannot read (a definition, a cast): leave it
        if declared_result != 'void' and result == 'void' and result_never_read(path, text, name):
            declared_result = 'void'  # every call discards it
            returns = 'void '
        if declared_result != 'void' and (result == 'void' or classes([declared_result]) != classes([result])):
            return None  # the caller reads a register the definition does not set
        if (path, name) in adapted:
            return ''  # a repeated declaration in the same file: the first adapter serves
        pieces = [part.strip() for part in parameters.split(',')]
        names = []
        for index, piece in enumerate(pieces):
            last = re.search(r'(\w+)\s*$', piece)
            has_name = last and last.group(1) not in BASIC and not piece.rstrip().endswith('*') \
                and len(piece.split()) > 1
            if has_name:
                names.append(last.group(1))
            else:
                names.append(f'reordered_{index}')
                pieces[index] = f'{piece} reordered_{index}'
        order = {'i': [], 'f': []}
        for index, kind in enumerate(classes(declared)):
            order[kind].append(names[index])
        arguments = [order[kind].pop(0) for kind in classes(kinds)]
        adapted[(path, name)] = ' '.join(parameters.split())
        inner = f'{name}__definition_order'
        returned = returns.replace('extern', '').strip()
        call = f"{inner}({', '.join(arguments)})"
        # An adapter named after the label itself would be a local symbol with the label's name,
        # and every other declaration in the file bound to that label would resolve to it. It gets
        # its own name, and later uses of the label name are redirected to it.
        local = f'{name}__caller_order' if name == symbol else name
        redirect = f"\n#define {name} {local}" if local != name else ''
        return (f"extern {result} {inner}({', '.join(kinds)}) __asm__(\"{symbol}\");\n"
                f"static inline {returned} {local}({', '.join(pieces)}) "
                f"{{ {'' if declared_result == 'void' else 'return '}{call}; }}{redirect}")

    header_declared = {os.path.normpath(path): {m.group(3) for m in DECLARATION.finditer(text)}
                       for path, text in sources.items() if path.endswith('.h')}
    by_normal_path = {os.path.normpath(path): path for path in sources}
    included_cache = {}

    def included_names(path):
        """Names declared in the headers `path` includes, transitively (quoted includes resolved
        against the file's folder, then the tree's include/)."""
        if path in included_cache:
            return included_cache[path]
        included_cache[path] = set()
        names = set()
        for header in re.findall(r'#\s*include\s+"([^"]+)"', sources.get(path, '')):
            for base in (os.path.dirname(path), os.path.join(OUT, 'include')):
                candidate = os.path.normpath(os.path.join(base, header))
                if candidate in by_normal_path:
                    names |= header_declared.get(candidate, set()) | included_names(by_normal_path[candidate])
                    break
        included_cache[path] = names
        return names

    def count_adapter(path, text, lead, returns, name, parameters, declared, declared_result,
                      result, kinds, symbol):
        """A caller that passes a different number of arguments, in the definition's order for the
        ones both list. Extra trailing arguments are dropped: the EE callee never reads those
        registers. Missing trailing arguments are passed as 0, only when the definition's body
        never names those parameters (unused_tail). Anything else stays a listed mismatch."""
        if not path.endswith('.c') or lead.strip() not in ('', 'extern') or lead[:1] in (' ', '\t'):
            return None
        shared = min(len(declared), len(kinds))
        if classes(declared[:shared]) != classes(kinds[:shared]) or declared_result is None:
            return None
        if len(declared) < len(kinds) and unused_tail.get(symbol, 0) < len(kinds) - len(declared):
            return None
        if declared_result != 'void' and (result == 'void' or classes([declared_result]) != classes([result])):
            return None
        if name in included_names(path):
            return None  # a header may declare the name too: a static adapter would conflict
        # Other names bound to the same label (by asm label, or the label used as the name) are
        # fine when each of their declarations can take an adapter too: all of them then call one
        # extern of the definition's type.
        bound = set(re.findall(r'\b(\w+)\s*\([^;{}()]*\)\s*__asm__\("' + re.escape(symbol) + r'"\)', text))
        if name != symbol and re.search(r'\b' + re.escape(symbol) + r'\s*\(', text):
            bound.add(symbol)
        for other in bound - {name}:
            for m in DECLARATION.finditer(text):
                if m.group(3) != other:
                    continue
                o_declared = portable_parameters(m.group(4))
                if o_declared is None or m.group(1)[:1] in (' ', '\t'):
                    return None
                o_shared = min(len(o_declared), len(kinds))
                if classes(o_declared[:o_shared]) != classes(kinds[:o_shared]):
                    return None
                if len(o_declared) < len(kinds) and unused_tail.get(symbol, 0) < len(kinds) - len(o_declared):
                    return None
        if (path, name) in adapted:
            return ''
        pieces = [part.strip() for part in parameters.split(',')] if declared else []
        names = []
        for index, piece in enumerate(pieces):
            last = re.search(r'(\w+)\s*$', piece)
            if last and last.group(1) not in BASIC and not piece.rstrip().endswith('*') and len(piece.split()) > 1:
                names.append(last.group(1))
            else:
                names.append(f'counted_{index}')
                pieces[index] = f'{piece} counted_{index}'
        arguments = names[:shared] + ['0.0f' if kind in FLOATS else '0' for kind in kinds[len(declared):]]
        adapted[(path, name)] = ' '.join(parameters.split())
        inner = f'{name}__definition_count'
        local = f'{name}__caller_count' if name == symbol else name
        redirect = f"\n#define {name} {local}" if local != name else ''
        returned = returns.replace('extern', '').strip()
        call = f"{inner}({', '.join(arguments)})"
        return (f"extern {result} {inner}({', '.join(kinds) or 'void'}) __asm__(\"{symbol}\");\n"
                f"static inline {returned} {local}({', '.join(pieces) or 'void'}) "
                f"{{ {'' if declared_result == 'void' else 'return '}{call}; }}{redirect}")

    for path, text in sources.items():
        def replace(match):
            nonlocal rewritten
            lead, returns, name, parameters, asm, label = match.groups()
            if not returns.strip() or returns.split()[0] in NOT_A_TYPE:
                return match.group(0)
            symbol = label or name
            entry = canon.get(symbol)
            if not entry:
                return match.group(0)
            result, kinds, defined_in, defined_as = entry
            declared = portable_parameters(parameters)
            declared_result = portable_type(returns.strip())
            if defined_in == path:
                # The defining file's own prototypes stay. A second C name it binds to the same
                # label with the integers and floats in another order gets the reordering adapter:
                # left alone, clang emits a direct call whose signature does not match the body.
                if name == defined_as or declared is None or len(declared) != len(kinds) \
                        or classes(declared) == classes(kinds):
                    return match.group(0)
                adapter = reorder_adapter(path, text, lead, returns, name, parameters, declared,
                                          declared_result, result, kinds, symbol, defined_as)
                if adapter is None:
                    return match.group(0)
                rewritten += 1
                return adapter
            if declared is None and unused_unprototyped(path, text, name, parameters):
                rewritten += 1
                return f"{lead}{result} {name}({', '.join(kinds) or 'void'}){asm or ''};"
            if lead[:1] in (' ', '\t') and adapted.get((path, name)) == ' '.join(parameters.split()):
                # A block-scope repeat of a declaration the file already adapted at file scope: the
                # adapter is in scope, and an extern of the same name would conflict with it.
                rewritten += 1
                return f"{re.match(r'[ \t]*', lead).group(0)}/* {name}: file-scope adapter above */"
            if declared is None and parameters.strip() == '' and result == 'void' \
                    and declared_result not in (None, 'void') and result_never_read(path, text, name):
                # `()`: the calls still decide the parameters, but none reads the result the
                # definition does not return, so only the result changes.
                rewritten += 1
                return f"{lead}void {name}(){asm or ''};"
            if declared is None and parameters.strip() == '' and path.endswith('.c') and kinds \
                    and symbol in decoded_signatures \
                    and not any(kind in FLOATS for kind in kinds) and result not in FLOATS:
                # `()` for a function whose signature was decoded (source-fixes.json "signatures")
                # and takes only integers or pointers: the prototype converts nothing a caller
                # could mean differently, and a call with the wrong argument count stops compiling
                # instead of trapping. Applied to decoded signatures only: callers of recovered C
                # often pass extra arguments the EE ignores.
                rewritten += 1
                return f"{lead}{result} {name}({', '.join(kinds)}){asm or ''};"
            if declared is None and parameters.strip() == '' and kinds:
                # `()` whose every call in the file passes no argument: as a (void) declaration,
                # the argument-count adapter applies (zeros for parameters the body never names).
                uses = len(re.findall(r'\b' + re.escape(name) + r'\s*\(', text)) - 1
                empty = len(re.findall(r'\b' + re.escape(name) + r'\s*\(\s*\)', text)) - 1
                if uses > 0 and uses == empty:
                    adapter = count_adapter(path, text, lead, returns, name, 'void', [],
                                            portable_type(returns.strip()), result, kinds, symbol)
                    if adapter:
                        rewritten += 1
                        return adapter
            if declared is None and parameters.strip() == '' and not kinds and path.endswith('.c') \
                    and lead[:1] not in (' ', '\t') and name not in included_names(path) and (path, name) not in adapted:
                # `()` for a definition with no parameters, called with N arguments: the EE callee
                # never reads them, so an adapter takes N ignored integers.
                counts = set(call_argument_counts(text, name))
                if len(counts) == 1 and next(iter(counts)) > 0:
                    count = next(iter(counts))
                    adapted[(path, name)] = '()'
                    rewritten += 1
                    inner = f'{name}__definition_count'
                    local = f'{name}__caller_count' if name == symbol else name
                    redirect = f"\n#define {name} {local}" if local != name else ''
                    returned = returns.replace('extern', '').strip()
                    params = ', '.join(f'long long ignored_{i}' for i in range(count))
                    body = f"{'' if portable_type(returned) == 'void' else 'return '}{inner}();"
                    return (f"extern {result} {inner}(void) __asm__(\"{symbol}\");\n"
                            f"static inline {returned} {local}({params}) {{ {body} }}{redirect}")
            if declared is None and parameters.strip() == '' and kinds and path.endswith('.c') \
                    and not any(kind in FLOATS for kind in kinds) and lead[:1] not in (' ', '\t') \
                    and name not in included_names(path) and (path, name) not in adapted:
                # `()` whose every call passes at least the definition's (integer or pointer)
                # arguments: a variadic adapter forwards those and drops the rest, which the EE
                # callee never reads.
                counts = call_argument_counts(text, name)
                if counts and min(counts) >= len(kinds) and max(counts) > len(kinds):
                    adapted[(path, name)] = '()'
                    rewritten += 1
                    params = ', '.join(f'{kind} forwarded_{i}' for i, kind in enumerate(kinds))
                    inner = f'{name}__definition_count'
                    local = f'{name}__caller_count' if name == symbol else name
                    redirect = f"\n#define {name} {local}" if local != name else ''
                    returned = returns.replace('extern', '').strip()
                    call = f"{inner}({', '.join(f'forwarded_{i}' for i in range(len(kinds)))})"
                    return (f"extern {result} {inner}({', '.join(kinds)}) __asm__(\"{symbol}\");\n"
                            f"static inline {returned} {local}({params}, ...) "
                            f"{{ {'' if portable_type(returned) == 'void' else 'return '}{call}; }}{redirect}")
            if declared is None:  # `()` or types this pass cannot read: calls decide the signature
                if parameters.strip() == '':
                    note(symbol, path, f"declared with unspecified parameters; definition ({', '.join(kinds) or 'void'})")
                return match.group(0)
            if len(declared) != len(kinds) or classes(declared) != classes(kinds):
                adapter = reorder_adapter(path, text, lead, returns, name, parameters, declared,
                                          declared_result, result, kinds, symbol)
                if adapter is None and len(declared) != len(kinds):
                    adapter = count_adapter(path, text, lead, returns, name, parameters, declared,
                                            declared_result, result, kinds, symbol)
                if adapter is not None:
                    rewritten += 1
                    return adapter
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
                if result == 'void' and result_never_read(path, text, name):
                    new_returns = 'void '  # declared with a result, but every call discards it
                elif result == 'void':
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


def unify_missing_results():
    """A function with no C definition (an import the spike runner provides, defers or traps)
    links as one WebAssembly import, so every declaration must agree. When the prototyped
    declarations of such a label agree on their parameters and differ only in declaring an integer
    result or none, the `void` ones are declared `s32`: a caller that ignores the result is
    unaffected, and one import type remains. Labels with a C definition or float results are
    left to unify_with_definitions."""
    sources = {}
    for folder, _, names in os.walk(OUT):
        for name in names:
            path = os.path.join(folder, name)
            with open(path, encoding='utf-8', errors='surrogateescape') as handle:
                sources[path] = handle.read()
    defined_labels = set()
    declarations = {}
    for path, text in sources.items():
        if path.endswith('.c'):
            defined = {m.group(2) for m in DEFINITION.finditer(text)}
            for m in re.finditer(r'\b(\w+)\s*\([^;{}()]*\)\s*__asm__\("([^"]+)"\)', text):
                if m.group(1) in defined:
                    defined_labels.add(m.group(2))
            defined_labels |= defined
        for m in DECLARATION.finditer(text):
            lead, returns, name, parameters, asm, label = m.groups()
            if not returns.strip() or returns.split()[0] in NOT_A_TYPE:
                continue
            declarations.setdefault(label or name, []).append(
                (portable_parameters(parameters), portable_type(returns.strip())))
    with open(os.path.join(ROOT, 'browser', 'spike', 'source-fixes.json'), encoding='utf-8') as handle:
        fixed = {rule['label'] for rule in json.load(handle).get('signatures', [])}
    for helper in PORTED_HELPERS + ('browser/moby-animation-advance.c', 'browser/moby-joint-matrices.c'):
        with open(os.path.join(ROOT, helper), encoding='utf-8') as handle:
            fixed |= set(re.findall(r'__asm__\("([^"]+)"\)', handle.read()))
    widen = set()
    for label, found in declarations.items():
        if label in defined_labels or label in fixed:
            continue
        prototyped = [(kinds, result) for kinds, result in found if kinds is not None]
        results = {result for _, result in found}
        if not prototyped or None in results or len(results) < 2 or 'void' not in results:
            continue
        if any(wasm_type(result) != 'i32' for result in results if result != 'void'):
            continue
        shapes = {tuple(wasm_type(kind) for kind in kinds) for kinds, _ in prototyped}
        if len(shapes) == 1:
            widen.add(label)
    changed = 0
    for path, text in sources.items():
        def replace(m):
            nonlocal changed
            lead, returns, name, parameters, asm, label = m.groups()
            if (label or name) not in widen or portable_type(returns.strip()) != 'void':
                return m.group(0)
            changed += 1
            return f"{lead}s32 {name}({parameters}){asm or ''};"
        new = DECLARATION.sub(replace, text)
        if new != text:
            with open(path, 'w', encoding='utf-8', errors='surrogateescape') as handle:
                handle.write(new)
    print(f"functions without C whose declarations differed only in a void result: {len(widen)} "
          f"({changed} declarations now s32)")


def functions_declared_as_data():
    """A function label that one file declares as data only to take its address
    (`extern u8 D_0023D080[];` passed as a callback) links as two symbol kinds, which wasm-ld
    rejects. Labels defined as functions anywhere (a definition bound to the label, or a function
    alias `extern __typeof__(fn) LABEL __attribute__((alias(...)))`) have such array declarations
    rewritten to `extern void NAME(void)`, keeping any asm label. Uses that only take the address
    compile unchanged; a read through it would no longer compile, so none can slip through."""
    sources = {}
    for folder, _, names in os.walk(OUT):
        for name in names:
            path = os.path.join(folder, name)
            with open(path, encoding='utf-8', errors='surrogateescape') as handle:
                sources[path] = handle.read()
    functions = set()
    for path, text in sources.items():
        if not path.endswith('.c'):
            continue
        defined = {m.group(2) for m in DEFINITION.finditer(text)}
        for m in re.finditer(r'\b(\w+)\s*\([^;{}()]*\)\s*__asm__\("([^"]+)"\)', text):
            if m.group(1) in defined:
                functions.add(m.group(2))
        functions |= defined
        for m in re.finditer(r'__typeof__\((\w+)\)\s+(\w+)\s+__attribute__\(\(alias', text):
            if m.group(1) in defined:
                functions.add(m.group(2))
        for m in re.finditer(r'\b(\w+)\s*\([^;{}()]*\)\s*__attribute__\(\(alias', text):
            functions.add(m.group(1))
    pattern = re.compile(r'^([ \t]*extern[ \t]+)[A-Za-z_][\w \t]*?\b(\w+)\s*\[\s*\](\s*__asm__\("([^"]+)"\))?\s*;', re.M)
    rewritten = 0
    for path, text in sources.items():
        def replace(m):
            nonlocal rewritten
            label = m.group(4) or m.group(2)
            if label not in functions:
                return m.group(0)
            rewritten += 1
            return f"{m.group(1)}void {m.group(2)}(void){m.group(3) or ''};"
        new = pattern.sub(replace, text)
        if new != text:
            with open(path, 'w', encoding='utf-8', errors='surrogateescape') as handle:
                handle.write(new)
    print(f"function labels declared as data, now declared as functions: {rewritten}")


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
            if any(re.match(r'\s*\w+\s*\[', piece) for piece in parameters.split(',')):
                return match.group(0)  # `a * f(b[6]);` is an expression statement, not a declaration
            before = match.string[:match.start()].rstrip()
            if before and before[-1] in '=+-*/%&|^<>?:,(!~':
                return match.group(0)  # the previous line leaves an expression open: `x =\n a * f(b);`
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
    # alias("Name") targets the same symbol, so it is rewritten with it.
    named_alias = re.compile(r'alias\("(' + '|'.join(sorted(map(re.escape, by_name), key=len, reverse=True)) + r')"\)')
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
                new = named_alias.sub(lambda m: f'alias("{by_name[m.group(1)]}")', new)
                new, removed = SELF_ALIAS.subn('', new)
                new = widen_long(new)
                new = map_scratchpad(new)
                new = REGISTER_PIN.sub(r'\1', new)
                files += 1
                changed += new != text
                dropped += removed
                with open(os.path.join(target, name), 'w', encoding='utf-8', errors='surrogateescape') as handle:
                    handle.write(new)
    label_unlabelled_declarations()
    functions_declared_as_data()
    unify_missing_results()
    # Per-file fixes come before unify_with_definitions, so it sees the corrected prototypes.
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
    unify_with_definitions()
    for rule in fixes.get('empty_functions', []):
        total = 0
        for folder, _, names in os.walk(OUT):
            for name in names:
                path = os.path.join(folder, name)
                with open(path, encoding='utf-8', errors='surrogateescape') as handle:
                    text = handle.read()
                if rule['label'] not in text:
                    continue

                placed = set()

                def define(match, rule=rule, placed=placed):
                    lead, returns, name, parameters, asm, label = match.groups()
                    if (label or name) != rule['label'] or not returns.strip() \
                            or returns.split()[0] in NOT_A_TYPE:
                        return match.group(0)
                    if name in placed:  # a second declaration in one file: the first copy serves
                        return lead.replace('extern', '').rstrip()
                    placed.add(name)
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
