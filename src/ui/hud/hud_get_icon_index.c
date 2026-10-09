#include "types.h"
#include "rnc/ui/hud/hud_state.h"
s32 hud_get_icon_index(s32 id) __asm__("FUN_001fee38");

s32 hud_get_icon_index(s32 id) {
    s32 i;

    for (i = 0; hud_state.anim_defs[i].id != 0xFFFF; i++) {
        if (hud_state.anim_defs[i].id == id) {
            break;
        }
    }
    return i;
}

extern __typeof__(hud_get_icon_index) func_001FEE38
    __attribute__((alias("FUN_001fee38")));
