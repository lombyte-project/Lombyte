# Matching workflow and acceptance discipline

> **New contributors:** [CONTRIBUTING.md](../CONTRIBUTING.md) has the
> pick-a-function loop and the exact commands. This document is the acceptance
> discipline behind them; read it before promoting a unit.

Procedure and acceptance bar for Ratchet & Clank (PS2, `SCUS_971.99`).

## 1. Ground truth

- The retail boot executable and its configured boundaries are the only
  authority. Every claim binds to an address, a byte range, and hashes.
- Reconstruction (image bytes) and decompilation (readable C) are separate
  metrics. Keep them separate.

## 2. Ownership and oracles

- Every configured range has exactly one owner. Unfinished owners stay
  assembly-backed so the image is always reconstructible. The oracle is
  generated from your own retail ELF (`configure.py --make-asm`,
  `config/us/expected/asm/`, gitignored) and included with `INCLUDE_ASM`; the
  repository stores no transcribed assembly.
- An owner becomes C only when its entire compiled range is byte-exact.
  Retagging ownership records progress. The oracle is removed only after the
  replacement passes every gate.

## 3. Compiler model

- Two compilers built the retail executable: the SDK compiler for the SDK
  library block, the game compiler for everything from `GAME_TEXT_START` on.
  A unit's compiler follows from where retail placed it, not from which
  compiler happens to match it; match it there. Directory names and single
  samples are not evidence.
- A per-owner compiler or assembler option is a last resort, and the project is
  removing the ones it has. Add one only when the mismatch is measured, the
  cause is not explained by the source or by a compiler bug you can fix, and the
  entry carries that reason in `configure.py` next to the option. Prefer fixing
  the source or the compiler over freezing a switch for a function.

## 4. The unit loop

1. Read the complete target plus its callers and callees. Recover types,
   layouts, signatures, and one or two exact siblings.
2. Write descriptive C for one owned range.
3. Compile fresh and compare at object level.
4. Classify the earliest mismatch: CFG, ABI, field width or signedness,
   addressing, lifetime, ordering, scheduling.
5. Change one hypothesis. Recompile. Keep only a strict improvement; revert to
   the prior best on regression.
6. If the same classification survives three attempts, change the approach:
   recover missing context, try another source family, or escalate to a
   recovery route.

## 5. Shape families and batching

- Batch independent variants of one hypothesis (arm mirroring, hoisting,
  width, temporaries) and score them together. Serial probing is slower and
  destabilizes register allocation.
- Inspect complete aligned instruction streams before changing order or
  addressing. Filtered mismatch rows are not aligned pairs.

## 6. Exactness and the gate

- Exact means 100% code, functions, data, and complete-data for the owner,
  plus a byte-identical reconstructed image.
- A high score is not exactness, and relocation-blind comparisons are not
  proof. Verify linked bytes at the retail address, then the whole image.
- Run the aggregate image gate once per promotion batch; on failure, bisect
  the batch.

## 7. Promotion transaction

- Place the owner under its logical subsystem path. Pending assembly-backed
  owners stay under `assembly/<subsystem>/`. Keep the canonical linker symbol and
  required external aliases, resolve relocations and data ownership, and
  re-verify all four measures from a fresh compile.
- Remove the oracle only after the transaction succeeds, and update the record.
  Refresh the audit from fresh evidence; counters are derived, never edited.
- Use the semantic C identifier from `docs/recovered-names.md` when its evidence
  supports the function's role. Bind it to the existing `FUN_<address>` linker
  symbol with a GNU assembler label; this keeps the emitted symbol stable while
  making C references descriptive. Keep other aliases only when code or
  configuration requires them.

## 8. Evidence discipline

- Every attempt keeps a unique set of artifacts: source hash, compiler
  invocation, object, raw comparison report, measures, decision, next action.
- Evidence paths stay with the maintainers' artifacts, not in this repository.
- Raw reports are preserved; summaries do not replace them. Parked work records
  the tested hypotheses, the blocker class, and a concrete revisit trigger.

## 9. Recovery ladder

Escalate in order when the unit loop stalls:

1. Context recovery: field widths, pointer chains, ABI, layouts, siblings.
2. Source shapes: evaluation order, temporaries, branch polarity, loop form,
   declaration size.
3. Compiler and flag probes tied to a codegen hypothesis.
4. Guided C mutation search for allocation, store order, or delay-slot ties.
5. Alternate compiler builds when the required codegen is absent.
6. Park with a recorded blocker and a concrete revisit trigger.

A failed search is not proof of impossibility, and a higher score does not
justify a change in behavior.

## 10. Progress accounting

- Report exact functions and exact bytes separately. A partial score is not
  throughput until it is promoted.
- C_EXACT and C_FUZZY share the same recoverable-C denominator (intentional
  asm excluded). C_FUZZY replaces each pending unit's 0 with its measured
  `.text` similarity (objdiff via `check-unit`), weighted by unit bytes; exact
  units stay 100%. C_EXACT remains the authoritative metric.
- Exclude terminal categories such as hand-written low-level assembly from
  the recoverable-C denominator, and state the denominator.
- Keep the image hash gate status with the numbers.

## 11. Hygiene

- Do not refactor for style when it changes generated code. Matching,
  descriptive C is the deliverable.
- Keep game data, disc images, and proprietary compiler binaries out of the
  repository.

## 12. Configuration choices

- Segment boundaries and the undefined-function list are maintained by hand. A
  matching build must place every byte at its retail address, so automatic
  boundary detection or symbol inference is a starting point, never the
  authority; a wrong entry fails the full-image gate instead of silently
  changing the output.
- Compiler routing is a rule, not a list: `configure.py` builds a unit with
  the compiler of the retail block it sits in. Every exception and per-unit
  flag sits in one section at the top of `configure.py` (compiler routes).
  `ROUTE_EXCEPTIONS` names the
  units that do not reproduce on that compiler yet and the route that still
  builds them (SN, the patched EE-GCC, or the SDK compiler for a few game
  functions). It only shrinks: a unit leaves it when it builds on its own
  compiler with the common configuration, and nothing new is added.
- Small-data variables belong to the file that defines them. Ps2EeAs reaches a
  variable gp-relative only when it already knows its size: in a delay slot
  (`.set nomacro`) or after its definition in the same translation unit. So a
  unit that retail reaches gp-relative outside delay slots defines the
  variable itself, ahead of its functions, and is listed in `SDATA_OVERLAYS`
  so its `.sdata` lands at the retail address; functions that share such a
  variable live in one file (`rendering/vu1_chain.c`,
  `audio/rpc/snd_returns.c`). No `__asm__(".extern ...")` is needed.
- The build targets the boot executable and its embedded DVP overlay blobs;
  other disc files are out of scope.
- `verify-baseline.sh` only rebuilds a directory it owns: the staging root must
  contain its `.rnc-baseline-root` marker, so pointing `BASELINE_ROOT` at
  unrelated data cannot delete it.
- Public regression tests for the build scripts live in
  `scripts/test_public_tools.py`.
