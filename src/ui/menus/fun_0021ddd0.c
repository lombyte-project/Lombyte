/* Ported from rac1-decomp (src/game/pause.c, func_0021EDD8). */
#include "sda.h"
extern unsigned char D_001413F4 NOT_SDA;
static inline short pauseFlagState(void) {
    if (D_001413F4 != 1) {
        return 3;
    }
    return 0;
}
/* The select sits in a `static inline short` helper so that it stays a
   branch: jump.c turns an if into movz/movn only when its arm sets a
   full register, and the inline's short return value is a subreg. */
int FUN_0021ddd0(char *arg0) {
    char *p = *(char **)(arg0 + 0x34);
    *(short *)(p + 2) = pauseFlagState();
    return 0;
}

extern __typeof__(FUN_0021ddd0) func_0021DDD0 __attribute__((alias("FUN_0021ddd0")));
