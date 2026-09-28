#include "types.h"
struct M2c_arg0 {
    u8 pad_0[0x7C];
    u8 unk7C;
    u8 unk7D;
};
struct M2c_temp_3_16 {
    u8 pad_0[0x7E];
    s16 unk7E;
    u8 pad_80[0x8];
    s32 unk88;
};

extern u8 D_0013E550[];
extern void release_voice_slot(s32) __asm__("FUN_0022d798");
extern s32 func_0022DA68();

void update_moby_voice(struct M2c_arg0 *arg0) __asm__("FUN_0020c940");

void update_moby_voice(struct M2c_arg0 *arg0) {
    s32 idx;
    struct M2c_temp_3_16 *e;

    if (arg0->unk7D != 0xFF) {
        idx = arg0->unk7D;
        e = (struct M2c_temp_3_16 *)(idx * 0x70 + D_0013E550);
        if (e->unk88 != (s32)arg0) {
            arg0->unk7D = 0xFF;
        } else if (e->unk7E != arg0->unk7C) {
            release_voice_slot(idx);
            arg0->unk7D = 0xFF;
        }
    } else if (arg0->unk7C != 0xFF) {
        arg0->unk7D = func_0022DA68(arg0->unk7C, 4, arg0);
    }
}

extern __typeof__(update_moby_voice) func_0020C940 __attribute__((alias("FUN_0020c940")));
