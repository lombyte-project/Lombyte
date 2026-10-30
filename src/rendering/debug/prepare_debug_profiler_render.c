#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/game/game_globals.h"
#include "rnc/rendering/render_globals.h"

extern void draw_debug_profiler() __asm__("func_001F39D0");
extern void append_gif_transfer_packet() __asm__("func_001FB368");

void prepare_debug_profiler_render(void) __asm__("FUN_001f4248");

void prepare_debug_profiler_render(void) {
    if (space_load_in_progress == 0) {
        s32 draw_mask;
        append_gif_transfer_packet();
        draw_mask = 0x7F;
        draw_enable_mask = draw_mask;
        draw_debug_profiler();
    }
}

extern void func_001F4248(void) __attribute__((alias("FUN_001f4248")));
