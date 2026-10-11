# Code cleanup

This guide is for C that already matches the original game. It explains how
to make that code easier to read. To decompile new code, see
[CONTRIBUTING.md](../CONTRIBUTING.md).

> **The one rule:** after every change, the build must still produce exactly
> the same bytes. Cleanup never changes what the game does.

## Quick start

1. Make a branch from the latest `main`.
2. Pick one area, such as a folder or a struct.
3. Make the safe changes (see the table below) across the whole area.
4. After each risky change, run `python3 scripts/check-unit.py <unit>`. It
   must still say the unit matches.
5. Run `make elf` and `make overlays`. Both must end with `PASS`. If one
   fails, find the change that broke it and undo it.
6. Commit each area on its own and say what became easier to read.

## What you can change

| Change | Risk | When to check |
|---|---|---|
| Comments, formatting, names of locals and parameters | none | once per batch |
| Names of functions and globals (C name + `__asm__` label) | none | once per batch |
| Moving a file to a better folder (update config too) | none | once per batch |
| `#define` / `enum` for magic numbers (keep the same type) | none | once per batch |
| Offset access → struct field | low: same size and sign needed | after each function |
| Shared prototypes in a header instead of a local `extern` | low: wrong types change the code | after each function |
| Casts and integer types | medium | after each function |
| Code shape: order of expressions, temporary variables, loops, `if`/`switch` | high | after each function; expect to undo it |

Make the safe changes everywhere. Change the code shape only when the code is
really hard to read, and undo it as soon as it stops matching. If you can't
make it clearer, add a comment that explains why it looks odd.

**Never:**

- add inline assembly (other than the name label);
- use `#ifdef` tricks;
- change which compiler a unit uses;
- edit files in `asm/`;
- fix bugs from the original game.

## Names

- **Only name what you can prove.** Good evidence: recovered symbols
  ([recovered-names.md](recovered-names.md)), strings, callers, tables and
  nearby functions. A wrong name is worse than `fun_<addr>`.
- **Functions:** `lower_snake_case`, verb first (`draw_hud`), with a
  subsystem prefix when it helps (`hud_`, `save_card_`). Recovered CamelCase
  names become clear snake_case. Sony SDK names stay as they are. Keep
  `FUN_<addr>` only in the asm label.
- **Renaming:** use the tool, one function at a time, so callers in level
  code are updated too:
  `python3 scripts/rename-function-proposals.py --address 0x... --apply`
- **Globals:** `D_<addr>` gets a C name plus a `__asm__("D_<addr>")` label.
  Use one spelling and declare it once, in a header.
- **Locals and fields:** name them by what they hold (`count`, `next`), not by
  register or offset (`v1`, `unk_70`). Keep `unkXXX` only until the meaning is
  clear.

## Structs and data

**Structs**

- Replace placeholder types with one named struct.
- A struct used by several files goes in `include/`. A struct used by one file
  stays in that file.
- Extend an existing header instead of copying a struct.

**Adding a struct, step by step**

1. Find every offset used on that pointer across `src/`.
2. Add the fields, with padding so every offset stays the same.
3. Convert one function and check it. Then convert the rest.
4. If a field breaks the match, use the size and sign the assembly loads
   (`lw` or `lwu`, `lh` or `lhu`).

**Making headers easy to read**

- Group fields by what they are for.
- Name unused fields `pad_XXX[n]`.
- Put offset checks in `src/check/*_layout_check.c`. `make check` runs them.

**Data**

- Each variable has one definition and one `extern` in a header.
- Data that only one file reads is defined in that file and listed in
  `DATA_OVERLAYS` in `configure.py`. This command writes it for you:
  `python3 scripts/data-refs.py --emit ADDR --into FILE`
- Shared data and raw bytes stay in assembly, with their type in a header.
- A variable defined in C is removed from `config/us/symbol_addrs.txt`.

## Moving files

- Folders follow the subsystem, for example `ui/menus/<screen>` or
  `gameplay/...`.
- In the same commit, update the unit path in `config/us/rnc1.us.yaml`, or in
  `config/overlays/us/functions.tsv` for level code.
- The config decides the link order, so a correct move can't change the
  output.

## Common traps

- **Two names for one symbol** in a file can matter: the compiler may load it
  again through the second name. Merge them only if the unit still matches.
- **Pointer to a global:** turn `char *p = (char *)&g; *(T *)(p + 0x10)` into
  `struct X *p = &g; p->field`, not `g.field`. Writing `g.field` changes how
  the address is computed.
- **Splitting a big file** by screen or subsystem counts as a move, not a
  rewrite.
