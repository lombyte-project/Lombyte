#include "rnc/mobys/moby.h"

/* Ported from rac1-decomp (src/game/mobyutil.c, func_00215048). */
/* A goto into the first `return 0` gives retail's backward beqz; the two
   nops before it are short-loop padding (tools/ps2eeas_nops.py). */
int get_moby_target_info(char *moby) __asm__("FUN_002141f8");

int get_moby_target_info(char *moby) {
    if (moby == 0) {
    ret0:
        return 0;
    }
    if ((MOBY(moby)->flags & 0x20) == 0) {
        goto ret0;
    }
    return *((int *)MOBY(moby)->pvars);
}

extern __typeof__(get_moby_target_info) func_002141F8 __attribute__((alias("FUN_002141f8")));
