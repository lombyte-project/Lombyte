/* Level overlay code: stubs become C as functions are matched. */
#include "types.h"
#include "asm.h"

#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/mobyfunc_0024FD50.c: func_L00_00250800), where it is exact; names translated to the US level program. */

extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("FUN_001f9cf8");
extern void FUN_00210850(void *, int, int *, void *);

/* Writes into arg2 the 0x30 point of joint arg1, scaled by 0x2C/1024 and moved to world space. */
void moby_point_position(char *arg0, int arg1, void *arg2) __asm__("FUN_L00_0024f7c8");
void moby_point_position(char *moby, int joint_index, void *world_position_out) {
    char buf[0x40];
    int n;
    float s;

    s = *(float *)(moby + 0x2C) * 0.0009765625f;
    n = joint_index;
    FUN_00210850(moby, 1, &n, buf);
    fast_vec_scale(world_position_out, buf + 0x30, s);
    transform_vector_by_basis(world_position_out, world_position_out, moby + 0xC0);
    add_vector_xyz(world_position_out, world_position_out, moby + 0x10);
}
