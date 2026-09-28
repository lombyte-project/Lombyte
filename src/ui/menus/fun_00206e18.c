#include "types.h"

struct M2c_D_0013F350 {
    u8 pad_0[0x12E4];
    u8 unk12E4;
    u8 pad_12E5[0xDA7];
    s32 unk208C;
};

extern struct M2c_D_0013F350 D_0013F350;
extern s32 D_001A03A8[];
extern s32 func_00208818();

s32 FUN_00206e18(s32 arg0, s32 arg1, f32 fparg0, f32 fparg1, f32 fparg2) {
    s32 var_18_8;
    register s32 last asm("v1");

    var_18_8 = 0;
    if (((u32) (D_0013F350.unk208C - 0x11) < 2U) || (D_0013F350.unk12E4 == 1)) {
        var_18_8 = 1;
    }
    if (!(func_00208818(arg0, arg1, 0x93, 0x168, 0x182, 0x168) != 0 && arg1 >= 0x135 && D_001A03A8[0] != 0 && fparg2 >= 47.7f)) {
        if (var_18_8 != 0 && func_00208818(arg0, arg1, 0xC5, 0x9A, 0x13C, 0xE1) != 0 && func_00208818(arg0, arg1, 0xD6, 0xC3, 0x157, 0xC5) != 0) {
            last = func_00208818(arg0, arg1, 0x107, 0xDA, 0x171, 0xA0);
            __asm__ volatile ("" : "+r"(last));
            if (last == 0) {
                return 0;
            }
        } else {
            return 0;
        }
    }
    return 1;
}
