#include "types.h"
struct M2c_D_001516D0 {
    u8 pad_0[0x5A];
    u16 unk5A;
};

struct M2c_arg0 {
    u8 pad_0[0x3C];
    s32 unk3C;
};

extern struct M2c_D_001516D0 D_001516D0;
extern s32 FUN_00225530();
extern s32 func_00225CD8();
s32 FUN_00225660(struct M2c_arg0 *arg0) {
    s32 *var_16_10;
    s32 var_17_8;

    var_17_8 = 0x17;
    var_16_10 = ((u8 *)arg0 + (0x44));
    do {
        var_17_8 -= 1;
        *var_16_10 = FUN_00225530(*var_16_10);
        var_16_10 += 1;
    } while (var_17_8 >= 0);
    arg0->unk3C = func_00225CD8(arg0->unk3C);
    if ((u32) (D_001516D0.unk5A - 6) >= 2U) {
        D_001516D0.unk5A = 5U;
    }
    return 0;
}
