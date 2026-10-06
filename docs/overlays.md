# Level code overlays

Each of the 19 levels ships its own build of the game program. When a level
loads, `ParseBin` copies seven records (`lit, bss, data, vtbl, camvtbl,
sndvtbl, text`) over the executable's `main` segment from 0x15EF00 up.
Everything below that (SDK, `core.*`, resident code) stays. `$gp` is
0x166C00 in every level. Distinct level code is about 3.2 MB and counts
toward C_EXACT ([progress-metrics.md](progress-metrics.md)).

How to contribute: [CONTRIBUTING.md](../CONTRIBUTING.md#level-overlays).

## Files

| Path | What | In Git |
| :--- | :--- | :--- |
| `config/overlays/us/functions.tsv` | the function catalogue (below) | yes |
| `config/overlays/us/level-NN.json`, `index.json`, `level-table.json` | where each level and record lies on the disc, with hashes | yes |
| `src/overlays/shared/`, `src/overlays/lNN/` | C, and `INCLUDE_ASM` stubs for functions not in C yet | yes |
| `config/us/overlays/level_NN/*.bin` | the records, cut from your disc image | **no** |
| `config/us/overlays/asm/*.s` | one listing per function, generated from the records | **no** |

`scripts/overlay-extract.py --iso <image>` (run by `./setup.sh --iso`) writes
the two gitignored rows. It checks every level against `level-NN.json`
first. Nothing from the disc is ever committed; CI fails if it is.

## Catalogue and names

`functions.tsv` lists every distinct function of the 19 programs once, with
its size and every `level:address` where it occurs. Two functions are the
same when their instructions agree with link-dependent fields (`jal`
targets, `lui`, `$gp` and memory offsets) masked.

| kind | meaning | name |
| :--- | :--- | :--- |
| `exe` | the same code as an executable function | its executable name; its C lives there |
| `shared` | in two or more levels | `FUN_LNN_xxxxxxxx` |
| `level` | in one level | `FUN_LNN_xxxxxxxx` |

`NN` and the address are the function's place in the lowest-numbered level
that has it. Level data is `D_LNN_XXXXXXXX` (0x15EF00 and up), jump tables
`jtbl_LNN_XXXXXXXX`; anything below 0x15EF00 keeps the executable's name.

## Sources and the proof

`src/overlays/` files follow link order, one file per run between two
executable units (about 32 KB), named after that unit and its first
function. A function is pending while its stub is the compiled code.
Retail hand asm (VU0/COP2, MMI, `addi`) is listed as `overlays/<name>`
under `intentional_asm` in `config/us/unit_categories.json` and stays out
of C_EXACT, as in the executable.

A function is exact when its C, compiled as retail game code was (game
compiler `cc1`, then `Ps2EeAs`) and placed at its address with every symbol
resolved to that level's address, equals the level's text byte for byte
(`scripts/overlay_proof.py`). Files in `OVERLAY_SN_UNITS` (compiler routes,
top of `configure.py`) build with SN ee-gcc 2.95.2 instead; list a file only
when every C function in it stays exact. `check-unit.py FUN_LNN_xxxxxxxx` runs this for one function; `make overlays` builds every file and runs it for every
function in C, ending with `PASS`.

## Progress

Every distinct function counts once: an `exe` copy counts with the
executable, a shared function once, not per level. The progress map shows
the executable as a drawer above a tree of shared code and the 19 levels;
the report has the categories `boot`, `shared`, `levels` and `level_NN`.
