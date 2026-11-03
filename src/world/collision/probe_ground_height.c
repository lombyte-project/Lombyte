/* Ported from rac1-decomp (src/game/mobyutil.c, func_00214358). */
typedef int   s32;
typedef float f32;

#include "qcopy.h"

extern int  collision_line(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern char D_00194120[];
f32         probe_ground_height(void *pos, s32 flags, f32 height_offset) __asm__("FUN_00213508");

f32 probe_ground_height(void *probe_position, s32 collision_mask, f32 height_offset) {
    f32 low_point[4];
    f32 raised_point[4];

    qcopy(low_point, probe_position);
    low_point[2] = 0.01f;
    qcopy(raised_point, probe_position);
    raised_point[2] = raised_point[2] + height_offset;
    if (collision_line(raised_point, low_point, collision_mask | 2, 0, 0) != 0) {
        return *(f32 *)(D_00194120 + 8);
    }
    return 0.0f;
}

extern __typeof__(probe_ground_height) func_00213508 __attribute__((alias("FUN_00213508")));
