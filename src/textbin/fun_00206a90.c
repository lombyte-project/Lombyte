/* Ported from rac1-decomp, the PAL decompilation (src/game/menu.c, func_002072C0). */
#include "sda.h"
extern int D_001413DC NOT_SDA;
extern unsigned char D_0013D396 NOT_SDA;
/* `if (x) return 1;` in both arms with one shared `return 0;` keeps the
   flag test a beqz with the li in its slot; a result variable or a
   return 0 per arm becomes sltu. */
int FUN_00206a90(int arg0, float unused1, float unused2, float arg1) {
    int is16 = D_001413DC == 0x10;

    if (arg0 < 0x100) {
        if (D_0013D396 != 0) {
            return 1;
        }
    } else if (arg1 >= 58.0f && arg1 <= 86.0f && !is16) {
        return 1;
    }
    return 0;
}

extern __typeof__(FUN_00206a90) func_00206A90 __attribute__((alias("FUN_00206a90")));
