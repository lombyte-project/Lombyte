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
   `python3 scripts/check-cleanup.py` lists the rules below that your change
   breaks. CI runs it on every pull request; older code on `main` does not
   count.
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

- add inline assembly (other than the name label); for `ei`, `di`, `sync`,
  `mfc0` or `syscall` use the macros in `include/kernel.h`;
- use `#ifdef` tricks;
- change which compiler a unit uses;
- edit files in `asm/`;
- fix bugs from the original game.

## Fakematches

A fakematch is code that only steers the compiler: `x++; x--;`, `x = x;`, a
`do { } while (0)` outside a macro, an `if (0)` branch, a dead store. It is
allowed until someone finds the real form, but it needs a comment that starts
with `Fakematch:` and says what it changes, ending at most three lines above
it. `check-cleanup.py` finds the first four shapes without that comment (not
dead stores or `x += 1; x -= 1;`, so mark those yourself). If
removing the trick does not change the code, it is not a fakematch: delete it.

## Names

- **Only name what you can prove.** Good evidence: recovered symbols
  ([recovered-names.md](recovered-names.md)), strings, callers, tables and
  nearby functions. A wrong name is worse than `fun_<addr>`.
- **Functions:** `lower_snake_case`, verb first (`draw_hud`), with a
  subsystem prefix when it helps (`hud_`, `save_card_`). Recovered CamelCase
  names become clear snake_case. Sony SDK names stay as they are. Keep
  `FUN_<addr>` only in the asm label, and use the same name the function's
  own file uses in every file that calls it.
- **Files:** a boot file is named after the function it holds; a level code
  file after the moby class or function it holds
  ([overlays.md](overlays.md#sources-and-the-proof)). Keep `fun_<addr>.c`
  until the name is proven.
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

## Data

Data in C has its bytes checked too (`make elf`, `make overlays`), so a wrong
edit fails at once.

| Change | Risk | When to check |
|---|---|---|
| C name of a global (the `__asm__("D_...")` label stays) | none | once per batch |
| How a value is written, same bytes (`enum`/`#define`, hex colour, `0` instead of `{0}` on a single value) | none | once per batch |
| Element type to a struct with the same layout (`float t[8]` → `struct { float u, v; } t[4]`) | low: the code that reads it changes | after each unit |
| Sign, size or `const` of a global | medium: reading code and section change | after each unit |
| Moving a definition (in its file or to another file) | medium: `$gp` or `lui` access in readers can change | after each unit |

- **One name, one type.** Each variable has one definition and one `extern`
  in a header. A file that needs another placement keeps one alias,
  `<name>_gp` (sda), `<name>_lui` (`NOT_SDA`) or `<name>_macro` (`MACRO_ADDR`).
  In an SN unit, functions above the first `sda` declaration keep `lui` when
  they read a `<name>_lui` alias declared before it.
  File suffixes (`_28efc8`, `_t`) and level prefixes on main-ELF data go.
  `scripts/data-decls.py` lists the labels that still disagree; fix the type
  when you move that data to C, and lower the count in `test_public_tools.py`.
- **Names** come from what the reading code does with the data
  (`camera_saved_states`, `panel_slot_enabled`). No evidence: the label is the
  name.
- **Copies in several levels** (`D_L05_X`, `D_L11_X`) share one type, and one
  name when the same code reads them.
- **Tables read as rows** become arrays of a struct with fields named by use.
- **Values say what they mean:** `enum`/`#define` for ids and flags, hex for
  colours and bit masks, floats exactly as `data-refs.py` prints them.
- **Where it lives:** data that only one file reads is defined after that
  file's functions and listed in `DATA_OVERLAYS` (`BSS_OVERLAYS` when
  uninitialised) in `configure.py`; level data is a row in
  `config/overlays/us/data.tsv`. `data-refs.py --emit ADDR --into FILE`
  (`--level N` for level data) writes both. Shared data and raw bytes stay in
  assembly, with their type in a header.
- **Never guess a size.** When `--emit` stops (no declaration, `T x[]` with no
  size, a type it cannot write), the data stays in assembly. Code or bytes
  written as a C array of numbers are not data in C; `check-cleanup.py` flags
  byte arrays of more than 16 values.
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
