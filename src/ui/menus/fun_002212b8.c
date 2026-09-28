#include "types.h"
struct M2c_D_0013C940 {
    u8 pad_0[0x1C4];
    s32 unk1C4;
};

struct M2c_D_001D5BF0 {
    u8 pad_0[0x4];
    struct M2c_D_001D5BF0_unk4 * unk4;
    s32 unk8;
    u8 pad_C[0x118];
    s32 unk124;
};

struct M2c_D_001D5BF0_unk4 {
    u8 pad_0[0x38];
    s32 unk38;
    u8 pad_3C[0x4];
    s32 unk40;
};

struct M2c_arg0 {
    u8 pad_0[0x14];
    s32 unk14;
    u8 pad_18[0x1C];
    s32 unk34;
    s32 unk38;
};

struct M2c_temp_4_79 {
    u8 pad_0[0x4];
    s32 unk4;
    s32 unk8;
};

extern struct M2c_D_0013C940 D_0013C940;
extern struct M2c_D_001D5BF0 D_001D5BF0;
extern s32 func_0022DA68();
s32 FUN_002212b8(struct M2c_arg0 *arg0) {
    s32 *var_3_83;
    s32 temp_2_34;
    s32 temp_5_58;
    s32 temp_6_48;
    s32 var_7_77;
    u8 *temp_6_98;
    struct M2c_temp_4_79 *temp_4_79;

    if (D_001D5BF0.unk4->unk40 != arg0) {
        return 0;
    }
    if ((D_0013C940.unk1C4 & 0xD00) && (D_001D5BF0.unk124 == 0)) {
        return 1;
    }
    if (D_0013C940.unk1C4 & 0x10) {
        temp_2_34 = D_001D5BF0.unk4->unk38;
        if (temp_2_34 != 0) {
            D_001D5BF0.unk8 = temp_2_34;
        } else if (D_001D5BF0.unk124 == 0) {
            return -1;
        }
    }
    temp_6_48 = arg0->unk38;
    if ((D_0013C940.unk1C4 & 0x1000) && (0 != temp_6_48)) {
        arg0->unk38 = (s32) (temp_6_48 - 1);
    }
    if (D_0013C940.unk1C4 & 0x4000) {
        temp_5_58 = arg0->unk38;
        if (*(s32 *)((u8 *)((temp_5_58 * 0x18) + arg0->unk34) + 0x18) != 0) {
            arg0->unk38 = (s32) (temp_5_58 + 1);
        }
    }
    if (temp_6_48 != arg0->unk38) {
        func_0022DA68(1, 0x11, arg0->unk14);
    }
    temp_4_79 = (struct M2c_temp_4_79 *) (arg0->unk34 + (arg0->unk38 * 0x18));
    var_7_77 = 0;
    __asm__ __volatile__("" : "+r"(var_7_77));
    if (temp_4_79->unk8 != 0) {
        var_3_83 = (s32 *) ((u8 *) temp_4_79 + 8);
        __asm__ __volatile__("" : "+r"(var_3_83));
        var_3_83 = (s32 *) ((u8 *) var_3_83 + 4);
loop_21:
        var_7_77 += 1;
        if (*var_3_83 != 0) {
            var_3_83 += 1;
            if (var_7_77 < 4) {
                goto loop_21;
            }
        }
    }
    if (D_0013C940.unk1C4 & 0x40) {
        temp_6_98 = temp_4_79->unk4;
        if (temp_6_98 != NULL) {
            *temp_6_98 = (u8) ((s32) (*temp_6_98 + 1) % var_7_77);
            func_0022DA68(0, 0x11, arg0->unk14);
        }
    }
    return 0;
}
