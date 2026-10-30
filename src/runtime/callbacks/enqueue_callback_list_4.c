#include "types.h"
#include "rnc/rendering/render_globals.h"

void enqueue_callback_list_4(s32 callback, s32 callback_argument) __asm__("FUN_001f47b8");

void enqueue_callback_list_4(s32 callback, s32 callback_argument) {
    s32 callback_entry_byte_offset;

    if (draw_callback2_count < 0x40) {
        callback_entry_byte_offset = draw_callback2_count * 4;
        *(s32 *)(callback_entry_byte_offset + (u8 *)draw_callback2_funcs) = callback;
        *(s32 *)(callback_entry_byte_offset + (u8 *)draw_callback2_args) = callback_argument;
        draw_callback2_count += 1;
    }
}

void (*draw_callback2_funcs[0x40])(s32) = {0};

s32 draw_callback2_args[0x40] = {0};
