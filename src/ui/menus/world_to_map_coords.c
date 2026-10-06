#include "types.h"
struct ViewParams {
    f32 ox;
    f32 sx;
    f32 oy;
    f32 sy;
};
extern s32 D_0015ED84;
extern struct ViewParams D_0019E970[];
void world_to_map_coords(f32 *outx, f32 *outy, s32 view, f32 x, f32 y) __asm__("FUN_00208408");

void world_to_map_coords(f32 *outx, f32 *outy, s32 view, f32 x, f32 y) {
    s32 alt;

    alt = 0;
    if (view >= 100) {
        view -= 100;
        alt = 1;
    }
    if (view == -1) {
        view = D_0015ED84;
    }
    if (view < 0) {
        view = 0;
    }
    if (view >= 19) {
        view = 0;
    }
    if (view == 6 && alt) {
        *outx = (D_0019E970[6].sy * y + 1053.0f) * 0.001953125f;
        *outy = (740.0f - D_0019E970[6].sx * x) * 0.001953125f;
        return;
    }
    *outx = (D_0019E970[view].ox + D_0019E970[view].sx * x) * 0.001953125f;
    *outy = (D_0019E970[view].oy + D_0019E970[view].sy * y) * 0.001953125f;
}

extern __typeof__(world_to_map_coords) func_00208408 __attribute__((alias("FUN_00208408")));
