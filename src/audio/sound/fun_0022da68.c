#include "types.h"
struct M2c_arg2 {
    u8 pad_0[0x24];
    struct M2c_temp_2_11 * unk24;
};

struct M2c_temp_2_11 {
    u8 pad_0[0xD];
    u8 unkD;
    u8 pad_E[0x1A];
    s32 unk28;
};

struct M2c_temp_3_37 {
    u8 pad_0[0x7E];
    s16 unk7E;
    u8 pad_80[0x8];
    s32 unk88;
};

extern u8 D_0013E550[];
extern s32 func_0022D7F0();
s32 FUN_0022da68(s32 arg0, s32 arg1, struct M2c_arg2 *arg2) {
    struct M2c_temp_2_11 *temp_2_11;
    s32 temp_2_29;
    s32 temp_3_14;
    struct M2c_temp_3_37 *temp_3_37;

    if (arg2 == NULL) {
        return -1;
    }
    temp_2_11 = arg2->unk24;
    if (temp_2_11 == NULL) {
        return -1;
    }
    temp_3_14 = temp_2_11->unk28;
    if (temp_3_14 == 0) {
        return -1;
    }
    if (arg0 >= (s32) temp_2_11->unkD) {
        return -1;
    }
    temp_2_29 = func_0022D7F0(temp_3_14 + (arg0 << 5), arg1, arg2, 0, 0x400);
    if (temp_2_29 >= 0) {
        temp_3_37 = (struct M2c_temp_3_37 *)((temp_2_29 * 0x70) + (s32) D_0013E550);
        temp_3_37->unk88 = arg2;
        temp_3_37->unk7E = arg0;
    }
    return temp_2_29;
}

extern __typeof__(FUN_0022da68) func_0022DA68 __attribute__((alias("FUN_0022da68")));
