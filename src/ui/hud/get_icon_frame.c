#include "types.h"
#include "rnc/ui/hud/hud_state.h"
extern s32 hud_get_icon_index(s32) __asm__("func_001FEE38");
s32 get_icon_frame(s32 id, s32 frame) __asm__("FUN_001ff960");

s32 get_icon_frame(s32 id, s32 frame) {
    s32 i = hud_get_icon_index(id);
    s32 k;

    if (hud_state.anim_defs[i].id == 0xFFFF) {
        return 0;
    }
    if (frame >= hud_state.anim_defs[i].count) {
        return 0;
    }
    k = hud_state.anim_defs[i].start + frame;
    if (hud_state.palette_pages[hud_state.frame_refs[k].palette_index].source_address & 0x80000000) {
        return 0;
    }
    if (hud_state.image_pages[hud_state.frame_refs[k].image_index].source_address & 0x80000000) {
        return 0;
    }
    return k;
}

extern __typeof__(get_icon_frame) func_001FF960
    __attribute__((alias("FUN_001ff960")));
