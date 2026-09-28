/* Ported from rac1-decomp, the PAL decompilation (src/game/stream.c, func_00217970). */
#include "sda.h"
extern short D_001516F0 NOT_SDA;
void FUN_00216ad0(int arg0, long arg1) {
    short *p = (short *)(int)arg1;
    if (p != 0) {
        if (arg0 < 0) {
            *(int *)p = arg0;
        }
        if (arg0 != 0) {
            if (p[5] == 9) {
                p[5] = 4;
                if (p[8] != 0) {
                    D_001516F0 = 1;
                }
            }
        } else {
            p[5] = 0;
        }
    }
}

extern __typeof__(FUN_00216ad0) func_00216AD0 __attribute__((alias("FUN_00216ad0")));
