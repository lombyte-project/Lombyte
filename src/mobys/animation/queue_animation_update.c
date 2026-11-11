#include "types.h"
#include "rnc/ui/hud/hud_state.h"
#include "rnc/globals.h"

struct Anim {
    s32 id;
    s32 flags;
    u8  pad8[0x18];
    s32 next_id;
    s32 next_mode;
    s32 next_b;
    s32 next_c;
    s32 next_fn;
    s32 next_d;
    s32 next_e;
    u8  pad3c[0x28];
    s32 serial;
    s32 pending;
    u8  pad6c[4];
    s32 counter_digits; /* 0x70: HudElem.cnt */
    u8  pad74[8];
    s32 visible_timer;
    u8  pad80[0x10];
};

extern struct Anim hud_chan_slots[] __asm__("D_00199B60");
extern void        apply_pending_animation(struct Anim *) __asm__("func_001FF418");
s32 queue_animation_update(s32 chan, s32 id, s32 fn, s32 d, s32 e, s32 c, s32 b) __asm__("FUN_001ff308");

s32 queue_animation_update(s32 chan,
                           s32 queued_animation_id,
                           s32 fn,
                           s32 animation_callback,
                           s32 queued_animation_callback,
                           s32 animation_context,

                           s32 queued_animation_value) {
    s32          slot = chan & 0xF;
    s32          channel_flags = chan & 0xFFF0;
    struct Anim *anim = &hud_chan_slots[slot];

    if (game_mode == 5 && slot != 2 && slot != 0) {
        return 0;
    }
    if (anim->next_c != animation_context || anim->next_b != queued_animation_value ||
        anim->next_id != queued_animation_id || anim->next_mode != channel_flags ||
        anim->next_fn != fn || anim->next_d != animation_callback ||
        anim->next_e != queued_animation_callback) {
        anim->next_c = animation_context;
        anim->serial = hud_state.serial++;
        anim->next_b = queued_animation_value;
        anim->next_id = queued_animation_id;
        anim->next_fn = fn;
        anim->next_d = animation_callback;
        anim->next_e = queued_animation_callback;
        anim->pending = 1;
        anim->next_mode = channel_flags;
        anim->visible_timer = 0;
        anim->counter_digits = 0;
        if (channel_flags & anim->flags & 0x20) {
            apply_pending_animation(anim);
        }
    }
    return anim->serial;
}

extern __typeof__(queue_animation_update) func_001FF308 __attribute__((alias("FUN_001ff308")));
