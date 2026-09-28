#include "types.h"
struct M2c_D_0013F350 {
    u8 pad_0[0x12E4];
    u8 unk12E4;
    u8 pad_12E5[0xDA7];
    s32 unk208C;
};

extern struct M2c_D_0013F350 D_0013F350;
extern s32 D_001A03AC[];
extern s32 func_00208818();
s32 FUN_00207300(s32 arg0, s32 arg1, f32 fparg0, f32 fparg1, f32 fparg2) {
    s32 var_16_12;

    var_16_12 = 0;
    if ((u32) (D_0013F350.unk208C - 0x11) < 2U) {
        goto block_2;
    }
    if (D_0013F350.unk12E4 != 1) {
        goto block_3;
    }
block_2:
    var_16_12 = 1;
block_3:
    if (arg1 < 0x105) {
        goto block_7;
    }
    if (D_001A03AC[0] == 0) {
        return 0;
    }
    if (fparg2 >= 47.7f) {
        return 1;
    }
    return 0;
block_7:
    if (arg1 < 0xC1) {
        return var_16_12;
    }
    if (func_00208818(arg0, arg1, 0xD9, 0xB8, 0x156, 0xD2) != 0) {
        var_16_12 = 0;
    }
    return var_16_12;
}
