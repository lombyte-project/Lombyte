#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"

extern void copy_matrix_rotation(void *, void *) __asm__("func_001FA298");
extern void get_moby_joint_matrices(struct Moby *, s32, s32 *, s32) __asm__("FUN_00210850");
void        build_moby_bone_transform(struct Moby *moby,
                                      s32          bone_index,
                                      s32          output_transform) __asm__("FUN_0020cca8");

void build_moby_bone_transform(struct Moby *moby, s32 bone_index, s32 output_transform) {
    u8  sp_slot[0x40];
    s32 bone_index_input;
    f32 scale;
    s32 translation;

    bone_index_input = bone_index;
    scale = (*(f32 *)((u8 *)moby + 0x2C)) * 0.0009765625f;
    get_moby_joint_matrices(moby, 1, &bone_index_input, output_transform);
    translation = output_transform + 0x30;
    fast_vec_scale(translation, translation, scale);
    copy_matrix_rotation(sp_slot, ((u8 *)moby + (0xC0)));
    multiply_matrix(output_transform, sp_slot, output_transform);
    fast_vec_add(translation, translation, ((u8 *)moby + (0x10)));
}

extern __typeof__(build_moby_bone_transform) func_0020CCA8 __attribute__((alias("FUN_0020cca8")));
