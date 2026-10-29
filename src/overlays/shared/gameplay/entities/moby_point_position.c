/* Level overlay code: stubs become C as functions are matched. */
#include "types.h"
#include "asm.h"
#include "rnc/math/fast_math.h"



/* Ported from rac1-decomp (src/overlays/shared/mobyfunc_0024FD50.c: func_L00_00250800), where it is exact; names translated to the US level program. */

extern void transform_vector_by_basis(void *, void *, void *) __asm__("FUN_001f9cf8");
extern void get_moby_joint_matrices(void *, int, int *, void *) __asm__("FUN_00210850");

/* Writes into arg2 the 0x30 point of joint arg1, scaled by 0x2C/1024 and moved to world space. */
void moby_point_position(char *moby, int joint_index, void *world_position_out) __asm__("FUN_L00_0024f7c8");
void moby_point_position(char *moby, int joint_index, void *world_position_out) {
    char joint_point_data[0x40];
    int joint_index_copy;
    float point_scale;

    point_scale = *(float *)(moby + 0x2C) * 0.0009765625f;
    joint_index_copy = joint_index;
    get_moby_joint_matrices(moby, 1, &joint_index_copy, joint_point_data);
    fast_vec_scale(world_position_out, joint_point_data + 0x30, point_scale);
    transform_vector_by_basis(world_position_out, world_position_out, moby + 0xC0);
    fast_vec_add(world_position_out, world_position_out, moby + 0x10);
}
