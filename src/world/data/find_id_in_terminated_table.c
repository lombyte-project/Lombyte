#include "types.h"
extern s32 *D_001601E0;
s32 find_id_in_terminated_table(s32 id) __asm__("FUN_00205220");

s32 find_id_in_terminated_table(s32 id) {
    s32 i;

    for (i = 0; D_001601E0[i] != 0 && i < 20; i++) {
        if (D_001601E0[i] == id) {
            return i;
        }
    }
    return -1;
}

extern __typeof__(find_id_in_terminated_table) func_00205220 __attribute__((alias("FUN_00205220")));
