#include "types.h"
extern s32 parse_occlusion_grid() __asm__("FUN_001f2690");
void get_occlusion_grid_from_pair(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5,
                                  f32 fparg0) __asm__("FUN_001f2768");

void get_occlusion_grid_from_pair(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5,
                                  f32 fparg0) {
    s32 var_4_25;
    s32 var_5_26;
    s32 var_6_28;

    if (fparg0 < 0.5f) {
        if (parse_occlusion_grid() == 0) {
            var_4_25 = arg3;
            var_5_26 = arg4;
            var_6_28 = arg5;
            goto block_5;
        }
    } else if (parse_occlusion_grid(arg3, arg4, arg5) == 0) {
        var_4_25 = arg0;
        var_5_26 = arg1;
        var_6_28 = arg2;
    block_5:
        parse_occlusion_grid(var_4_25, var_5_26, var_6_28);
    }
}

extern void func_001F2768(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, f32 fparg0)
    __attribute__((alias("FUN_001f2768")));

/* Recovered original symbol name. */
extern __typeof__(get_occlusion_grid_from_pair) GetOcclGridFromPair__Fiiiiiif
    __attribute__((alias("FUN_001f2768")));
