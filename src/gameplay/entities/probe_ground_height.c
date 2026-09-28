/* Ported from rac1-decomp, the PAL decompilation (src/game/mobyutil.c, func_00214358). */
typedef int s32;
typedef float f32;
#include "qcopy.h"
extern int FUN_001efa68(void *, void *, int, int, int);
extern char D_00194120[];
/* Builds two 16-byte copies of *arg0: one with byte offset 8 (a float)
   forced to 0.01f, the other with its offset-8 float bumped by arg2.
   Passes both to FUN_001efa68 (a collision/line test elsewhere in the
   file's neighbours); returns D_00194120's float at +8 on success, else
   0.0f. */
f32 probe_ground_height(void *arg0, s32 arg1, f32 arg2) __asm__("FUN_00213508");

f32 probe_ground_height(void *arg0, s32 arg1, f32 arg2) {
    char sp0[16];
    char sp1[16];

    qcopy(sp0, arg0);
    *(f32 *)(sp0 + 8) = 0.01f;
    qcopy(sp1, arg0);
    *(f32 *)(sp1 + 8) = *(f32 *)(sp1 + 8) + arg2;
    if (FUN_001efa68(sp1, sp0, arg1 | 2, 0, 0) != 0) {
        return *(f32 *)(D_00194120 + 8);
    }
    return 0.0f;
}

extern __typeof__(probe_ground_height) func_00213508 __attribute__((alias("FUN_00213508")));
