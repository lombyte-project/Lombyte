#include "types.h"

void decode_motion_vector(s32 *arg0, s32 arg1, s32 arg2, s32 arg3,
                          s32 arg4) __asm__("FUN_001280a8");

void decode_motion_vector(s32 *motion_vector, s32 delta_shift, s32 motion_code, s32 motion_residual, s32 arg4) {
    int new_var;
    s32 temp_4_5;
    int new_var3;
    int new_var2;
    s32 temp_9_6;
    s32 var_2_17;
    s32 var_4_9;
    temp_4_5 = *motion_vector;
    new_var = 1;
    temp_9_6 = 0x10 << delta_shift;
    var_4_9 = (arg4 != 0) ? (temp_4_5 >> new_var) : (temp_4_5);
    new_var3 = motion_code > 0;
    if (new_var3) {
        var_4_9 = (var_4_9 + new_var) + (((motion_code - new_var) << delta_shift) + motion_residual);
        var_2_17 = var_4_9 * 2;
        if (var_4_9 >= temp_9_6) {
            var_4_9 -= temp_9_6 * 2;
            goto block_6;
        }
    } else {
        var_2_17 = var_4_9 * 2;
        if (motion_code < 0) {
            new_var2 = (~motion_code) << delta_shift;
            var_4_9 = (var_4_9 - new_var) - (new_var2 + motion_residual);
            if (var_4_9 < (-temp_9_6)) {
                temp_9_6 = temp_9_6 * 2;
                new_var3 = temp_9_6;
                var_4_9 += new_var3;
            }
        block_6:;

            ;
            ;
            ;
            ;
        }
    }
    if (motion_code) {
        {
            new_var3 = arg4 != 0;
        }
        *motion_vector = (new_var3) ? (var_4_9 * 2) : (var_4_9);
    } else {
        *motion_vector = (arg4 != 0) ? (var_4_9 * 2) : (var_4_9);
    }
}

extern void func_001280A8(s32 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
    __attribute__((alias("FUN_001280a8")));
