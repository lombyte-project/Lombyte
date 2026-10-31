#include "types.h"

struct E {
    u8  pad0[0x4];
    s32 unk4;
    u8  pad8[0x1C];
    s32 unk24;
    u8  pad28[0x3C];
    s32 unk64;
    s32 unk68;
    u8  pad6C[0x24];
};

extern struct E hud_chan_slots[] __asm__("D_00199B60");

void set_animation_parameter(s32 anim_id, s32 value) __asm__("FUN_001ff570");

void set_animation_parameter(s32 anim_id, s32 requested_parameter_value) {
    s32       i;
    struct E *e;

    for (i = 0; i < 13; i++) {
        if (hud_chan_slots[i].unk64 == anim_id) {
            break;
        }
    }
    if (i < 13) {
        hud_chan_slots[i].unk24 = requested_parameter_value;
        if (hud_chan_slots[i].unk68 == 0) {
            hud_chan_slots[i].unk4 = requested_parameter_value;
        }
    }
}

extern __typeof__(set_animation_parameter) func_001FF570 __attribute__((alias("FUN_001ff570")));
