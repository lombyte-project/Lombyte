#include "types.h"
#include "rnc/gameplay/entities/moby.h"
#include "rnc/math/fast_math.h"

extern void copy_matrix_rotation(void *, void *) __asm__("func_001FA298");
extern void multiply_matrix(s32, void *, s32) __asm__("FUN_001fa378");
extern void get_moby_joint_matrices(struct Moby *, s32, s32 *, s32) __asm__("FUN_00210850");
void build_moby_bone_transform(struct Moby *moby, s32 bone_index, s32 output_transform) __asm__("FUN_0020cca8");

void build_moby_bone_transform(struct Moby *moby, s32 bone_index, s32 output_transform) {
    u8 sp_slot[0x40];
    s32 bone_index_input;
    f32 scale;
    s32 temp_16_22;

    bone_index_input = bone_index;
    scale = (*(f32 *)((u8 *)moby + 0x2C)) * 0.0009765625f;
    get_moby_joint_matrices(moby, 1, &bone_index_input, output_transform);
    temp_16_22 = output_transform + 0x30;
    fast_vec_scale(temp_16_22, temp_16_22, scale);
    copy_matrix_rotation(sp_slot, ((u8 *)moby + (0xC0)));
    multiply_matrix(output_transform, sp_slot, output_transform);
    fast_vec_add(temp_16_22, temp_16_22, ((u8 *)moby + (0x10)));
}

extern __typeof__(build_moby_bone_transform) func_0020CCA8 __attribute__((alias("FUN_0020cca8")));
