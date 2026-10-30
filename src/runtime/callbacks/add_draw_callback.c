#include "types.h"

extern s32 draw_callback_count __asm__("D_0015F464");
extern u8  draw_callback_funcs[] __asm__("D_0018DB40");
extern u8  draw_callback_args[] __asm__("D_0018DC40");
void       add_draw_callback(void *callback, void *callback_argument) __asm__("FUN_001f4600");

void add_draw_callback(void *callback, void *callback_argument) {
    s32 callback_entry_byte_offset;

    if (draw_callback_count < 0x40) {
        callback_entry_byte_offset = draw_callback_count * 4;
        *(void **)(callback_entry_byte_offset + draw_callback_funcs) = callback;
        *(void **)(callback_entry_byte_offset + draw_callback_args) = callback_argument;
        draw_callback_count += 1;
    }
}
