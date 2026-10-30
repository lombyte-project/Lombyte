/* Level 14 cutscene effects, run from the level main loop: particle pairs and a draw callback on the second cutscene moby during frames 0x1C-0x21 of scene 0. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/l14/cutscene_state.h"

extern void emit_moby_particle_pairs(void *) __asm__("FUN_L00_0023e008");

extern void func_L00_0023E4C8(void *) __asm__("FUN_L14_002459c0");

void update_level14_cutscene_effects(void) __asm__("FUN_L14_002460f8");

void update_level14_cutscene_effects(void) {
    void *moby;
    if (current_level_index != 14 || D_L14_0016CF60.scene != 0) {
        return;
    }
    moby = D_L14_0016CF60.scene_mobys[1];
    if (moby != 0 && (unsigned)(D_L14_0016CF60.frame - 0x1C) < 6) {
        emit_moby_particle_pairs(moby);
        add_draw_callback(func_L00_0023E4C8, moby);
    }
}
