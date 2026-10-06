/* Ported from rac1-decomp (src/game/mobyutil.c, func_00214358). */
typedef int s32;
typedef float f32;
#include "qcopy.h"
extern int FUN_001efa68(void *, void *, int, int, int);
extern char D_00194120[];
/* Builds two 16-byte copies of *pos: one with byte offset 8 (a float)
   forced to 0.01f, the other with its offset-8 float bumped by height_offset.
   Passes both to FUN_001efa68 (a collision/line test elsewhere in the
   file's neighbours); returns D_00194120's float at +8 on success, else
   0.0f. */
f32 probe_ground_height(void *pos, s32 flags, f32 height_offset) __asm__("FUN_00213508");

f32 probe_ground_height(void *pos, s32 flags, f32 height_offset) {
    char low_point[16];
    char raised_point[16];

    qcopy(low_point, pos);
    *(f32 *)(low_point + 8) = 0.01f;
    qcopy(raised_point, pos);
    *(f32 *)(raised_point + 8) = *(f32 *)(raised_point + 8) + height_offset;
    if (FUN_001efa68(raised_point, low_point, flags | 2, 0, 0) != 0) {
        return *(f32 *)(D_00194120 + 8);
    }
    return 0.0f;
}

extern __typeof__(probe_ground_height) func_00213508 __attribute__((alias("FUN_00213508")));
