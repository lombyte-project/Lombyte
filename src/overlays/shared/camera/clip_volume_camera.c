/* Camera type 1, the clip volume camera: active while the hero is inside its clip volume, plus the helpers that place it from its level record and turn it towards the hero. */
#include "rnc/hero/hero.h"
#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/overlay/shared/camera/init_camera_system.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"

typedef struct {
    char  pad0[0x40];
    int   x40;
    char  pad1[8];
    float x4C;
} CamSub;

typedef struct {
    char   pad0[4];
    float  f4;
    float  f8;
    int    fC;
    CamSub sub;
    char   pad1[0x20];
    int    x80;
} CamInfo;

extern int   D_L03_0015EF50;
extern void  RaiseKernelTrap();
extern float transform_basis_reference_vector[] __asm__("D_L03_00166F30");
extern void  rotate_vector_by_scaled_angle_between_vectors(
     float, float *, float *, float *, int) __asm__("FUN_L03_0024e6f8");

/* Ported from rac1-decomp (src/overlays/shared/vendor_00292AC0.c: func_L03_002E99F0), where it is exact; names translated to the US level program. */

void initialize_transform_state_from_level_table(char *transform_state) __asm__("FUN_L03_002e8628");

void initialize_transform_state_from_level_table(char *transform_state) {
    float    entry_euler_angles[4] __attribute__((aligned(16)));
    float    initial_rotation_matrix[16] __attribute__((aligned(16)));
    CamSub  *camera_subrecord = &(*(CamInfo **)(transform_state + 0x70))->sub;
    CamInfo *camera_info;
    char    *ent;
    int      level_table_entry_index;
    char    *entry_subrecord;
    camera_subrecord->x4C = 1.5f;
    camera_subrecord->x40 = 0;
    (*(CamInfo **)(transform_state + 0x70))->x80 = 0;
    camera_info = *(CamInfo **)(transform_state + 0x70);
    camera_info->f4 = 0.02f;
    camera_info->f8 = 0.1f;
    camera_info->fC = 0;
    level_table_entry_index = *(short *)(transform_state + 0x84);
    ent = (char *)D_L03_0015EF50 + (level_table_entry_index << 5);
    entry_subrecord = *(char **)(ent + 0x1C);
    if (level_table_entry_index < 0) {
        RaiseKernelTrap();
    } else {
        camera_subrecord->x4C = *(float *)(entry_subrecord + 0x18);
    }
    qcopy(transform_state + 0x30, ent);
    qcopy(entry_euler_angles, ent + 0x10);
    euler_rotation_matrix(initial_rotation_matrix, entry_euler_angles);
    qcopy(transform_state, initial_rotation_matrix);
    qcopy(transform_state + 0x10, initial_rotation_matrix + 4);
    qcopy(transform_state + 0x20, initial_rotation_matrix + 8);
    qcopy(transform_state + 0x40, transform_state);
    *(short *)(transform_state + 0x7E) = 0;
}

/* smooths a direction and rebuilds its orthogonal matrix axes */
/* Ported from rac1-decomp (src/overlays/shared/vendor_00292AC0.c: func_L03_002E9AF8), where it is exact; names translated to the US level program. */

void smooth_transform_orientation_toward_hero(char *transform_state) __asm__("FUN_L03_002e8730");

void smooth_transform_orientation_toward_hero(char *transform_state) {
    float  position[4] __attribute__((aligned(16)));
    float  direction[4] __attribute__((aligned(16)));
    char  *orientation_spring_state = *(char **)(transform_state + 0x70);
    float *height = (float *)(orientation_spring_state + 0x10);
    char  *saved_direction;
    float  angle, rotation_scale;
    qcopy(position, hero.motion.pos.f);
    position[2] += height[19];
    saved_direction = transform_state + 0x40;
    fast_vec_sub(direction, position, transform_state + 0x30);
    fast_vec_normalize(direction, direction, 1.0f);
    angle = fast_arc_sin(fast_vec_dot(direction, transform_state));
    rotation_scale = rope_spring_turn(orientation_spring_state,
                                      0.0f,
                                      1.57079637f - angle,
                                      *(float *)(orientation_spring_state + 4),
                                      *(float *)(orientation_spring_state + 8),
                                      *(float *)(orientation_spring_state + 12));
    rotate_vector_by_scaled_angle_between_vectors(
        rotation_scale, (float *)transform_state, (float *)saved_direction, direction, 1);
    fast_vec_normalize(saved_direction, transform_state, 1.0f);
    qcopy(transform_state, saved_direction);
    fast_vec_cross(transform_state + 0x10, transform_state, transform_basis_reference_vector);
    fast_vec_normalize(transform_state + 0x10, transform_state + 0x10, 1.0f);
    fast_vec_cross(transform_state + 0x20, transform_state + 0x10, transform_state);
}

int evaluate_clip_volume_candidate(char *candidate_object,
                                   char *comparison_object) __asm__("FUN_L03_002e8870");

int evaluate_clip_volume_candidate(char *candidate_object, char *comparison_object) {
    char *candidate_level_table_entry =
        (char *)(D_L03_0015EF50 + (*(short *)(candidate_object + 0x84) << 5));
    char *sub = *(char **)(candidate_level_table_entry + 0x1C);
    char *global_state_base;
    int   five;
    if (*(int *)(candidate_object + 0x74) != 4)
        return 0;
    if (comparison_object != 0 && *(short *)(comparison_object + 0x7E) == 0 &&
        !(*(unsigned char *)(candidate_object + 0x7C) >
          *(unsigned char *)(comparison_object + 0x7C)))
        return 0;
    if (is_point_inside_clip_volume(D_0013E533 + 0xE9D, *(int *)(sub + 0xC)) != 0) {
        if (is_point_inside_clip_volume(D_L03_00166EC0, *(int *)(sub + 0xC)) != 0)
            return 1;
        global_state_base = D_L03_00166EC0 - 0x140;
        five = *(int *)(global_state_base + 0x180);
        *(unsigned char *)(global_state_base + 0x273) = 0;
        *(short *)(five + 0x7E) = 2;
        *(float *)(global_state_base + 0x294) = 0.018f;
        *(float *)(global_state_base + 0x288) = 0.018f;
        return 1;
    }
    return 0;
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_00292AC0.c: func_L03_002E9D48), where it is exact; names translated to the US level program. */

void set_transform_state_three_when_outside_clip_volume(char *moby) __asm__("FUN_L03_002e8980");

void set_transform_state_three_when_outside_clip_volume(char *moby) {
    char *level_table_entry = (char *)(D_L03_0015EF50 + (*(short *)(moby + 0x84) << 5));
    char *sub = *(char **)(level_table_entry + 0x1C);
    if (*(int *)(moby + 0x74) == 4) {
        if (is_point_inside_clip_volume(D_0013E533 + 0xE9D, *(int *)(sub + 0xC)) == 0) {
            *(short *)(moby + 0x7E) = 3;
        }
    }
}
