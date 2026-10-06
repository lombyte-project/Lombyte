#ifndef LOMBYTE_ASM_H
#define LOMBYTE_ASM_H

/*
 * Assembly-backed C fallback support.
 *
 * The normal matching build leaves NON_MATCHING undefined, so the original
 * expected assembly is included verbatim.  Defining NON_MATCHING selects the
 * descriptive C branch for coverage-oriented builds and review.  Keep this
 * macro deliberately small: the expected .s file is the single source of
 * truth for the exact branch and the symbol parameter documents the oracle
 * identity without changing the assembler input.
 *
 * The `.s` files live under `config/us/expected/asm/`, are generated from the
 * user's own retail ELF by `configure.py --make-asm`, and are gitignored --
 * the repository stores no transcribed assembly.  `.set noreorder` around the
 * include is required because the generated per-function files do not set it
 * and the assembler would otherwise reorder delay slots.
 */
#define INCLUDE_ASM(path, symbol)                                                                  \
    __asm__(".set noreorder\n.include \"macro.inc\"\n.include \"" path "\"\n.set reorder")

#endif /* LOMBYTE_ASM_H */
