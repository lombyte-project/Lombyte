#include "types.h"

extern volatile s32 D_001611E0;
extern volatile s32 D_001611E4;
extern s32 sceGsSyncV();

void wait_for_display_vsync(s32 arg0) __asm__("FUN_0023b590");

void wait_for_display_vsync(s32 arg0) {
    do {

    } while (sceGsSyncV(0) == arg0);
    D_001611E0 = 1;
    D_001611E4 = 0;
}

extern __typeof__(wait_for_display_vsync) func_0023B590 __attribute__((alias("FUN_0023b590")));
