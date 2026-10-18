#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/gameplay/state/game_globals.h"

extern void prepare_debug_profiler_render(void) __asm__("func_001F4248");
extern void draw_debug_profiler(void) __asm__("func_001F39D0");
extern void render_level_frame(void) __asm__("func_0022F288");

void dispatch_game_state_update(void) __asm__("FUN_00230ee8");

extern s32 draw_enable_mask_gp __asm__("D_0015F434") __attribute__((sda));
void dispatch_game_state_update(void) {
    enum {
        LEVEL_FRAME_RENDER = 4
    };
    s32 state;

    if (space_load_in_progress != 0) {
        return;
    }
    state = ship_substate[0];
    if ((u32)state >= 9U) {
        return;
    }
    switch (state) {
    case 0:
    case 8:
        prepare_debug_profiler_render();
        return;
    case 3:
    case 7:
        draw_enable_mask_gp = 0x7F;
        draw_debug_profiler();
        return;
    case LEVEL_FRAME_RENDER:
        render_level_frame();
        break;
    }
}

extern __typeof__(dispatch_game_state_update) func_00230EE8 __attribute__((alias("FUN_00230ee8")));
