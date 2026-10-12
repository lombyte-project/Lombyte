#include "types.h"
extern s32 draw_callback2_count __asm__("D_0015F468");
extern u8 draw_callback2_funcs[] __asm__("D_0018DD40");
extern u8 draw_callback2_args[] __asm__("D_0018DE40");
void enqueue_callback_list_4(s32 arg0, s32 arg1) __asm__("FUN_001f47b8");

void enqueue_callback_list_4(s32 arg0, s32 arg1) {
    s32 off;

    if (draw_callback2_count < 0x40) {
        off = draw_callback2_count * 4;
        *(s32 *)(off + draw_callback2_funcs) = arg0;
        *(s32 *)(off + draw_callback2_args) = arg1;
        draw_callback2_count += 1;
    }
}
