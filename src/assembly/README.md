# Assembly-backed units

Units here still build from the retail assembly: `INCLUDE_ASM` of a listing
that `configure.py --make-asm` generates from your own game executable
(`config/us/expected/asm/`, not in Git). The C draft sits under the `#else`
of `#ifndef NON_MATCHING`.

Units listed as `intentional_asm` in `config/us/unit_categories.json` are
hand-written low-level code and stay here with no C body. Every other unit
leaves this tree once its C is exact
([CONTRIBUTING.md](../../CONTRIBUTING.md#6-promote-an-exact-unit)).
