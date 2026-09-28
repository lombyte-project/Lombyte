#include "types.h"
struct M2c_D_0013C940 {
    u8 pad_0[0x1A4];
    s32 unk1A4;
    u8 pad_1A8[0x1C];
    s32 unk1C4;
};
struct M2c_D_001D5BF0_unk4 {
    u8 pad_0[0x38];
    s32 unk38;
};
struct M2c_D_001D5BF0 {
    u8 pad_0[0x4];
    struct M2c_D_001D5BF0_unk4 * unk4;
    s32 unk8;
    u8 pad_C[0x118];
    s32 unk124;
};
struct M2c_arg0 {
    u8 pad_0[0x14];
    s32 unk14;
    u8 pad_18[0x3C];
    s32 unk54;
};

extern struct M2c_D_0013C940 D_0013C940;
extern struct M2c_D_001D5BF0 D_001D5BF0;
extern s32 D_001D5D14[];
extern s32 func_0022DA68();
s32 FUN_00221e50(struct M2c_arg0 *arg0) {
    register struct M2c_arg0 *p __asm__("t0");
    register s32 temp_hi_49 __asm__("a3");
    register s32 temp_hi_67 __asm__("a3");
    s32 temp_3_27;

    p = arg0;

    if (D_0013C940.unk1C4 & 0xD00) {
        if (D_001D5D14[0] == 0) {
            return 1;
        }
        goto block_4;
    }
block_4:
    if (D_0013C940.unk1C4 & 0x10) {
        temp_3_27 = D_001D5BF0.unk4->unk38;
        if (temp_3_27 != 0) {
            D_001D5BF0.unk8 = temp_3_27;
            goto block_10;
        }
        if (D_001D5BF0.unk124 == 0) {
            return -1;
        }
        goto block_11;
    }
block_10:
block_11:
    if (D_0013C940.unk1A4 & 0x2040) {
        temp_hi_49 = (s32) (p->unk54 + 1) % 12;
        p->unk54 = temp_hi_49;
        func_0022DA68(1, 0x11, p->unk14, temp_hi_49, p);
    } else if (D_0013C940.unk1A4 & 0x8020) {
        temp_hi_67 = (s32) (p->unk54 + 0xB) % 12;
        p->unk54 = temp_hi_67;
        func_0022DA68(1, 0x11, p->unk14, temp_hi_67, p);
    }
    return 0;
}
