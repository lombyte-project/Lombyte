#include "types.h"
extern s32 draw_callback2_count __asm__("D_0015F468");
extern void (*draw_callback2_funcs[])(s32) __asm__("D_0018DD40");
extern s32 draw_callback2_args[] __asm__("D_0018DE40");
void dispatch_callback_list_4(void) __asm__("FUN_001f4808");

void dispatch_callback_list_4(void) {
    s32 i;

    for (i = 0; i < draw_callback2_count; i++) {
        draw_callback2_funcs[i](draw_callback2_args[i]);
    }
}

extern __typeof__(dispatch_callback_list_4) func_001F4808 __attribute__((alias("FUN_001f4808")));
