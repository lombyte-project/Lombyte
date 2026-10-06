#include "types.h"
struct E {
    u8 pad0[0x4];
    s32 unk4;
    u8 pad8[0x1C];
    s32 unk24;
    u8 pad28[0x3C];
    s32 unk64;
    s32 unk68;
    u8 pad6C[0x24];
};
extern struct E D_00199B60[];

void set_animation_parameter(s32 anim_id, s32 value) __asm__("FUN_001ff570");

void set_animation_parameter(s32 anim_id, s32 value) {
    s32 i;
    struct E *e;

    for (i = 0; i < 13; i++) {
        if (D_00199B60[i].unk64 == anim_id) {
            break;
        }
    }
    if (i < 13) {
        D_00199B60[i].unk24 = value;
        if (D_00199B60[i].unk68 == 0) {
            D_00199B60[i].unk4 = value;
        }
    }
}

extern __typeof__(set_animation_parameter) func_001FF570 __attribute__((alias("FUN_001ff570")));
