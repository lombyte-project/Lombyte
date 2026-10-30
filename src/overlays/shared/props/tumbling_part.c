/* Shared moby class robot_arm (also the hoverboard and other parts): a broken-off part that tumbles, bounces and then explodes or fades out. */

#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/quaternions/quaternion_to_matrix.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/math/rotations/matrix_to_euler.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"
#include "rnc/overlay/collision.h"

extern float scale_time(float) __asm__("FUN_001f96b0");
extern void  normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void  spawn_explosion(void *,
                             void *,
                             void *,
                             float,
                             float,
                             int,
                             float,
                             int,
                             float,
                             float,
                             int,
                             float,
                             int,
                             float,
                             int,
                             int,
                             int,
                             int) __asm__("FUN_L00_0025e450");
extern void
spawn_color_swapped_explosion_burst(void *, void *, int, float, float) __asm__("FUN_L00_0025f3e8");
extern void build_quaternion_from_axis_angle(void *, void *, f32) __asm__("FUN_00214530");
extern int  func_001F9908_r(int *) __asm__("FUN_001f9740");
extern int  func_L00_001F10E0_s(void *, float, int, void *) __asm__("FUN_L00_001f0d60");

extern Vec4 zero_vector __asm__("D_L00_0015F580") MACRO_ADDR;

/* Update of an effect-group moby: state machine over its life, then it steers a sprite along its path and bounces it off surfaces. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002E1660.c: func_L00_002E5238), where it is exact; names translated to the US level program. */
void robot_arm_update(char *m) __asm__("FUN_L00_002e3d88");

void robot_arm_update(char *moby) {
    enum {
        ROBOT_ARM_REMOVE = 0,
        ROBOT_ARM_WAIT_FOR_TIMER = 1,
        ROBOT_ARM_SPAWN_EFFECT_AND_REMOVE = 2,
        ROBOT_ARM_FADE_AND_REMOVE = 3
    };
    char  *moby_data = ((char *)MOBY(moby)->pvars);
    float  effect_offset_or_world_position[4];
    float  rotation_step_basis[12];
    float  offset_after_rotation[4];
    float  rotation_offset_delta[4];
    float  collision_probe_position[4];
    float  bounce_rotation_matrix[16];
    float  aD0[4];
    float  aE0[4];
    char  *rotation_step_euler;
    float  collision_radius;
    float *collision_normal;

    switch (MOBY(moby)->state) {
    case ROBOT_ARM_REMOVE:
        mark_moby_for_removal(moby);
        return;
    case ROBOT_ARM_WAIT_FOR_TIMER:
        if (func_001F9908_r((int *)(moby_data + 0x30))) {
            moby[0x20] = (*(int *)(moby_data + 0x34) & 1) ? 3 : 2;
            *(int *)(moby_data + 0x30) = truncate_float_to_s32(scale_time(10.0f));
            return;
        }
        break;
    case ROBOT_ARM_SPAWN_EFFECT_AND_REMOVE:
        transform_vector_by_basis(effect_offset_or_world_position, moby_data + 0x20, moby + 0xC0);
        fast_vec_add(effect_offset_or_world_position, effect_offset_or_world_position, moby + 0x10);
        if (*(int *)(moby_data + 0x34) & 2) {
            spawn_color_swapped_explosion_burst(moby,
                                                effect_offset_or_world_position,
                                                -1,
                                                *(float *)(moby_data + 0x2C) * 0.5f,
                                                0.0f);
        } else {
            float half_collision_radius = *(float *)(moby_data + 0x2C) * 0.5f;
            spawn_explosion(moby,
                            &zero_vector,
                            effect_offset_or_world_position,
                            0.0f,
                            0.0f,
                            5,
                            *(float *)(moby_data + 0x2C),
                            3,
                            half_collision_radius,
                            9.0f,
                            5,
                            half_collision_radius,
                            -1,
                            0.0f,
                            0,
                            0,
                            -1,
                            0);
        }
        mark_moby_for_removal(moby);
        return;
    case ROBOT_ARM_FADE_AND_REMOVE:
        if (func_001F9908_r((int *)(moby_data + 0x30))) {
            mark_moby_for_removal(moby);
            return;
        }
        moby[0x23] =
            truncate_float_to_s32((float)*(int *)(moby_data + 0x30) / scale_time(10.0f) * 128.0f);
        break;
    }
    rotation_step_euler = moby_data + 0x10;
    euler_rotation_basis(rotation_step_basis, rotation_step_euler);
    transform_vector_by_basis(effect_offset_or_world_position, moby_data + 0x20, moby + 0xC0);
    multiply_matrix3x3(moby + 0xC0, rotation_step_basis, moby + 0xC0);
    transform_vector_by_basis(offset_after_rotation, moby_data + 0x20, moby + 0xC0);
    fast_vec_sub(rotation_offset_delta, offset_after_rotation, effect_offset_or_world_position);
    fast_vec_sub((float *)&MOBY(moby)->pos, (float *)&MOBY(moby)->pos, rotation_offset_delta);
    fast_vec_add(moby + 0x10, moby + 0x10, moby_data);
    collision_radius = *(float *)(moby_data + 0x2C);
    *(float *)(moby_data + 8) = *(float *)(moby_data + 8) - *(float *)(moby_data + 0x38);
    transform_vector_by_basis(collision_probe_position, moby_data + 0x20, moby + 0xC0);
    fast_vec_add(collision_probe_position, collision_probe_position, moby + 0x10);
    if (func_L00_001F10E0_s(collision_probe_position, collision_radius, 0, moby)) {
        float speed, approach;
        collision_normal = coll_hit_normal.f;
        normalize_vector_xyz(collision_normal, collision_normal, 1.0f);
        speed = fast_vec_length(moby_data);
        approach = fast_vec_dot(moby_data, collision_normal);
        if (approach < 0.0f) {
            reflect_vector(moby_data, moby_data, collision_normal);
            normalize_vector_xyz(aD0, collision_normal, approach * 0.5f);
            fast_vec_add(moby_data, moby_data, aD0);
            normalize_vector_xyz(moby_data, moby_data, speed);
            fast_vec_cross(aE0, collision_normal, moby_data);
            normalize_vector_xyz(aE0, aE0, 1.0f);
            build_quaternion_from_axis_angle(aE0, aE0, fast_vec_length(rotation_step_euler));
            quaternion_to_matrix(aE0, bounce_rotation_matrix);
            matrix_to_euler(bounce_rotation_matrix, rotation_step_euler);
        }
    }
}
