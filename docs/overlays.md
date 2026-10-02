# Level code overlays

Each of the 19 levels carries its own build of the game program. When a level
loads, `ParseBin` copies seven records over the executable's whole `main`
segment: `lit, bss, data, vtbl, camvtbl, sndvtbl, text`. The executable's game
code is a subset of every level's program, in the same link order; the rest is
per-level code (enemies, bosses, level logic, the `update/*` and `hero*`
modules). All distinct level code is about 3.2 MB, nine times the executable's
game code, and it is part of the C_EXACT goal ([progress-metrics.md](progress-metrics.md)).

The method is rac1-decomp's (`docs/OVERLAYS.md` there, for the PAL build); this
page records what was measured on the US records and how the layer is laid out
in this repository.

## Data

The records are game data and are never committed here. They are cut from a
legally owned US disc image by the tooling repository's `scripts/overlay-dump.py`
(code only, no assets, 33.6 MB for all 19 levels) and restored into the
gitignored `config/us/overlays/level_NN/` by the tooling setup, with the
generated assembly under `config/us/overlays/asm/`. This repository holds
only what carries no bytes:

| File | What |
| :--- | :--- |
| `config/overlays/us/level-table.json` | where each level lies in the ISO |
| `config/overlays/us/level-NN.json`, `index.json` | the records of each level program: address, size, type, entry point |
| `config/overlays/us/functions.tsv` | the function catalogue (below) |
| `config/overlays/us/confirmed-starts.tsv` | function starts confirmed by hand, for the ones no rule finds |
| `config/overlays/us/levels.json` | planet name and description per level (progress map) |
| `src/overlays/shared/`, `src/overlays/lNN/` | the C, and `INCLUDE_ASM` stubs for functions not yet in C |

## Facts measured on the US records

`scripts/decomp overlays facts` in the tooling repository prints these from
the records and the catalogue.

- `lit` starts at 0x15EF00 in every level; the executable's `.lit` starts at
  0x15EF00 too, and the resident `core.lit` ends at 0x15EEE0. Everything from
  0x15EF00 up is replaced by the level; everything below (the SDK, `core.*`,
  the executable's resident code and data) stays and is shared by every level.
  PAL's boundary is 0x15F000: every US constant is 0x100 lower.
- Record order `lit, bss, data, vtbl, camvtbl, sndvtbl, text`, types
  `1, 8, 1, 1, 1, 1, 1`, in every level. The text record is 1,065,136 to
  1,200,936 bytes per level; the executable's game `.text` is 345,696 bytes.
- `$gp` is the executable's, 0x166C00, in every level. No level text contains
  an `addiu $gp, $gp, imm`: the value the executable's startup sets stays.
  Level copies of executable functions that reach resident memory (below
  0x15EF00) through `$gp` agree with the executable word for word at those
  instructions; the few that differ are masked-fingerprint collisions (small
  functions that load different `core.lit` constants), not a different `$gp`.
  Level code's `$gp` offsets span -0x7EA8..0x7BDC, from `core.lit` into the
  level's `data` record.
- Dispatch records: `vtbl` 12-byte entries `{oClass, update, table}` (100 to
  189 classes per level), `camvtbl` 20-byte entries `{id, init, activate,
  update, exit}` (6 to 9), `sndvtbl` 8-byte entries `{id, function}` (0 to 6);
  every list ends at an id of -1. Every table pointer lands on a catalogued
  function start.
- Every level's entry point (0x245C28 for level 00 .. 0x247F10 for level 18)
  is a catalogued shared function: the level initialiser.

## The catalogue and the names

`config/overlays/us/functions.tsv` lists every distinct function of the 19
level programs once. Two functions are the same function when their
instructions agree with the link-dependent fields masked: `j`/`jal` targets,
`lui` values, `$gp` offsets and non-stack memory offsets. Each row has a kind:

| kind | meaning |
| :--- | :--- |
| `exe` | the same code as an executable function; keeps its `FUN_xxxxxxxx` name, its C lives where the executable's does |
| `shared` | in two or more levels |
| `level` | in one level only |

### What makes a place a function start

A row is a function, so its start has to be an entry point of the retail code,
not a place the text happens to look like one. The old rule cut after every
`jr $ra` and at every frame opener, which turned a shared epilogue, a loop body
or the tail of a function into a row of its own, and a fragment cannot be
written in C because it never was a function.
Now a start needs one of:

- a `jal` that targets it;
- a dispatch record that names it (`vtbl`, `camvtbl`, `sndvtbl`);
- a pointer in a table the code calls through with `jalr` (a memory-card state
  handler table is 25 pointers the dispatcher indexes; a `switch` table is
  entered with `jr`, and its entries are case labels inside one function, so
  they are not starts);
- a callback address the code builds itself and keeps (`lui` plus `addiu`,
  passed as an argument or stored; `FUN_L00_002377b8` is reached no other way);
- a `j` from another function (a tail call);
- code right after a return (and its delay slot and padding) that no row
  reaches, when it opens a frame in its first four instructions, runs to a
  `jr $ra` of its own over at least 32 bytes, or is a whole leaf (its own
  `jr $ra`, no `$sp` access, no trap: a getter like `lui; lbu; jr $ra; sltu`
  or a `return 0;`): the only sign left for the functions a level links but
  never calls;
- a copy of an executable function body (the row is exactly the executable
  function's size), the level's entry point, the start of the text record, or
  a hand-confirmed row in `config/overlays/us/confirmed-starts.tsv`. A
  confirmed start cuts every copy of its function, in every level.

A row ends at its last *reachable* instruction (branches with their delay
slots, `j` inside the function, `switch` cases read from the jump table), so the bytes the compiler
leaves between one function's return and the next function's first instruction
(the tail of two returns whose bodies are elsewhere, a `beqz`/`jr $ra`/`break`
block) belong to no row: no C produces them, so a row that swallowed them could
never be matched. The fingerprint is taken over that reachable body too, so
every copy of a function in every level lands in one row.

A shared or level function is `FUN_LNN_xxxxxxxx`: its address in the
lowest-numbered level that has it, its *canonical level*. Data it references
is `D_LNN_XXXXXXXX` for addresses at 0x15EF00 and up (the level's own
records) and the executable's own `D_XXXXXXXX` / `FUN_xxxxxxxx` below it,
since that memory is resident. Jump tables are `jtbl_LNN_XXXXXXXX`; they live
in the level's `data` record.

Masking hides a constant that goes through a masked field, so a few tiny
functions that differ only in such a constant share one catalogue row (the
`core.lit` constant loaders above). Only executable code from 0x15EF00 up can
reappear in a level as the same function; a level body whose masked
fingerprint equals a resident function's (8 and 12 byte bodies) is a distinct
copy and is catalogued as level code. The per-place proof below is what
finally decides a C body for every place.

## Sources and the proof

`src/overlays/shared/` holds the shared functions, `src/overlays/lNN/` each
level's own, in link order: a file runs from one executable unit to the next
(the level program keeps the executable's link order and interleaves its own
functions between the executable's), or about 32 KB, and is named after that
unit and its first function. Every function starts as an
`INCLUDE_ASM("config/us/overlays/asm/<name>.s", <name>)` stub and is replaced
by C as it is matched. The build target `make overlays` (`configure.py
--overlays`, then ninja in `build/overlays/`) compiles every file with the
game compiler's driver (its `cc1`, GNU `as`) and the executable's game-code
flags into `build/overlays/obj/`; nothing is linked, and the executable
build and `./verify-baseline.sh` are untouched.

An overlay function is C_EXACT when its C, compiled on the game compiler
route (`cc1`, then `Ps2EeAs` as retail's game code was assembled) and
placed at its canonical address with every symbol at that level's address
(`_gp` = 0x166C00, jump tables from the level's `data` record), is byte for
byte the level's text (`scripts/decomp try FUN_LNN_xxxxxxxx cand.c` in the
tooling repository, method `overlay-place-bytes-v1`). That is stricter than
the masked comparison the catalogue uses: every relocation has to reach the
right place. Only such C is promoted into `src/overlays/`; a function in C
there counts as exact, exactly as a promoted executable unit does.

## Progress

Every distinct function counts once. An executable function repeated in the
levels counts as the executable's; a shared function counts once, not 19
times. The headline C_EXACT covers the executable and the overlays together
([progress-metrics.md](progress-metrics.md)); the progress map shows the
executable as a drawer with its own percentage above a tree of the shared code
and the 19 levels, and the progress report carries the categories `boot`,
`shared`, `levels` and `level_NN` for decomp.dev.
