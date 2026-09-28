/* Ported from rac1-decomp, the PAL decompilation (src/game/menu.c, func_00207A80). */
#include "sda.h"
extern int D_001A03C0 NOT_SDA;
extern int D_001A03B8 NOT_SDA;
/* Hit test in two layouts: arg3 (the third float, $f14) must lie in
   [73.5, 80] with D_001A03C0 set, or for arg1 >= 0x9D in [51.5, 54] with
   D_001A03B8 set. Written as `<`/`>` rejections so they compile to retail's
   c.lt/bc1t; the second arm's `return 0` cross-jumps into the first's. */
int FUN_00207250(void *arg0, int arg1, float unused1, float unused2, float arg3) {
    if (arg1 < 0x9D) {
        if (arg3 < 73.5f || arg3 > 80.0f || D_001A03C0 == 0) {
            return 0;
        }
        return 1;
    } else {
        if (arg3 < 51.5f || arg3 > 54.0f || D_001A03B8 == 0) {
            return 0;
        }
        return 1;
    }
}

extern __typeof__(FUN_00207250) func_00207250 __attribute__((alias("FUN_00207250")));
