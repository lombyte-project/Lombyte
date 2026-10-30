#include "types.h"
#include "rnc/rendering/render_globals.h"

void add_draw_callback(void *callback, void *callback_argument) __asm__("FUN_001f4600");

void add_draw_callback(void *callback, void *callback_argument) {
    s32 callback_entry_byte_offset;

    if (draw_callback_count < 0x40) {
        callback_entry_byte_offset = draw_callback_count * 4;
        *(void **)(callback_entry_byte_offset + (u8 *)draw_callback_funcs) = callback;
        *(void **)(callback_entry_byte_offset + (u8 *)draw_callback_args) = callback_argument;
        draw_callback_count += 1;
    }
}

void (*draw_callback_funcs[0x40])(s32) = {0};

s32 draw_callback_args[0x40] = {0};
