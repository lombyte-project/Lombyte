# Progress metrics

Two numbers describe the same reconstruction from different angles, and both are
byte-weighted over the **configured code in the boot executable** — not the whole
disc. The disc's embedded DVP overlay blobs are rebuilt as raw data; overlays
and executables elsewhere on the disc are out of scope.

| Metric      | Meaning                                                                   |
| :---------- | :------------------------------------------------------------------------ |
| **C_EXACT** | the unit's functions compile from C to the same bytes as retail           |
| **C_FUZZY** | a pending unit's C body is measurably similar to retail, without matching |

This document deliberately carries **no percentages**: they are regenerated from
the build and would be stale the moment they were written down. Read the current
figures from `assets/decomp_map.svg`, from `progress/report.json`, or from
[decomp.dev](https://decomp.dev/mateuszklysz/Lombyte) — never from prose.

## Exact bytes are not enough: the source must be C

A promoted unit counts toward C_EXACT only when its source is C. Inline asm is
allowed solely as a label that binds a declaration to its linked name
(`void f(void) __asm__("FUN_00202d10");`), plus the one approved idiom in
`include/qcopy.h`. A unit whose file holds any other asm - instructions, an
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

`scripts/promote-unit.py` requires both, and the promotion receipt records the
object hash, the retail hash and the layout hash the comparison was bound to. A
100 % candidate with `exact=False` is a real state, not a rounding artifact: it
usually means the store order or a linked address is wrong in a way objdiff
cannot see. Sorting a near miss by the diff rows and the instruction count is
more reliable than trusting the percentage, which moves with row alignment.

Matching is verified at two levels: objdiff per compiled object, and a whole-ELF
comparison against the original through `./verify-baseline.sh` (see
[building.md](building.md)).

## The progress map

`assets/decomp_map.svg` tiles the boot executable by logical function group,
sized by the combined executable bytes of its functions. A tile's label shows
byte-weighted **C_EXACT** progress, and **C_FUZZY** when similarity scores are
available. **Orange** groups have all recoverable C functions matching exactly,
**chrome** groups contain intentional low-level assembly only, and **dark
steel** groups contain pending C.

```sh
.venv/bin/python scripts/generate_treemap.py
```

Pass `--workspace build/baseline` to also measure the pending C bodies and report
C_FUZZY alongside C_EXACT. The same progress is published on
[decomp.dev](https://decomp.dev/mateuszklysz/Lombyte) from the committed objdiff-format report
`progress/report.json`; regenerate it with `make progress` after `make elf`. The
`progress` workflow only checks, validates and uploads that file — it never
builds the game.

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
`progress/report.json` is measured through the public harness, on whichever build
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
