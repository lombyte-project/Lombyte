#include "types.h"
struct Anim;
typedef void (*AnimFn)(struct Anim *);
struct Anim {
    s32 unk0;
    s32 a;
    s32 b;
    s32 c;
    AnimFn fn;
    s32 d;
    s32 e;
    u8 pad1c[4];
    s32 next_id;
    s32 next_a;
    s32 next_b;
    s32 next_c;
    AnimFn next_fn;
    s32 next_d;
    s32 next_e;
    u8 pad3c[0x2C];
    s32 pending;
};
extern void load_animation_definition(struct Anim *, s32) __asm__("func_001FF500");
void apply_pending_animation(struct Anim *anim) __asm__("FUN_001ff418");

void apply_pending_animation(struct Anim *anim) {
    load_animation_definition(anim, anim->next_id);
    anim->a = anim->next_a;
    anim->d = anim->next_d;
    anim->e = anim->next_e;
    anim->c = anim->next_c;
    anim->b = anim->next_b;
    anim->fn = anim->next_fn;
    if (anim->fn != 0) {
        anim->fn(anim);
    }
    anim->pending = 0;
}

extern __typeof__(apply_pending_animation) func_001FF418 __attribute__((alias("FUN_001ff418")));
