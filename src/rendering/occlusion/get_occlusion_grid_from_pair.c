#include "types.h"
extern s32 parse_occlusion_grid() __asm__("FUN_001f2690");
void get_occlusion_grid_from_pair(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5,
                                  f32 fparg0) __asm__("FUN_001f2768");

void get_occlusion_grid_from_pair(s32 first_grid_x, s32 first_grid_y, s32 first_grid_z, s32 second_grid_x, s32 second_grid_y, s32 second_grid_z,
                                  f32 grid_axis_fraction) {
    s32 fallback_grid_x;
    s32 fallback_grid_y;
    s32 fallback_grid_z;

    if (grid_axis_fraction < 0.5f) {
        if (parse_occlusion_grid() == 0) {
            fallback_grid_x = second_grid_x;
            fallback_grid_y = second_grid_y;
            fallback_grid_z = second_grid_z;
            goto block_5;
        }
    } else if (parse_occlusion_grid(second_grid_x, second_grid_y, second_grid_z) == 0) {
        fallback_grid_x = first_grid_x;
        fallback_grid_y = first_grid_y;
        fallback_grid_z = first_grid_z;
    block_5:
        parse_occlusion_grid(fallback_grid_x, fallback_grid_y, fallback_grid_z);
    }
}

extern void func_001F2768(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, f32 fparg0)
    __attribute__((alias("FUN_001f2768")));

/* Recovered original symbol name. */
extern __typeof__(get_occlusion_grid_from_pair) GetOcclGridFromPair__Fiiiiiif
    __attribute__((alias("FUN_001f2768")));
