#include "types.h"
#include "rnc/ui/hud/hud_state.h"
struct Anim {
    s32 id;
    u8 pad4[0x3C];
    s16 index;
    u8 flags;
    u8 pad43;
    s32 frames;
};
extern s32 hud_get_icon_index(s32) __asm__("func_001FEE38");
void load_animation_definition(struct Anim *anim, s32 id) __asm__("FUN_001ff500");

void load_animation_definition(struct Anim *anim, s32 id) {
    s32 i = hud_get_icon_index(id);

    anim->id = hud_state.anim_defs[i].id;
    anim->index = i;
    anim->flags = hud_state.anim_defs[i].flags;
    anim->frames = hud_state.anim_defs[i].start;
}

extern __typeof__(load_animation_definition) func_001FF500 __attribute__((alias("FUN_001ff500")));
