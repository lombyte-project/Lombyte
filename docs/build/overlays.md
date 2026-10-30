# Level code (overlays)

Each of the 19 levels has its own copy of the game program. When a level
loads, `ParseBin` copies seven records (`lit, bss, data, vtbl, camvtbl,
sndvtbl, text`) over the executable's `main` segment, from 0x15EF00 up.
Everything below that (SDK, `core.*`, resident code) stays. `$gp` is 0x166C00
in every level. Level code is about 3.2 MB and counts toward C_EXACT
([progress.md](progress.md)).

How to work on it: [CONTRIBUTING.md](../../CONTRIBUTING.md#level-overlays).

## Files

| Path                                                     | What                                         | In Git |
| :------------------------------------------------------- | :------------------------------------------- | :----- |
| `config/overlays/us/functions.tsv`                       | list of every function (see below)           | yes    |
| `config/overlays/us/level-NN.json`, `index.json`, `level-table.json` | where each level and record is on the disc, with hashes | yes |
| `src/overlays/shared/`, `src/overlays/lNN/`              | C, and `INCLUDE_ASM` stubs for the rest      | yes    |
| `config/us/overlays/level_NN/*.bin`                      | the records, cut from your disc image        | **no** |
| `config/us/overlays/asm/*.s`                             | one listing per function, made from the records | **no** |

`scripts/overlay-extract.py --iso <image>` (run by `./setup.sh --iso`) makes
the two "no" rows. It checks every level against `level-NN.json` first.
Nothing from the disc is ever committed; CI fails if it is.

## Function list and names

`functions.tsv` lists every distinct function of the 19 levels once, with its
size and every `level:address` where it appears. Two functions count as the
same when their instructions match with link-dependent fields (`jal`
targets, `lui`, `$gp` and memory offsets) ignored.

| kind     | meaning                                | name                                  |
| :------- | :------------------------------------- | :------------------------------------ |
| `exe`    | same code as an executable function    | the executable's name; its C is there |
| `shared` | in two or more levels                  | `FUN_LNN_xxxxxxxx`                    |
| `level`  | in one level                           | `FUN_LNN_xxxxxxxx`                    |

`NN` and the address come from the lowest-numbered level that has it. Level
data is `D_LNN_XXXXXXXX` (0x15EF00 and up), jump tables
`jtbl_LNN_XXXXXXXX`. Anything below 0x15EF00 keeps the executable's name.

## Source files and the check

Files in `src/overlays/` follow link order, one file per stretch between two
executable units (about 32 KB). A new file is named
`<unit dir>/<first function address>.c`. Rename it after the moby class or
function it holds once you know it (`lNN/gameplay/entities/sand_shark.c`);
`decomp overlays stubs` keeps the new name.

A function is exact when its C, compiled like retail (game compiler `cc1`,
then `Ps2EeAs`) and placed at its address with every symbol resolved for that
level, equals the level's code byte for byte (`scripts/overlay_proof.py`).
Files in `OVERLAY_SN_UNITS` (top of `configure.py`) use SN's preprocessor and
SN's `cc1` rebuilt from source
([patches/sn-2.95.2](../../patches/sn-2.95.2/README.md)) instead; add a file
there only when every C function in it stays exact.

- `check-unit.py FUN_LNN_xxxxxxxx` checks one function.
- `make overlays` builds every file, checks every function in C and ends with
  `PASS`.

Hand-written code (VU0/COP2, MMI, `addi`) is listed as `overlays/<name>`
under `intentional_asm` in `config/us/unit_categories.json` and stays out of
C_EXACT, like in the executable.

Level data in C has its `D_LNN_XXXXXXXX` label and a row (name, size) in
`config/overlays/us/data.tsv`. `scripts/data-refs.py --level N --emit ADDR
--into FILE` writes both. `make overlays` compares its bytes, with pointers
resolved for that level, to what the level loads there. A row named
`D_XXXXXXXX` (no level) is data that every level has at that address, like
the zero globals in `src/overlays/shared/data/level_globals.c`; it is checked
and counted in every level. A pointer to a function the level holds more than
once must name the copy the level's word points to.

Each level's `lvl.vtbl`, `lvl.camvtbl` and `lvl.sndvtbl` sections (the moby
classes, camera types and sound emitter types the executable looks ids up in)
are in `src/overlays/lNN/level_class_tables.c`.

The compiler's own data for a function in C, its switch tables and float
literals, is checked too: each table word must point into the function at the
place the level's table holds, and each literal must equal the level's bytes.
`config/overlays/us/compiler_data.tsv` lists these places (level, address,
size); `make overlays` fails when it is out of date, and
`scripts/overlay_unit.py --write` rewrites it.

A level program also holds a copy of the executable's data, moved to other
addresses. Where the executable's code reads a data object at A and the same
function's copy in a level reads A + d, the level's bytes at A + d must equal
the executable's (pointers moved the same way).
`config/overlays/us/exe_data.tsv` lists the copies of executable data in C
that pass (`scripts/overlay_exe_data.py`), so data put in C in the executable
counts in every level that holds it.

## Progress

Every distinct function counts once: an `exe` copy counts with the
executable, a shared function once, not once per level. The report has the
categories `boot`, `shared`, `levels` and `level_NN`.
