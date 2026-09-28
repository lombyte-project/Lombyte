/* Ported from rac1-decomp, the PAL decompilation (src/game/pause.c, func_0021DA98). */
#include "sda.h"
extern unsigned char D_0013D4C2 NOT_SDA;
extern char D_001D06D0[];
extern char D_001D0708[];
int FUN_0021ca98(void *arg0) {
    *(char **)((char *)arg0 + 0x34) =
        (D_0013D4C2 != 0) ? D_001D06D0 : D_001D0708;
    return 0;
}

extern __typeof__(FUN_0021ca98) func_0021CA98 __attribute__((alias("FUN_0021ca98")));
