#include "types.h"
struct M2c_D_0013F350 {
    u8 pad_0[0x12E4];
    u8 unk12E4;
    u8 pad_12E5[0xDA7];
    s32 unk208C;
};

extern struct M2c_D_0013F350 D_0013F350;
extern u32 D_001A03BC[];
extern s32 func_00208818();
s32 FUN_00207100(s32 arg0, s32 arg1, f32 fparg0, f32 fparg1, f32 fparg2) {
    register s32 var_2_22 __asm__("v0");
    register s32 var_3_26 __asm__("v1");
    s32 var_4_13;

    if (arg1 < 0xBB) {
        var_4_13 = 0;
        if (((u32) (D_0013F350.unk208C - 0x11) < 2U) || (D_0013F350.unk12E4 == 1)) {
            var_4_13 = 1;
        }
        var_2_22 = 0;
        if (var_4_13 == 0) {
            var_2_22 = 1;
            var_3_26 = D_001A03BC[0];
            goto block_9;
        }
    } else {
        var_2_22 = 0;
        if ((fparg2 >= 51.5f) && (fparg2 <= 54.0f)) {
            var_3_26 = func_00208818(arg0, arg1, 0x10A, 0xE5, 0x124, 0xF9);
            var_2_22 = 1;
block_9:
            if (var_3_26 != 0) {
                goto block_keep;
            }
            goto block_zero;
        }
    }
block_zero:
    var_2_22 = 0;
block_keep:
    return var_2_22;
}
