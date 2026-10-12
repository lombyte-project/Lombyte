#include "types.h"
extern s32 draw_callback_count __asm__("D_0015F464");
extern void (*draw_callback_funcs[])(s32) __asm__("D_0018DB40");
extern s32 draw_callback_args[] __asm__("D_0018DC40");
void dispatch_callback_list_1(void) __asm__("FUN_001f4650");

void dispatch_callback_list_1(void) {
    s32 i;

    for (i = 0; i < draw_callback_count; i++) {
        draw_callback_funcs[i](draw_callback_args[i]);
    }
}

extern __typeof__(dispatch_callback_list_1) func_001F4650 __attribute__((alias("FUN_001f4650")));
