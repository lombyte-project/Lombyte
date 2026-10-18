# Progress metrics

Two numbers describe the same reconstruction from different angles, and both are
byte-weighted over the **configured code in the boot executable and the 19 level
code overlays** — the game's program, not the whole disc. The disc's embedded
DVP overlay blobs are rebuilt as raw data; other executables on the disc are out
of scope. How the overlays are counted is in [Level overlays](#level-overlays).

| Metric      | Meaning                                                                   |
| :---------- | :------------------------------------------------------------------------ |
| **C_EXACT** | the unit's functions compile from C to the same bytes as retail           |
| **C_FUZZY** | a pending unit's C body is measurably similar to retail, without matching |

A unit that holds several functions counts each one, split at the retail labels
its source binds them to (`__asm__("FUN_…")`).

This document deliberately carries **no percentages**: they are regenerated from
the build and would be stale the moment they were written down. Read the current
figures from the [progress map](https://github.com/lombyte-project/lombyte/blob/progress/decomp_map.svg),
the [report](https://github.com/lombyte-project/lombyte/blob/progress/report.json), or from
[recomp.fyi](https://recomp.fyi/game/ratchet-and-clank-ps2) — never from prose.

## Data

The report also counts data (`total_data`, `matched_data`): the executable's
preserved data (`core_data` to `lvl_sndvtbl` in `config/us/rnc1.us.yaml`) and
each level's `.lit`, `.data` and vtables; bss holds no bytes and is left out.
Data is matched when C defines it and a proof checks it: an entry in
`RODATA_OVERLAYS`, `SDATA_OVERLAYS` or `DATA_OVERLAYS` (`configure.py`) links
the unit's section in place of the blob bytes, so the boot ELF hash covers it;
a level object in `config/overlays/us/data.tsv` is compared by `make overlays`.
A `D_XXXXXXXX` row counts in every level. `decomp_map.json` also gives
`bytes_matching_c_zero`: the part of the matched data that is zeros.

## Exact bytes are not enough: the source must be C

A promoted unit counts toward C_EXACT only when its source is C. Inline asm is
allowed solely as a label that binds a declaration to its linked name
(`void f(void) __asm__("FUN_00202d10");`), plus the approved macros in headers:
the quadword idiom in `include/qcopy.h` and `include/qzero.h`, and the
privileged instructions C cannot express (`CpuEnableInt`, `CpuSync`,
`CpuReadStatus`, `CpuSyscall`, ...) in `include/kernel.h`. A unit whose file holds any other asm - instructions, an
empty memory barrier, `__asm__("" : "+r"(x))`, a `.extern` directive, or a
register pin (`register int x asm("v1")`) - builds byte-exact but stays
**pending** (C_FUZZY 99.99) until the asm is gone. `non_label_asm()` in
`scripts/rnc_units.py` (mirrored in `scripts/generate_treemap.py`) is the rule.

## 100 % is not the same as exact

objdiff compares instructions and is **relocation-agnostic**, so any permutation
of stores to distinct globals scores 100 % while the linked bytes differ. A
source that reports `code_percent=100.0` is therefore not promotable until it
also passes the linked-byte proof, which relinks the candidate with the real PS2
linker at the generated retail layout addresses and compares each allocated
section byte-for-byte against `config/us/SCUS_971.99`
(`retail_verification`, method `ps2-ld-retail-bytes-v1`).

A 100 % candidate that is not exact usually has a store order or a linked
address wrong in a way objdiff cannot see. The diff rows and the instruction
count say more about a near miss than the percentage, which moves with row
alignment.

Matching is verified at two levels: objdiff per compiled object, and a whole-ELF
comparison against the original through `./verify-baseline.sh` (see
[building.md](building.md)).

## The progress map

The map (`decomp_map.svg`, published on the `progress` branch by CI) has two parts. The **drawer** at the top is the boot
executable on the left 3/4: its own C_EXACT percentage over its group
treemap, dimmed to a texture (every logical group is a tile sized by its bytes; the colour is the
group's state). Below it the **tree** lists the shared level code first and then
the 19 levels in game order, each with its planet, a one-line description, its
C_EXACT percentage and a bar of blocks, one per `src/overlays/` file, sized by
bytes. Right of the drawer's slanted cut sits the main figure, the C_EXACT of
the whole game (exe + overlays in C), with data in C (see Data above) above
it. Everywhere the same scale applies: **orange** is all recoverable C matching exactly, **chrome**
is intentional low-level assembly only, and **dark steel** to **copper** is
pending C by its exact coverage.

```sh
python3 scripts/generate_treemap.py   # preview in build/progress/decomp_map.svg
```

Neither the map nor the objdiff-format report behind
[recomp.fyi](https://recomp.fyi/game/ratchet-and-clank-ps2) is committed. The
`progress` workflow regenerates both from the tree on every push to `main`
(publishing them on the `progress` branch) and on every pull request (posting
the change as a comment); it never builds the game. C_FUZZY needs a build, so
it is measured by the maintainers' tooling and read by the workflow from there
(published as `fuzzy_scores.json` beside the report). Locally, `make progress`
after `make elf` measures it the same way.

## What counts as intentional assembly

`config/us/unit_categories.json` classifies units, and the ones marked
`intentional_asm` are excluded from the C goal: SIMD, VU0/MMI and COP2 helpers
whose C would not be a faithful reconstruction, and code written by hand in
assembly (kernel syscall stubs, code using `$at` or trapping `add`). Compiled
code that only needs `ei`, `di`, `sync`, `mfc0` or `syscall` is not on this
list: it is C with the `include/kernel.h` macros. They are left out of the
percentages, `check-unit.py` refuses them by design, and they carry **no `#else`
C body at all** — a body there can never be measured, staged or promoted, which
makes it the ideal hiding place for a wrong one. Evidence behind the
classification is kept by the maintainers.

The report groups boot functions by their source folder (`src/ui/menus/`
is the `ui/menus` group): move a file to change its group. Function names
come from the source (the C name bound to the `FUN_<address>` label).

## Oracles

A matching executable does not mean the decompilation is complete. Unconverted
units keep using assembly or raw machine-code _oracles_ to preserve the original
bytes, and matching C replaces them over time. The oracles are generated at
build time from your own `config/us/SCUS_971.99` into the gitignored
`config/us/expected/asm/` tree — the repository stores none, which is why a build
cannot start before the game file is in place.

## Compiler flags

The report measures each unit with the build rule `configure.py` picks for
it. The default rule takes no per-unit compiler flags; a flag such as
`-fno-schedule-insns` is only honoured for a unit listed in the per-unit
flag tables at the top of `configure.py` (`GAME_COMPILER_FLAG_UNITS`,
`SN_FLAG_UNITS`), and decompilation-tips.md asks to add one only as a last
resort.

## Working on a unit

The per-unit workflow, from picking a target to opening a pull request, is in
[CONTRIBUTING.md](../CONTRIBUTING.md).

## Level overlays

Every level's data starts with its own build of the game program: code records
that replace the executable's whole `main` segment (text, data, vtables, bss)
when the level loads. The executable's game code is a subset of each level's
program; the rest is per-level code (enemies, bosses, level logic, the `update/*`
and `hero*` modules). All distinct level code is about nine times the
executable's game code ([overlays.md](overlays.md)).

The overlays count in the headline C_EXACT with these rules:

- every distinct function counts **once**: an executable function repeated in
  the levels is the executable's, a function shared by several levels is one
  function (`config/overlays/us/functions.tsv` is the list);
- a shared or level function is **C_EXACT** when its C is in `src/overlays/`,
  where it is put only after the byte proof (compiled on the game compiler
  route, placed at its address in the level, byte for byte the level's text,
  method `overlay-place-bytes-v1`, `make overlays`); it is **pending** while its line there is
  an `INCLUDE_ASM` stub. Overlay functions carry no C_FUZZY: a stub counts 0;
- an overlay function shown to be hand-written (VU0/COP2, MMI, raw `.word`
  instructions, hand-placed `nop`s) is listed as `overlays/<name>` under
  `intentional_asm` and stays out of the percentages, as in the executable.

The report (`report.json`) keeps the executable and the overlays apart in its
categories: `boot` (= `game` + `sdk`), `overlays` (= `shared` + `levels`), and
one `level_NN` per level. `decomp_map.json` (schema
`rnc-public-progress-v3`) has the same split as `boot`, `overlays` and
`total`; its top-level fields describe the total. Each part has a `data`
object with the same data bytes the report counts.
