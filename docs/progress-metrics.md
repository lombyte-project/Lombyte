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

This document deliberately carries **no percentages**: they are regenerated from
the build and would be stale the moment they were written down. Read the current
figures from the [progress map](https://github.com/mateuszklysz/Lombyte/blob/progress/decomp_map.svg),
the [report](https://github.com/mateuszklysz/Lombyte/blob/progress/report.json), or from
[decomp.dev](https://decomp.dev/mateuszklysz/Lombyte) — never from prose.

## Exact bytes are not enough: the source must be C

A promoted unit counts toward C_EXACT only when its source is C. Inline asm is
allowed solely as a label that binds a declaration to its linked name
(`void f(void) __asm__("FUN_00202d10");`), plus the one approved idiom in
`include/qcopy.h` and `include/qzero.h`. A unit whose file holds any other asm - instructions, an
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

The maintainer's promotion tool (`promote-unit.py`, in the private tooling
checkout, not in `scripts/`) requires both, and the promotion receipt records the
object hash, the retail hash and the layout hash the comparison was bound to. A
100 % candidate with `exact=False` is a real state, not a rounding artifact: it
usually means the store order or a linked address is wrong in a way objdiff
cannot see. Sorting a near miss by the diff rows and the instruction count is
more reliable than trusting the percentage, which moves with row alignment.

Matching is verified at two levels: objdiff per compiled object, and a whole-ELF
comparison against the original through `./verify-baseline.sh` (see
[building.md](building.md)).

## The progress map

The map (`decomp_map.svg`, published on the `progress` branch by CI) has two parts. The **drawer** at the top is the boot
executable: its own C_EXACT percentage over its group treemap, dimmed to a
texture (every logical group is a tile sized by its bytes; the colour is the
group's state). Below it the **tree** lists the shared level code first and then
the 19 levels in game order, each with its planet, a one-line description, its
C_EXACT percentage and a bar of blocks, one per `src/overlays/` file, sized by
bytes. The headline percentage in the header is the whole game. Everywhere the
same scale applies: **orange** is all recoverable C matching exactly, **chrome**
is intentional low-level assembly only, and **dark steel** to **copper** is
pending C by its exact coverage.

```sh
python3 scripts/generate_treemap.py   # preview in build/progress/decomp_map.svg
```

Neither the map nor the objdiff-format report behind
[decomp.dev](https://decomp.dev/mateuszklysz/Lombyte) is committed. The
`progress` workflow regenerates both from the tree on every push to `main`
(publishing them on the `progress` branch) and on every pull request (posting
the change as a comment); it never builds the game. C_FUZZY needs a build, so
it is measured by the maintainers' tooling and read by the workflow from there
(published as `fuzzy_scores.json` beside the report). Locally, `make progress`
after `make elf` measures it the same way.

## What counts as intentional assembly

`config/us/unit_categories.json` classifies units, and the ones marked
`intentional_asm` are excluded from the C goal: SIMD, VU0/MMI and COP2 helpers
whose C would not be a faithful reconstruction. They are left out of the
percentages, `check-unit.py` refuses them by design, and they carry **no `#else`
C body at all** — a body there can never be measured, staged or promoted, which
makes it the ideal hiding place for a wrong one. Evidence behind the
classification is in `analysis/audits/intentional-asm-evidence.json` in the
sibling tooling checkout.

Group assignments and proposed semantic names come from
`config/us/recovered_names.json`; the naming rules are in
[recovered-names.md](recovered-names.md).

## Oracles

A matching executable does not mean the decompilation is complete. Unconverted
units keep using assembly or raw machine-code _oracles_ to preserve the original
bytes, and matching C replaces them over time. The oracles are generated at
build time from your own `config/us/SCUS_971.99` into the gitignored
`config/us/expected/asm/` tree — the repository stores none, which is why a build
cannot start before the game file is in place.

## Why a public score can differ from a banked one

A candidate's score in the tooling repository is measured on the route its bank
records, which need not be the route the public build uses. The public number in
the published report is measured through the public harness, on whichever build
rule `config/us/rnc1.us.yaml` and the generated `config/us/build.ninja` select for
that unit. When those differ the two numbers differ, and the public one is the one
the report shows.

One consequence worth knowing: the default `cc` build rule — which every unit in
no route set gets, and there are many — accepts **no per-unit compiler flags at
all**. A unit whose best C needs `-G0` or `-fno-schedule-insns` is therefore not
reproducible publicly unless it is routed to a profile that has the flag hook.

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
- there is no intentional-asm class in the overlays yet: everything is
  recoverable until a function is shown to be hand-written VU/MMI code.

The report (`report.json`) keeps the executable and the overlays apart in its
categories: `boot` (= `game` + `sdk`), `overlays` (= `shared` + `levels`), and
one `level_NN` per level. `decomp_map.json` (schema
`rnc-public-progress-v3`) has the same split as `boot`, `overlays` and
`total`; its top-level fields describe the total.
