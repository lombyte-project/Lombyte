#include "types.h"

void decode_motion_vector(s32 *motion_vector,
                          s32  delta_shift,
                          s32  motion_code,
                          s32  motion_residual,
                          s32  use_half_scale) __asm__("FUN_001280a8");

void decode_motion_vector(

    s32 *motion_vector, s32 delta_shift, s32 motion_code, s32 motion_residual, s32 use_half_scale) {
    int new_var;
    s32 prediction;
    int new_var3;
    int new_var2;
    s32 range;
    s32 doubled;
    s32 vector;
    prediction = *motion_vector;
    new_var = 1;
    range = 0x10 << delta_shift;
    vector = (use_half_scale != 0) ? (prediction >> new_var) : (prediction);
    new_var3 = motion_code > 0;
    if (new_var3) {
        vector = (vector + new_var) + (((motion_code - new_var) << delta_shift) + motion_residual);
        doubled = vector * 2;
        if (vector >= range) {
            vector -= range * 2;
            goto block_6;
        }
    } else {
        doubled = vector * 2;
        if (motion_code < 0) {
            new_var2 = (~motion_code) << delta_shift;
            vector = (vector - new_var) - (new_var2 + motion_residual);
            if (vector < (-range)) {
                range = range * 2;
                new_var3 = range;
                vector += new_var3;
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
            new_var3 = use_half_scale != 0;
        }
        *motion_vector = (new_var3) ? (vector * 2) : (vector);
    } else {
        *motion_vector = (use_half_scale != 0) ? (vector * 2) : (vector);
    }
}

extern void func_001280A8(s32 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
    __attribute__((alias("FUN_001280a8")));
