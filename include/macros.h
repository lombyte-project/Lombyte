#ifndef LOMBYTE_MACROS_H
#define LOMBYTE_MACROS_H

/* Compiler hints that emit no code. Matching code uses them only as marked
   fakematches (docs/contributing/code-cleanup.md). */

/* Keeps the compiler from moving insns across this point. */
#define BARRIER() __asm__ volatile("")

/* Reads hard register REG (e.g. "$16") here, so it counts as live up to this
   point. */
#define REG_LIVE(reg)                            \
    do {                                         \
        register int reg_live_ __asm__(reg);     \
        __asm__ volatile("" : : "r"(reg_live_)); \
    } while (0)

#endif /* LOMBYTE_MACROS_H */
