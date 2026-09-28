#include "types.h"
struct M2c_D_0013F350 {
    u8 pad_0[0x12E4];
    u8 unk12E4;
    u8 pad_12E5[0xDA7];
    s32 unk208C;
};

extern struct M2c_D_0013F350 D_0013F350;
extern s32 func_00208818();
s32 FUN_002073b8(s32 arg0, s32 arg1, f32 fparg0, f32 fparg1, f32 fparg2) {
    s32 var_4_15;

    var_4_15 = 0;
    if (((u32) (D_0013F350.unk208C - 0x11) < 2U) || (D_0013F350.unk12E4 == 1)) {
        var_4_15 = 1;
    }
    if (var_4_15 != 0) {
        return 0;
    }
    if (fparg2 < 71.5f) {
        return 0;
    }
    if (func_00208818(arg0, arg1, 0x131, 0xE2, 0xC6, 0x93) != 0) {
        return 0;
    }
    return func_00208818(arg0, arg1, 0x190, 0x89, 0xD1, 0xFB) == 0;
}
