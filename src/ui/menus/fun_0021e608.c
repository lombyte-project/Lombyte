#include "types.h"
struct M2c_D_001D5BF4 {
    u8 pad_0[0x40];
    struct M2c_temp_2_12 * unk40;
};

struct M2c_arg0 {
    u8 pad_0[0x44];
    s32 unk44;
    s32 unk48;
};

struct M2c_temp_2_12 {
    u8 pad_0[0x3C];
    s32 unk3C;
    u8 pad_40[0x8];
    s32 unk48;
};

extern u8 D_0013D4C0[];
extern struct M2c_D_001D5BF4 *D_001D5BF4[];
extern s32 func_0020D330();
s32 FUN_0021e608(struct M2c_arg0 *arg0) {
    s32 temp_4_21;
    s32 temp_4_29;
    struct M2c_temp_2_12 *temp_2_12;

    temp_2_12 = D_001D5BF4[0]->unk40;
    if (*(*(s16 *)((u8 *)((temp_2_12->unk3C * 0xA) + temp_2_12->unk48) + 0x6) + D_0013D4C0) == 0) {
        goto block_2;
    }
    temp_4_21 = arg0->unk44;
    goto block_4;
block_2:
    return 0;
block_4:
    if (temp_4_21 == 0) {
        goto block_6;
    }
    func_0020D330(temp_4_21, 1);
block_6:
    temp_4_29 = arg0->unk48;
    if (temp_4_29 == 0) {
        goto block_8;
    }
    func_0020D330(temp_4_29, 1);
block_8:
    return 8;
}

/* ACCEPTED: attempt-2 (sn-O2) direct 100/100/100; spurious trailing call args dropped (callee FUN_0020D330 reads a0/a1 only). */
