#include "types.h"
extern s32 draw_callback_count __asm__("D_0015F464");
extern u8 draw_callback_funcs[] __asm__("D_0018DB40");
extern u8 draw_callback_args[] __asm__("D_0018DC40");
void enqueue_callback_list_1(s32 arg0, s32 arg1) __asm__("FUN_001f4600");

void enqueue_callback_list_1(s32 callback, s32 callback_argument) {
    s32 off;

    if (draw_callback_count < 0x40) {
        off = draw_callback_count * 4;
        *(s32 *)(off + draw_callback_funcs) = callback;
        *(s32 *)(off + draw_callback_args) = callback_argument;
        draw_callback_count += 1;
    }
}
