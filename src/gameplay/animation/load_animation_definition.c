#include "types.h"
struct AnimDef {
    u16 id;
    u8 pad2[2];
    u16 frames;
    u8 flags;
    u8 pad7;
};
struct AnimTable {
    u8 pad0[0x1C];
    struct AnimDef *defs;
};
struct Anim {
    s32 id;
    u8 pad4[0x3C];
    s16 index;
    u8 flags;
    u8 pad43;
    s32 frames;
};
extern struct AnimTable D_0019A3E8;
extern s32 hud_get_icon_index(s32) __asm__("func_001FEE38");
void load_animation_definition(struct Anim *anim, s32 id) __asm__("FUN_001ff500");

void load_animation_definition(struct Anim *anim, s32 id) {
    s32 i = hud_get_icon_index(id);

    anim->id = D_0019A3E8.defs[i].id;
    anim->index = i;
    anim->flags = D_0019A3E8.defs[i].flags;
    anim->frames = D_0019A3E8.defs[i].frames;
}

extern __typeof__(load_animation_definition) func_001FF500 __attribute__((alias("FUN_001ff500")));
