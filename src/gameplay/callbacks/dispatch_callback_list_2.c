#include "types.h"
extern s32 draw_callback3_count __asm__("D_0015F46C");
extern void (*draw_callback3_funcs[])(s32) __asm__("D_0018DF40");
extern s32 draw_callback3_args[] __asm__("D_0018E040");
void dispatch_callback_list_2(void) __asm__("FUN_001f46c8");

void dispatch_callback_list_2(void) {
    s32 i;

    for (i = 0; i < draw_callback3_count; i++) {
        draw_callback3_funcs[i](draw_callback3_args[i]);
    }
}

extern __typeof__(dispatch_callback_list_2) func_001F46C8 __attribute__((alias("FUN_001f46c8")));
