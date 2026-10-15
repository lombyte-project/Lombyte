#include "types.h"
#include "rnc/gameplay/entities/moby.h"

extern void add_vector_xyz(s32, s32, s32) __asm__("FUN_001f9a10");
extern void fast_vec_scale(s32, s32, f32) __asm__("FUN_001f9a68");
extern void copy_matrix_rotation(void *, void *) __asm__("func_001FA298");
extern void multiply_matrix(s32, void *, s32) __asm__("FUN_001fa378");
extern void FUN_00210850(struct Moby *, s32, s32 *, s32);
void build_moby_bone_transform(struct Moby *arg0, s32 arg1, s32 arg2) __asm__("FUN_0020cca8");

void build_moby_bone_transform(struct Moby *arg0, s32 arg1, s32 arg2) {
    u8 sp_slot[0x40];
    s32 sp40;
    f32 scale;
    s32 temp_16_22;

    sp40 = arg1;
    scale = (*(f32 *)((u8 *)arg0 + 0x2C)) * 0.0009765625f;
    FUN_00210850(arg0, 1, &sp40, arg2);
    temp_16_22 = arg2 + 0x30;
    fast_vec_scale(temp_16_22, temp_16_22, scale);
    copy_matrix_rotation(sp_slot, ((u8 *)arg0 + (0xC0)));
    multiply_matrix(arg2, sp_slot, arg2);
    add_vector_xyz(temp_16_22, temp_16_22, ((u8 *)arg0 + (0x10)));
}

extern __typeof__(build_moby_bone_transform) func_0020CCA8 __attribute__((alias("FUN_0020cca8")));
