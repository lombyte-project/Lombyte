#include "types.h"
#include "rnc/globals.h"
struct LevelMapTransform {
    f32 offset_x;
    f32 scale_x;
    f32 offset_y;
    f32 scale_y;
};
extern struct LevelMapTransform level_map_transforms[] __asm__("D_0019E970");
void world_to_map_coords(f32 x, f32 y, f32 *map_x, f32 *map_y, s32 level) __asm__("FUN_00208408");

void world_to_map_coords(f32 x, f32 y, f32 *map_x, f32 *map_y, s32 level) {
    s32 rotated;

    rotated = 0;
    if (level >= 100) {
        level -= 100;
        rotated = 1;
    }
    if (level == -1) {
        level = current_level_index;
    }
    if (level < 0) {
        level = 0;
    }
    if (level >= 19) {
        level = 0;
    }
    if (level == 6 && rotated) {
        *map_x = (level_map_transforms[6].scale_y * y + 1053.0f) * 0.001953125f;
        *map_y = (740.0f - level_map_transforms[6].scale_x * x) * 0.001953125f;
        return;
    }
    *map_x = (level_map_transforms[level].offset_x + level_map_transforms[level].scale_x * x) *
             0.001953125f;
    *map_y = (level_map_transforms[level].offset_y + level_map_transforms[level].scale_y * y) *
             0.001953125f;
}

extern __typeof__(world_to_map_coords) func_00208408 __attribute__((alias("FUN_00208408")));
