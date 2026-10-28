#ifndef LOMBYTE_FAKEMATCH_H
#define LOMBYTE_FAKEMATCH_H

/* Asm-only fakematch tricks (docs/contributing/code-cleanup.md). They emit no
   code; each use still needs a `Fakematch:` comment saying what it changes. */

/* The compiler does not move insns across this point. */
#define FAKEMATCH_BARRIER() __asm__ volatile("")

/* Reads hard register REG (e.g. "$16") here, so the register counts as live
   up to this point even though nothing set it on this path. */
#define FAKEMATCH_KEEP_LIVE(reg)                                               \
    do {                                                                       \
        register int fakematch_live_ __asm__(reg);                             \
        __asm__ volatile("" : : "r"(fakematch_live_));                         \
    } while (0)

#endif /* LOMBYTE_FAKEMATCH_H */
