#include "types.h"

extern u8 D_001DF3F0[];
extern void measure_text_width(s32 arg0, s32 arg1, void *arg2) __asm__("func_001F6200");

void measure_text_width_small(s32 arg0, s32 arg1) __asm__("FUN_001f6270");

void measure_text_width_small(s32 arg0, s32 arg1) {
    measure_text_width(arg0, arg1, D_001DF3F0);
}

extern __typeof__(measure_text_width_small) func_001F6270 __attribute__((alias("FUN_001f6270")));
