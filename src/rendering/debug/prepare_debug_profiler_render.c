#include "types.h"

extern s32 draw_enable_mask __asm__("D_0015F434");
extern s32 space_load_in_progress __asm__("D_0015F618");
extern void draw_debug_profiler() __asm__("func_001F39D0");
extern void append_gif_transfer_packet() __asm__("func_001FB368");

void prepare_debug_profiler_render(void) __asm__("FUN_001f4248");

void prepare_debug_profiler_render(void) {
    if (space_load_in_progress == 0) {
        s32 v;
        append_gif_transfer_packet();
        v = 0x7F;
        draw_enable_mask = v;
        draw_debug_profiler();
    }
}

extern void func_001F4248(void) __attribute__((alias("FUN_001f4248")));
