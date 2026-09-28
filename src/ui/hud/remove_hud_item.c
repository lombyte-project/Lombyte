#include "types.h"
struct M2c_D_00199B60 {
    u8 pad_0[0x64];
    s32 unk64;
};

extern struct M2c_D_00199B60 D_00199B60;
extern s32 func_001FF308(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);


s32 remove_hud_item(s32 arg0) __asm__("FUN_001ff480");

s32 remove_hud_item(s32 arg0) {
    s32 *var_3_11;
    s32 var_4_10;

    var_4_10 = 0;
    if (D_00199B60.unk64 != arg0) {
        var_3_11 = (s32 *)((u8 *)&D_00199B60 + 0x64);
loop_2:
        var_4_10 += 1;
        var_3_11 = (s32 *)((u8 *)var_3_11 + 0x90);
        if (var_4_10 < 0xD) {
            if (*var_3_11 == arg0) {
                goto block_4;
            }
            goto loop_2;
        }
        goto block_5;
    }
block_4:
    if (var_4_10 >= 0xD) {
block_5:
        return 0;
    }
    func_001FF308(var_4_10, 0xFFFF, 0, 0, 0, 0, 0);
    return 1;
}
