# Cleaning up exact code

How to make matching C easier to read. Decompiling itself is covered in
[CONTRIBUTING.md](../CONTRIBUTING.md).

**Rule: the build stays byte-exact after every step.** Cleanup never changes
behaviour.

## What can change

| Change | Risk | Check |
|---|---|---|
| Comments, formatting, local and parameter names | none | per batch |
| Function and global names (C name + `__asm__` label) | none | per batch |
| Moving a unit to a better directory | none if config follows | per batch |
| `#define` / `enum` for magic numbers (same type) | none | per batch |
| Offset access → struct field | low: width and signedness must match | per function |
| Shared prototypes instead of local `extern` | low: wrong types change promotion | per function |
| Casts and integer types | medium | per function |
| Logic shape (expression order, temporaries, loops, `if`/`switch`) | high | per function, expect to revert |

Do the safe changes everywhere. Touch logic only where code is unreadable,
and revert as soon as it stops matching; a comment explaining the odd shape
is the fallback.

Never: inline assembly beyond the name label, `#ifdef` tricks, changing a
unit's compiler, editing `asm/`, or fixing bugs in the original.

## Naming

- Names need evidence: recovered symbols
  ([recovered-names.md](recovered-names.md)), strings, call sites, tables,
  neighbours. No evidence, no name; a wrong name is worse than `fun_<addr>`.
- Functions: `lower_snake_case`, verb-object, subsystem prefix where it helps
  (`hud_`, `save_card_`). Recovered CamelCase symbols become descriptive
  snake_case; Sony SDK names stay. Keep `FUN_<addr>` only in the asm label.
- Rename with the tool, one entry at a time, so overlay call sites follow:
  `python3 scripts/rename-function-proposals.py --address 0x... --apply`.
- Globals: `D_<addr>` gets a C name plus `__asm__("D_<addr>")` label, spelled
  one way and declared once in a header.
- Locals and fields: name by use (`count`, `next`), not by register or offset
  (`v1`, `unk_70`). Keep `unkXXX` only while nothing shows the meaning.

## Types and data

- Placeholder types become one named struct. A layout shared by several files
  lives in `include/`; a layout used by one file stays in that file. Extend
  existing headers instead of adding local copies.
- To add a struct: collect every offset used on that base across `src/`, add
  fields with padding that keeps exact offsets, convert one function, check,
  then the rest. If a field breaks the match, try the width and signedness the
  assembly loads (`lw`/`lwu`, `lh`/`lhu`).
- Group fields by purpose, name unused ones `pad_XXX[n]`, and put offset
  checks in `src/check/*_layout_check.c` (run by `make check`).
- Data: one definition per variable, one `extern` in a header. Data only one
  unit reads is defined in that file and listed in `DATA_OVERLAYS` in
  `configure.py` (`python3 scripts/data-refs.py --emit ADDR --into FILE`).
  Shared data and raw bytes stay in assembly, typed in a header. A variable
  defined in C leaves `config/us/symbol_addrs.txt`.

## Moving files

Directories follow the subsystem (`ui/menus/<screen>`, `gameplay/...`). One
commit updates the unit path in `config/us/rnc1.us.yaml` (or
`config/overlays/us/functions.tsv` for level code). Link order comes from
config, so a correct move cannot change the output.

## Workflow

1. Pick one area (a directory or a struct) and branch from latest `main`.
2. Apply the safe changes to the whole area.
3. After each risky change, `python3 scripts/check-unit.py <unit>` must still
   match.
4. `make elf` and `make overlays` must both end with `PASS`. If not, bisect
   and revert.
5. Commit per area, saying what became clearer.

## Pitfalls

- Two C names for one symbol in a file can matter: the compiler may reload
  through the second one. Merge aliases only if the unit still matches.
- `char *p = (char *)&g; *(T *)(p + 0x10)` becomes `struct X *p = &g; p->field`,
  not `g.field`, which changes the address calculation.
- Splitting a large file by screen or subsystem is a move, not a rewrite.
