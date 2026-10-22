# Progress

Progress is counted in bytes over the code of the boot executable and the 19
level programs ([overlays.md](overlays.md)). Other files on the disc are not
counted; the DVP blobs inside the executable are rebuilt as raw data.

| Metric      | Meaning                                                          |
| :---------- | :--------------------------------------------------------------- |
| **C_EXACT** | the function compiles from C to the same bytes as retail          |
| **C_FUZZY** | a pending function's C is close to retail but does not match yet |

A file with several functions counts each one, split at the retail labels its
source binds them to (`__asm__("FUN_…")`).

This page has no percentages on purpose; they would be out of date right
away. Current numbers: the
[progress map](https://github.com/lombyte-project/lombyte/blob/progress/decomp_map.svg),
the [report](https://github.com/lombyte-project/lombyte/blob/progress/report.json)
or [recomp.fyi](https://recomp.fyi/game/ratchet-and-clank-ps2).

## What counts as C

A function counts toward C_EXACT only if its source is C. Inline asm is
allowed only:

- as a label that binds a declaration to its linked name
  (`void f(void) __asm__("FUN_00202d10");`);
- in the approved header macros: the quadword copy in `include/qcopy.h` and
  `include/qzero.h`, and the privileged instructions C cannot write
  (`CpuEnableInt`, `CpuSync`, `CpuReadStatus`, `CpuSyscall`, ...) in
  `include/kernel.h`.

Any other asm (instructions, an empty memory barrier, `__asm__("" : "+r"(x))`,
`.extern`, a register pin like `register int x asm("v1")`) keeps the function
**pending** (C_FUZZY 99.99) even when the bytes match. The rule is
`non_label_asm()` in `scripts/rnc_units.py` (copied in
`scripts/generate_treemap.py`).

## 100 % in objdiff is not exact

objdiff compares instructions and ignores relocations, so stores to different
globals in the wrong order still score 100 %. A function is exact only when it
also passes the byte check: relinked with the real PS2 linker at the retail
addresses and compared byte for byte with `config/us/SCUS_971.99`
(`retail_verification`, method `ps2-ld-retail-bytes-v1`). `make elf` then
checks the whole executable.

For a near miss, the diff rows and the instruction count tell you more than
the percentage.

## Hand-written assembly

Units marked `intentional_asm` in `config/us/unit_categories.json` are left
out of the numbers: SIMD, VU0/MMI and COP2 helpers whose C would not be
faithful, and code written in assembly by hand (syscall stubs, code using
`$at` or trapping `add`). Code that only needs `ei`, `di`, `sync`, `mfc0` or
`syscall` is not on the list; it is C with the `include/kernel.h` macros.

`check-unit.py` refuses these units, and they have **no `#else` C body**: such
a body could never be checked, so it would be a perfect place for a wrong one
to hide.

## Data

The report also counts data (`total_data`, `matched_data`): the executable's
data (`core_data` to `lvl_sndvtbl` in `config/us/rnc1.us.yaml`) and each
level's `.lit`, `.data` and vtables. bss has no bytes and is left out.

Data counts as matched when C defines it and a check covers it:

- executable: an entry in `RODATA_OVERLAYS`, `SDATA_OVERLAYS` or
  `DATA_OVERLAYS` (`configure.py`) links the unit's section in place of the
  raw bytes, so the executable's hash covers it;
- levels: a row in `config/overlays/us/data.tsv`, checked by `make overlays`.
  A `D_XXXXXXXX` row counts in every level.

`decomp_map.json` also has `bytes_matching_c_zero`: the part of the matched
data that is zeros.

## Oracles

Units not in C yet are built from assembly or raw machine code (_oracles_)
so the executable still matches. They are made at build time from your own
`config/us/SCUS_971.99` into `config/us/expected/asm/` (not in Git). Matching
C replaces them over time.

## Compiler flags

Each unit is measured with the build rule `configure.py` picks for it. The
default rule has no per-unit flags. A flag such as `-fno-schedule-insns` is
used only for units listed in `GAME_COMPILER_FLAG_UNITS` or `SN_FLAG_UNITS`
at the top of `configure.py`, and only as a last resort
([decompilation-tips.md](../contributing/decompilation-tips.md)).

## The map and the report

The map (`decomp_map.svg`) has two parts:

- the **drawer** at the top: the boot executable, its C_EXACT and a tile per
  source folder, sized by bytes;
- the **tree** below: shared level code, then the 19 levels in game order,
  each with its C_EXACT and a bar with one block per `src/overlays/` file.

The big number on the right is C_EXACT for the whole game, with data in C
above it. Colours: **orange** is C that matches, **chrome** is hand-written
assembly, **dark steel** to **copper** is pending C by how much of it matches.

The report groups executable functions by source folder (`src/ui/menus/` is
the `ui/menus` group), so moving a file changes its group. Names come from
the C name bound to each `FUN_<address>` label.

`report.json` keeps the executable and levels apart: `boot` (= `game` +
`sdk`), `overlays` (= `shared` + `levels`) and one `level_NN` per level.
`decomp_map.json` (schema `rnc-public-progress-v3`) has `boot`, `overlays` and
`total`, each with a `data` part.

Neither file is in Git. The `progress` workflow rebuilds both on every push to
`main` (published on the `progress` branch) and comments the change on every
pull request. It does not build the game, so C_FUZZY comes from the
maintainers' builds (`fuzzy_scores.json` next to the report). Locally:

```sh
make progress                         # after make elf, measures C_FUZZY too
python3 scripts/generate_treemap.py   # map only, in build/progress/decomp_map.svg
```
