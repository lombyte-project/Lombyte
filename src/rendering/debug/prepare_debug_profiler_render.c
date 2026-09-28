#include "types.h"

extern s32 D_0015F434;
extern s32 D_0015F618;
extern void func_001F39D0();
extern void func_001FB368();

void prepare_debug_profiler_render(void) __asm__("FUN_001f4248");

void prepare_debug_profiler_render(void) {
    if (D_0015F618 == 0) {
        s32 v;
        func_001FB368();
        v = 0x7F;
        D_0015F434 = v;
        func_001F39D0();
    }
}

extern void func_001F4248(void) __attribute__((alias("FUN_001f4248")));
