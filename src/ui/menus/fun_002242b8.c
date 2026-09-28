#include "types.h"
struct M2c_D_001D5BF0 {
    u8 pad_0[0xA0];
    s32 unkA0;
    s32 unkA4;
    u8 pad_A8[0x20];
    u8 unkC8;
    u8 unkC9;
    u8 unkCA;
};

extern struct M2c_D_001D5BF0 D_001D5BF0;
extern s32 FUN_00225530();
extern s32 func_00225CD8();
extern s32 FUN_002267b8();

s32 FUN_002242b8(s32 arg0) {
    s32 *var_16_8;
    s32 *var_17_24;
    s32 var_17_6;
    s32 var_18_20;
    struct M2c_D_001D5BF0 *base;

    var_17_6 = 0x17;
    var_16_8 = arg0 + 0x44;
    do {
        var_17_6 -= 1;
        *var_16_8 = FUN_00225530(*var_16_8);
        var_16_8 += 1;
    } while (var_17_6 >= 0);
    var_18_20 = 2;
    base = &D_001D5BF0;
    base->unkA0 = func_00225CD8(base->unkA0);
    base->unkA4 = func_00225CD8(base->unkA4);
    base->unkC8 = 0xFF;
    base->unkC9 = 0xFF;
    base->unkCA = 0;
    FUN_002267b8();
    var_17_24 = ((u8 *)base + 0xB0);
    do {
        var_18_20 -= 1;
        *var_17_24 = func_00225CD8(*var_17_24);
        var_17_24 += 1;
    } while (var_18_20 >= 0);
    return 0;
}
