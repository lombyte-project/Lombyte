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

s32 FUN_00206e18(s32 arg0, s32 arg1, f32 unused1, f32 unused2, f32 arg3) {
    struct M2c_D_0013F350 *s = &D_0013F350;
    s32 a = s->unk208C == 17 || s->unk208C == 18 || s->unk12E4 == 1;

    if (func_00208818(arg0, arg1, 0x93, 0x168, 0x182, 0x168) != 0
        && arg1 >= 0x135 && D_001A03A8[0] != 0 && arg3 >= 47.7f) {
        return 1;
    }
    if (!a) {
        return 0;
    }
    if (func_00208818(arg0, arg1, 0xC5, 0x9A, 0x13C, 0xE1) != 0
        && func_00208818(arg0, arg1, 0xD6, 0xC3, 0x157, 0xC5) != 0
        && func_00208818(arg0, arg1, 0x107, 0xDA, 0x171, 0xA0) != 0) {
        return 1;
    }
    return 0;
}
