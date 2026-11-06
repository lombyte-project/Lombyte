#include "types.h"

/* A HUD animation channel (0x90 bytes, 13 of them); queue_animation_update and
   apply_pending_animation see the same layout. */
struct E {
    u8  pad0[0x4];
    s32 mode; /* 0x04: current mode, copied from next_mode by apply_pending_animation */
    u8  pad8[0x1C];
    s32 next_mode; /* 0x24 */
    u8  pad28[0x3C];
    s32 serial;  /* 0x64: returned by queue_animation_update */
    s32 pending; /* 0x68: next_* waits for apply_pending_animation */
    u8  pad6C[0x24];
};

extern struct E hud_chan_slots[] __asm__("D_00199B60");

void set_animation_parameter(s32 anim_id, s32 value) __asm__("FUN_001ff570");

void set_animation_parameter(s32 anim_id, s32 requested_parameter_value) {
    s32       i;
    struct E *e;

    for (i = 0; i < 13; i++) {
        if (hud_chan_slots[i].serial == anim_id) {
            break;
        }
    }
    if (i < 13) {
        hud_chan_slots[i].next_mode = requested_parameter_value;
        if (hud_chan_slots[i].pending == 0) {
            hud_chan_slots[i].mode = requested_parameter_value;
        }
    }
}

extern __typeof__(set_animation_parameter) func_001FF570 __attribute__((alias("FUN_001ff570")));
