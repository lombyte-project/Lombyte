#include "types.h"
extern s32 draw_callback2_count __asm__("D_0015F468");
extern u8 draw_callback2_funcs[] __asm__("D_0018DD40");
extern u8 draw_callback2_args[] __asm__("D_0018DE40");
void enqueue_callback_list_4(s32 arg0, s32 arg1) __asm__("FUN_001f47b8");

void enqueue_callback_list_4(s32 callback, s32 callback_argument) {
    s32 callback_entry_byte_offset;

    if (draw_callback2_count < 0x40) {
        callback_entry_byte_offset = draw_callback2_count * 4;
        *(s32 *)(callback_entry_byte_offset + draw_callback2_funcs) = callback;
        *(s32 *)(callback_entry_byte_offset + draw_callback2_args) = callback_argument;
        draw_callback2_count += 1;
    }
}
