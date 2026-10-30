/* Level 08 moby classes: bolt_thief, shrub. */
#include "types.h"
#include "rnc/game/bolt_sources.h"
#include "rnc/mobys/moby_class_id.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/math/vector.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/mobys/moby.h"
#include "sda.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/options.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/interpolation.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/hero/hero_check_damage.h"
#include "rnc/overlay/shared/math/interpolation/compute_directed_angle_delta.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/math/rotations/clip_negative_segment_endpoint_xyz.h"
#include "rnc/overlay/shared/math/vectors/compute_transformed_delta_component.h"
#include "rnc/overlay/shared/math/vectors/random_vector.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/math/rotations/matrix_to_euler.h"
#include "rnc/overlay/shared/camera/follow_camera_update.h"
#include "rnc/overlay/shared/vendor/set_camera_rotation.h"
#include "rnc/overlay/shared/math/vectors/reflect_vector.h"
#include "rnc/overlay/shared/rendering/update_entity.h"
#include "rnc/overlay/l08/hero/hero_set_state.h"
#include "rnc/overlay/shared/math/vectors/fast_subtract_rotations_cf.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"

/* finds the first of eight linked entries whose direction lines up with the moby */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002E0258.c: func_L08_002E15A0), where it is exact; names translated to the US level program. */

extern char D_L08_001675C0[];

int find_batalia_cannon_aim_target(struct Moby *moby) __asm__("FUN_L08_002e0228");

int find_batalia_cannon_aim_target(struct Moby *moby) {
    float facing_direction[4];
    float target_direction[4];
    int  *linked_targets = (int *)((char *)moby->pvars + 0x60);
    int   i;
    for (i = 0; i < 8; i++) {
        if (linked_targets[i] != 0) {
            float direction_dot;
            build_spherical_offset(facing_direction, 1.0f, moby->rot.z, -moby->rot.y);
            fast_vec_sub(target_direction, (char *)linked_targets[i] + 0x10, D_L08_001675C0);
            fast_vec_normalize(target_direction, target_direction, 1.0f);
            direction_dot = fast_vec_dot(target_direction, facing_direction);
            if (fast_cos(0.06981317f) < direction_dot)
                return linked_targets[i];
        }
    }
    return 0;
}

/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002E0258.c: func_L08_002E2A10), where it is exact; names translated to the US level program. */

/* Sets the moby's byte 0xBC and calls hero_set_state(0x32, 1). */
void request_batalia_cannon_mount(struct Moby *command_target_moby) __asm__("FUN_L08_002e1698");

void request_batalia_cannon_mount(struct Moby *command_target_moby) {
    command_target_moby->command = 1;
    hero_set_state(0x32, 1);
}

#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002E0258.c: func_L08_002E3010), where it is exact; names translated to the US level program. */

extern char D_0013E533[];

void *spawn_joint_aligned_moby_part(struct Moby *parent,
                                    int          oClass,
                                    int          joint,
                                    int          pjoint) __asm__("FUN_L08_002e1c98");

void *spawn_joint_aligned_moby_part(struct Moby *parent, int oClass, int joint, int pjoint) {
    unsigned char *child_moby = create_moby(oClass);
    float          child_joint_position[4], parent_joint_position[4];
    if (child_moby != 0) {
        child_moby[0x30] = 0xFF;
        MOBY(child_moby)->draw_dist = 0xFF;
        child_moby[0x31] = 1;
        MOBY(child_moby)->flags |= 0x4000;
        MOBY(child_moby)->spawn_frame = *(long *)(*(char **)(D_0013E533 + 0x2E9D) + 0x38);
        qcopy(child_moby + 0x40, &parent->rot);
        moby_point_position(child_moby, joint, child_joint_position);
        moby_point_position(parent, pjoint, parent_joint_position);
        fast_vec_sub(child_moby + 0x10, parent_joint_position, child_joint_position);
        update_entity(child_moby);
    }
    return child_moby;
}
/* builds a basis from a direction vector and writes the orientation into the moby */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002E0258.c: func_L08_002E6130), where it is exact; names translated to the US level program. */

void set_batalia_bolt_pickup_surface_rotation(struct Moby *moby,
                                              float *direction_vector) __asm__("FUN_L08_002e4db8");

void set_batalia_bolt_pickup_surface_rotation(struct Moby *moby, float *direction_vector) {
    float direction_basis_matrix[16];
    float reconstructed_rotation_matrix[16];
    float first_basis_vector[4];
    float second_basis_vector[4];
    char *pvars_cursor = (char *)moby->pvars;
    first_basis_vector[1] = direction_vector[2];
    first_basis_vector[2] = direction_vector[0];
    first_basis_vector[0] = -direction_vector[1];
    fast_vec_cross(first_basis_vector, direction_vector, first_basis_vector);
    fast_vec_cross(second_basis_vector, first_basis_vector, direction_vector);
    qcopy(direction_basis_matrix, first_basis_vector);
    qcopy(direction_basis_matrix + 4, second_basis_vector);
    qcopy(direction_basis_matrix + 8, direction_vector);
    clear_u64_value(direction_basis_matrix + 12);
    pvars_cursor += 0x10;
    direction_basis_matrix[15] = 1.0f;
    matrix_to_euler(direction_basis_matrix, pvars_cursor);
    euler_rotation_matrix(reconstructed_rotation_matrix, (float *)pvars_cursor);
    reconstructed_rotation_matrix[15] = 42.0f;
}
/* spawns the child mobys this object owns and fills its part table */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002E0258.c: func_L08_002E9B60), where it is exact; names translated to the US level program. */

extern int FUN_L08_002e1c98_c() __asm__("FUN_L08_002e1c98");

void spawn_shrub_parts(char *moby) __asm__("FUN_L08_002e8788");

void spawn_shrub_parts(char *moby) {
    char        *data = ((char *)MOBY(moby)->pvars);
    char        *anchor_joint_cursor = data + 0xBC;
    int          i = 0;
    struct Moby *child_moby;
    char        *muzzle_anchor_moby;
    char        *muzzle_part_moby;
    *(char **)(data + 0x60) = moby;
    *(int *)(data + 0x64) = 0;
    *(int *)(data + 0x68) = 0;
    *(int *)(data + 0x6C) = 0;
    *(char **)(data + 0x90) = FUN_L08_002e1c98_c(moby, 0x1C0, 1, 0);
    *(char **)(data + 0x94) = moby;
    *(int *)(data + 0x98) = 1;
    *(int *)(data + 0x9C) = 0;
    child_moby = FUN_L08_002e1c98_c(moby, 0x1BD, 0, 2);
    *(char **)(data + 0x70) = child_moby;
    *(int *)(data + 0x7C) = 2;
    *(char **)(data + 0x74) = moby;
    *(int *)(data + 0x78) = 0;
    child_moby->scale = *(float *)(((char *)child_moby->pclass) + 0x24) * 0.75f;
    muzzle_anchor_moby = FUN_L08_002e1c98_c(moby, 0x1BA, 1, 1);
    *(char **)(data + 0x80) = muzzle_anchor_moby;
    *(char **)(data + 0x84) = moby;
    *(int *)(data + 0x88) = 1;
    *(int *)(data + 0x8C) = 1;
    muzzle_part_moby = FUN_L08_002e1c98_c(muzzle_anchor_moby, 0x1B9, 0, 0);
    *(char **)(data + 0xA0) = muzzle_part_moby;
    *(char **)(data + 0xA4) = *(char **)(data + 0x80);
    *(int *)(data + 0xA8) = 0;
    *(int *)(data + 0xAC) = 0;
    do {
        int j = i + 2;
        i++;
        *(char **)(anchor_joint_cursor - 0xC) =
            FUN_L08_002e1c98_c(*(char **)(data + 0x90), 0x1BB, 0, j);
        *(char **)(anchor_joint_cursor - 8) = *(char **)(data + 0x90);
        *(int *)(anchor_joint_cursor - 4) = 0;
        *(int *)anchor_joint_cursor = j;
        anchor_joint_cursor += 0x10;
    } while (i < 8);
}
/* Re-aims the twelve child mobys of a parent, adjusts the parent's spin, then runs the children's follow-up. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002E0258.c: func_L08_002E9F78), where it is exact; names translated to the US level program. */

extern float D_0015ED6C;

#include "rnc/overlay/l08/mobys/bolt_thief_globals.h"
#include "rnc/overlay/level_path.h"

extern float shrub_part_spin_speed_degrees_gp __asm__("D_L08_00161D28") __attribute__((sda));
void         update_shrub_parts(struct Moby *parent_moby) __asm__("FUN_L08_002e8ba0");

void update_shrub_parts(struct Moby *parent_moby) {
    char *data = (char *)parent_moby->pvars;
    float part_joint_position[4], anchor_joint_position[4], joint_alignment_offset[4];
    char *base = data + 0x60;
    int   i;
    for (i = 0; i < 12; i++) {
        int  *anchor_joint_cursor = (int *)(base + 0x1C + i * 16);
        char *m = (char *)anchor_joint_cursor[-3];
        if (m != 0) {
            *(float *)(m + 0x48) = parent_moby->rot.z;
            moby_point_position(m, anchor_joint_cursor[-1], part_joint_position);
            moby_point_position(
                (void *)anchor_joint_cursor[-2], anchor_joint_cursor[0], anchor_joint_position);
            fast_vec_sub(joint_alignment_offset, anchor_joint_position, part_joint_position);
            fast_vec_add(m + 0x10, m + 0x10, joint_alignment_offset);
        }
    }
    if (*(int *)(data + 0x90) != 0) {
        char *spinning_part_moby = *(char **)(data + 0x90);
        *(float *)(*(char **)(data + 0x90) + 0x40) =
            fast_add_rotations(MOBY(spinning_part_moby)->rot.x,
                               shrub_part_spin_speed_degrees_gp * DEG_TO_RAD * D_0015ED6C);
    }
    for (i = 0; i < 12; i++) {
        char *m = *(char **)(base + 0x10 + i * 16);
        if (m != 0)
            update_entity(m);
    }
}
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002E0258.c: func_L08_002EADF0), where it is exact; names translated to the US level program. */

extern float commando_part_spin_speed_degrees_gp __asm__("D_L08_00161D38") __attribute__((sda));
void         update_commando_parts(struct Moby *parent_moby) __asm__("FUN_L08_002e9a18");

void update_commando_parts(struct Moby *parent_moby) {
    char *data = (char *)parent_moby->pvars;
    float part_joint_position[4];
    float anchor_joint_position[4];
    float joint_alignment_offset[4];
    int   i;
    char *part_entry_cursor;
    char *parts = data + 0x60;
    for (i = 0; i < 16; i++) {
        char *anchor_joint_cursor = parts + 0x1C + i * 16;
        char *part = *(char **)(anchor_joint_cursor - 0xC);
        if (part != 0) {
            *(float *)(part + 0x48) = parent_moby->rot.z;
            moby_point_position(part, *(int *)(anchor_joint_cursor - 4), part_joint_position);
            moby_point_position(*(void **)(anchor_joint_cursor - 8),
                                *(int *)anchor_joint_cursor,
                                anchor_joint_position);
            fast_vec_sub(joint_alignment_offset, anchor_joint_position, part_joint_position);
            fast_vec_add(part + 0x10, part + 0x10, joint_alignment_offset);
        }
    }
    if (*(char **)(data + 0x90) != 0) {
        *(float *)(*(char **)(data + 0x90) + 0x40) =
            fast_add_rotations(*(float *)(*(char **)(data + 0x90) + 0x40),
                               commando_part_spin_speed_degrees_gp * DEG_TO_RAD * D_0015ED6C);
    }
    part_entry_cursor = parts + 0x10;
    for (i = 0; i < 16; i++) {
        char *updated_part = *(char **)part_entry_cursor;
        if (updated_part != 0) {
            update_entity(updated_part);
            updated_part = *(char **)part_entry_cursor;
            if (*(unsigned short *)(updated_part + 0x34) & 0x8000) {
                fast_vec_scale(updated_part + 0xD0, updated_part + 0xD0, -1.0f);
            }
        }
        part_entry_cursor += 16;
    }
}

#include "rnc/hero/hero.h"

/* Draws the Batalia cannon's HUD: a four-part frame, markers for the eight linked and four indexed targets
   (relative to the cannon's heading), the inner frame, a 32-segment health dial and two rotating needles. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002E0258.c: func_L08_002E0258), where it is exact; names translated to the US level program. */

typedef struct Moby Moby;

typedef struct RData51 RData51;

struct RData51 {
    char  pad0[0x58];
    int   health, pad5C;
    Moby *linked[8];
    int   indices[4];
    char  pad90[0x10];
    int   timer;
};

extern char *D_L08_0015FFD8_lui __asm__("D_L08_0015FFD8") NOT_SDA;
extern int   D_0015EDB4_lui __asm__("D_0015EDB4") NOT_SDA;         /* no foreign declaration */
extern int   D_L08_0015F5CC_lui __asm__("D_L08_0015F5CC") NOT_SDA; /* no foreign declaration */
extern f32   D_L08_00161AC0_gp __asm__("D_L08_00161AC0")
    __attribute__((sda)); /* no foreign declaration */
extern int D_L08_00161AC8_gp __asm__("D_L08_00161AC8")
    __attribute__((sda)); /* no foreign declaration */
extern int D_L08_00161AD4_gp __asm__("D_L08_00161AD4")
    __attribute__((sda)); /* no foreign declaration */
extern int D_L08_00161AD8_gp __asm__("D_L08_00161AD8")
    __attribute__((sda)); /* no foreign declaration */
extern int D_L08_00161ADC_gp __asm__("D_L08_00161ADC")
    __attribute__((sda)); /* no foreign declaration */
extern f32 D_L08_00161B60_gp __asm__("D_L08_00161B60")
    __attribute__((sda)); /* no foreign declaration */
extern f32 D_L08_00161B64_gp __asm__("D_L08_00161B64")
    __attribute__((sda)); /* no foreign declaration */
extern f32 D_L08_00161B68_gp __asm__("D_L08_00161B68")
    __attribute__((sda)); /* no foreign declaration */
extern f32 D_L08_00161B6C_gp __asm__("D_L08_00161B6C")
    __attribute__((sda)); /* no foreign declaration */
extern f32 D_L08_00161B70_gp __asm__("D_L08_00161B70")
    __attribute__((sda)); /* no foreign declaration */
extern f32 D_L08_00161B74_gp __asm__("D_L08_00161B74")
    __attribute__((sda)); /* no foreign declaration */
extern f32 D_L08_00161B78_gp __asm__("D_L08_00161B78")
    __attribute__((sda)); /* no foreign declaration */
extern f32 D_L08_00161B7C_gp __asm__("D_L08_00161B7C")
    __attribute__((sda)); /* no foreign declaration */
extern f32 D_L08_00161B80_gp __asm__("D_L08_00161B80")
    __attribute__((sda)); /* no foreign declaration */
extern f32 D_L08_00161B84_gp __asm__("D_L08_00161B84")
    __attribute__((sda)); /* no foreign declaration */
extern f32 D_L08_00161B88_gp __asm__("D_L08_00161B88")
    __attribute__((sda)); /* no foreign declaration */
extern f32 D_L08_00161B8C_gp __asm__("D_L08_00161B8C")
    __attribute__((sda)); /* no foreign declaration */
extern f32 D_L08_00161B90_gp __asm__("D_L08_00161B90")
    __attribute__((sda)); /* no foreign declaration */
extern f32 D_L08_00161B94_gp __asm__("D_L08_00161B94")
    __attribute__((sda)); /* no foreign declaration */
extern f32 D_L08_00161B98_gp __asm__("D_L08_00161B98")
    __attribute__((sda)); /* no foreign declaration */
extern f32 D_L08_00161B9C_gp __asm__("D_L08_00161B9C")
    __attribute__((sda)); /* no foreign declaration */
extern int D_L08_00161BA0_gp __asm__("D_L08_00161BA0")
    __attribute__((sda)); /* no foreign declaration */
extern int D_L08_00161BA4_gp __asm__("D_L08_00161BA4")
    __attribute__((sda)); /* no foreign declaration */
extern int D_L08_00161BA8_gp __asm__("D_L08_00161BA8")
    __attribute__((sda)); /* no foreign declaration */
extern int D_L08_00161BAC_gp __asm__("D_L08_00161BAC")
    __attribute__((sda)); /* no foreign declaration */
extern int D_L08_00161BB0_gp __asm__("D_L08_00161BB0")
    __attribute__((sda)); /* no foreign declaration */
extern int D_L08_00161BB4_gp __asm__("D_L08_00161BB4")
    __attribute__((sda)); /* no foreign declaration */
extern int D_L08_00161BB8_gp __asm__("D_L08_00161BB8")
    __attribute__((sda)); /* no foreign declaration */
extern int D_L08_00161BBC_gp __asm__("D_L08_00161BBC")
    __attribute__((sda)); /* no foreign declaration */
extern int D_L08_00161BC0_gp __asm__("D_L08_00161BC0")
    __attribute__((sda)); /* no foreign declaration */
extern int D_L08_00161BC4_gp __asm__("D_L08_00161BC4")
    __attribute__((sda)); /* no foreign declaration */
extern int D_L08_00161BC8_gp __asm__("D_L08_00161BC8")
    __attribute__((sda)); /* no foreign declaration */
extern int D_L08_00161BCC_gp __asm__("D_L08_00161BCC")
    __attribute__((sda)); /* no foreign declaration */
extern int D_L08_00161BD0_gp __asm__("D_L08_00161BD0")
    __attribute__((sda)); /* no foreign declaration */
extern int D_L08_00161BD4_gp __asm__("D_L08_00161BD4")
    __attribute__((sda)); /* no foreign declaration */
extern int D_L08_00161BD8_gp __asm__("D_L08_00161BD8")
    __attribute__((sda)); /* no foreign declaration */
extern int D_L08_00161BDC_gp __asm__("D_L08_00161BDC")
    __attribute__((sda)); /* no foreign declaration */
extern int D_L08_00161BE0_gp __asm__("D_L08_00161BE0")
    __attribute__((sda)); /* no foreign declaration */
extern int D_L08_00161BE4_gp __asm__("D_L08_00161BE4")
    __attribute__((sda)); /* no foreign declaration */
extern int D_L08_00161BE8_gp __asm__("D_L08_00161BE8")
    __attribute__((sda)); /* no foreign declaration */
extern int D_L08_00161BEC_gp __asm__("D_L08_00161BEC")
    __attribute__((sda)); /* no foreign declaration */
extern int   fast_tween_color() __asm__("FUN_001fa6e0");
extern s64   get_effect_texture(s32) __asm__("func_001F44B8");
extern void  FUN_L02_0020bc88(void *, void *, void *, s32, s32);
extern void  vu1_add_g_sregister(s32, u64) __asm__("func_00233980");
extern void  FUN_L02_0020bc88_c(void *, void *, void *, long, int) __asm__("FUN_L02_0020bc88");
extern int   func_001FA8A8_2E0258(int, int, float) __asm__("FUN_001fa6e0");
extern char *D_L08_0015FFD8;
extern int   D_L08_0015F5CC __asm__("D_L08_0015F5CC");
extern unsigned char draw_map51[4] __asm__("D_0015EDB4");
extern float         D_L08_00161AC0 __asm__("D_L08_00161AC0");
extern float         D_L08_00161B60 __asm__("D_L08_00161B60");
extern float         D_L08_00161B64 __asm__("D_L08_00161B64");
extern float         D_L08_00161B68 __asm__("D_L08_00161B68");
extern float         D_L08_00161B6C __asm__("D_L08_00161B6C");
extern float         D_L08_00161B70 __asm__("D_L08_00161B70");
extern float         D_L08_00161B74 __asm__("D_L08_00161B74");
extern float         D_L08_00161B78 __asm__("D_L08_00161B78");
extern float         D_L08_00161B7C __asm__("D_L08_00161B7C");
extern float         D_L08_00161B80 __asm__("D_L08_00161B80");
extern float         D_L08_00161B84 __asm__("D_L08_00161B84");
extern float         D_L08_00161B88 __asm__("D_L08_00161B88");
extern float         D_L08_00161B8C __asm__("D_L08_00161B8C");
extern float         D_L08_00161B90 __asm__("D_L08_00161B90");
extern float         D_L08_00161B94 __asm__("D_L08_00161B94");
extern float         D_L08_00161B98 __asm__("D_L08_00161B98");
extern float         D_L08_00161B9C __asm__("D_L08_00161B9C");
extern int           D_L08_00161AC8 __asm__("D_L08_00161AC8");
extern int           D_L08_00161AD4 __asm__("D_L08_00161AD4");
extern int           D_L08_00161AD8 __asm__("D_L08_00161AD8");
extern int           D_L08_00161ADC __asm__("D_L08_00161ADC");
extern int           D_L08_00161BA0 __asm__("D_L08_00161BA0");
extern int           D_L08_00161BA4 __asm__("D_L08_00161BA4");
extern int           D_L08_00161BA8 __asm__("D_L08_00161BA8");
extern int           D_L08_00161BAC __asm__("D_L08_00161BAC");
extern int           D_L08_00161BB0 __asm__("D_L08_00161BB0");
extern int           D_L08_00161BB4 __asm__("D_L08_00161BB4");
extern int           D_L08_00161BB8 __asm__("D_L08_00161BB8");
extern int           D_L08_00161BBC __asm__("D_L08_00161BBC");
extern int           D_L08_00161BC0 __asm__("D_L08_00161BC0");
extern int           D_L08_00161BC4 __asm__("D_L08_00161BC4");
extern int           D_L08_00161BC8 __asm__("D_L08_00161BC8");
extern int           D_L08_00161BCC __asm__("D_L08_00161BCC");
extern int           D_L08_00161BD0 __asm__("D_L08_00161BD0");
extern int           D_L08_00161BD4 __asm__("D_L08_00161BD4");
extern int           D_L08_00161BD8 __asm__("D_L08_00161BD8");
extern int           D_L08_00161BDC __asm__("D_L08_00161BDC");
extern int           D_L08_00161BE0 __asm__("D_L08_00161BE0");
extern int           D_L08_00161BE4 __asm__("D_L08_00161BE4");
extern int           D_L08_00161BE8 __asm__("D_L08_00161BE8");
extern int           D_L08_00161BEC __asm__("D_L08_00161BEC");

void draw_batalia_cannon_hud(Moby *moby) __asm__("FUN_L08_002deee0");

void draw_batalia_cannon_hud(Moby *moby) {
    long     xy[4];
    int      color_words[4], uv_words[4];
    float    relative[4];
    RData51 *data = (*(RData51 **)&moby->pvars);
    long     texture;
    int      segments;
    int      i, j, k, indexed_target_slot, sign, color;
    int     *indices;
    float    sign_x, sign_y, angle, radius, fraction, next_angle;
    int      outer_x, outer_y, inner_x, inner_y;
    int      color_a, color_b;
    long     base;
    vu1_add_g_sregister(0x42, 0x8000000044L);
    texture = get_effect_texture(D_L08_00161BA0 + 0x28);
    uv_words[0] = D_L08_00161BB0;
    uv_words[1] = D_L08_00161BB4;
    uv_words[2] = D_L08_00161BB8;
    uv_words[3] = D_L08_00161BBC;
    color_words[0] = color_words[1] = color_words[2] = color_words[3] = D_L08_00161AC8;
    for (i = 0; i < 4; i++) {
        sign_x = (i & 1) ? -1.0f : 1.0f;
        sign_y = (i >= 2) ? -1.0f : 1.0f;
        xy[3] = 0xFFFFF000000000L;
        xy[2] = 0xFFFFF000000000L;
        xy[1] = 0xFFFFF000000000L;
        xy[0] = 0xFFFFF000000000L;
        xy[0] += (unsigned int)((truncate_float_to_s32((D_L08_00161B84 + D_L08_00161B8C * sign_y) *
                                                       16.0f)
                                 << 16) -
                                (int)0x80000000);
        xy[0] += (truncate_float_to_s32(D_L08_00161B80 + D_L08_00161B88 * sign_x) << 4) + 0x8000;
        xy[1] +=
            (unsigned int)((truncate_float_to_s32(D_L08_00161B84 * 16.0f) << 16) - (int)0x80000000);
        xy[1] += (truncate_float_to_s32(D_L08_00161B80 + D_L08_00161B88 * sign_x) << 4) + 0x8000;
        xy[2] += (unsigned int)((truncate_float_to_s32((D_L08_00161B84 + D_L08_00161B8C * sign_y) *
                                                       16.0f)
                                 << 16) -
                                (int)0x80000000);
        xy[2] += (truncate_float_to_s32(D_L08_00161B80) << 4) + 0x8000;
        xy[3] +=
            (unsigned int)((truncate_float_to_s32(D_L08_00161B84 * 16.0f) << 16) - (int)0x80000000);
        xy[3] += (truncate_float_to_s32(D_L08_00161B80) << 4) + 0x8000;
        FUN_L02_0020bc88_c(xy, uv_words, color_words, texture, 1);
    }
    texture = get_effect_texture(D_L08_00161BA4);
    uv_words[0] = D_L08_00161BC0;
    uv_words[1] = D_L08_00161BC4;
    uv_words[2] = D_L08_00161BC8;
    uv_words[3] = D_L08_00161BCC;
    for (k = 0; k < 8; k++) {
        float angle, radius;
        if (data->linked[k]) {
            if ((signed char)data->linked[k]->state >= 0) {
                if (data->linked[k]->state == 2) {
                    if (D_L08_0015F5CC & 0x10) {
                        color_words[0] = color_words[1] = color_words[2] = color_words[3] =
                            D_L08_00161AD4;
                    } else {
                        color_words[0] = color_words[1] = color_words[2] = color_words[3] =
                            0x60000000;
                    }
                } else {
                    color_words[0] = color_words[1] = color_words[2] = color_words[3] =
                        D_L08_00161ADC;
                }
                angle = fast_subtract_rotations(
                    fast_arc_tan((*(float(*)[4]) & data->linked[k]->pos)[0] -
                                     (*(float(*)[4]) & moby->pos)[0],
                                 (*(float(*)[4]) & data->linked[k]->pos)[1] -
                                     (*(float(*)[4]) & moby->pos)[1]),
                    (*(float(*)[4]) & moby->rot)[2]);
                radius = fast_vec_dist((*(float(*)[4]) & data->linked[k]->pos),
                                       (*(float(*)[4]) & moby->pos)) *
                         D_L08_00161AC0;
                sign = draw_map51[0] ? -1 : 1;
                relative[0] = (float)-sign * fast_sin(angle) * radius;
                relative[1] = -fast_cos(angle) * radius;
                xy[3] = 0xFFFFF000000000L;
                xy[2] = 0xFFFFF000000000L;
                xy[1] = 0xFFFFF000000000L;
                xy[0] = 0xFFFFF000000000L;
                xy[0] +=
                    (unsigned int)((truncate_float_to_s32(
                                        (D_L08_00161B84 + D_L08_00161B94) * 16.0f + relative[1])
                                    << 16) -
                                   (int)0x80000000);
                xy[0] +=
                    truncate_float_to_s32((D_L08_00161B80 + D_L08_00161B90) * 16.0f + relative[0]) +
                    0x8000;
                xy[1] +=
                    (unsigned int)((truncate_float_to_s32(
                                        (D_L08_00161B84 - D_L08_00161B94) * 16.0f + relative[1])
                                    << 16) -
                                   (int)0x80000000);
                xy[1] +=
                    truncate_float_to_s32((D_L08_00161B80 + D_L08_00161B90) * 16.0f + relative[0]) +
                    0x8000;
                xy[2] +=
                    (unsigned int)((truncate_float_to_s32(
                                        (D_L08_00161B84 + D_L08_00161B94) * 16.0f + relative[1])
                                    << 16) -
                                   (int)0x80000000);
                xy[2] +=
                    truncate_float_to_s32((D_L08_00161B80 - D_L08_00161B90) * 16.0f + relative[0]) +
                    0x8000;
                xy[3] +=
                    (unsigned int)((truncate_float_to_s32(
                                        (D_L08_00161B84 - D_L08_00161B94) * 16.0f + relative[1])
                                    << 16) -
                                   (int)0x80000000);
                xy[3] +=
                    truncate_float_to_s32((D_L08_00161B80 - D_L08_00161B90) * 16.0f + relative[0]) +
                    0x8000;
                FUN_L02_0020bc88_c(xy, uv_words, color_words, texture, 1);
            } else
                data->linked[k] = 0;
        }
    }
    indices = data->indices;
    for (indexed_target_slot = 0; indexed_target_slot < 4; indexed_target_slot++) {
        float angle, radius;
        if (((Moby *)D_L08_0015FFD8)[*indices].state < 0x7F) {
            color_words[0] = color_words[1] = color_words[2] = color_words[3] = D_L08_00161AD8;
            angle = fast_subtract_rotations(
                fast_arc_tan((*(float(*)[4]) & ((Moby *)D_L08_0015FFD8)[*indices].pos)[0] -
                                 (*(float(*)[4]) & moby->pos)[0],
                             (*(float(*)[4]) & ((Moby *)D_L08_0015FFD8)[*indices].pos)[1] -
                                 (*(float(*)[4]) & moby->pos)[1]),
                (*(float(*)[4]) & moby->rot)[2]);
            radius = fast_vec_dist((*(float(*)[4]) & ((Moby *)D_L08_0015FFD8)[*indices].pos),
                                   (*(float(*)[4]) & moby->pos)) *
                     D_L08_00161AC0;
            relative[0] = -fast_sin(angle) * radius;
            relative[1] = -fast_cos(angle) * radius;
            xy[3] = 0xFFFFF000000000L;
            xy[2] = 0xFFFFF000000000L;
            xy[1] = 0xFFFFF000000000L;
            xy[0] = 0xFFFFF000000000L;
            xy[0] += (unsigned int)((truncate_float_to_s32(
                                         (D_L08_00161B84 + D_L08_00161B9C) * 16.0f + relative[1])
                                     << 16) -
                                    (int)0x80000000);
            xy[0] +=
                truncate_float_to_s32((D_L08_00161B80 + D_L08_00161B98) * 16.0f + relative[0]) +
                0x8000;
            xy[1] += (unsigned int)((truncate_float_to_s32(
                                         (D_L08_00161B84 - D_L08_00161B9C) * 16.0f + relative[1])
                                     << 16) -
                                    (int)0x80000000);
            xy[1] +=
                truncate_float_to_s32((D_L08_00161B80 + D_L08_00161B98) * 16.0f + relative[0]) +
                0x8000;
            xy[2] += (unsigned int)((truncate_float_to_s32(
                                         (D_L08_00161B84 + D_L08_00161B9C) * 16.0f + relative[1])
                                     << 16) -
                                    (int)0x80000000);
            xy[2] +=
                truncate_float_to_s32((D_L08_00161B80 - D_L08_00161B98) * 16.0f + relative[0]) +
                0x8000;
            xy[3] += (unsigned int)((truncate_float_to_s32(
                                         (D_L08_00161B84 - D_L08_00161B9C) * 16.0f + relative[1])
                                     << 16) -
                                    (int)0x80000000);
            xy[3] +=
                truncate_float_to_s32((D_L08_00161B80 - D_L08_00161B98) * 16.0f + relative[0]) +
                0x8000;
            FUN_L02_0020bc88_c(xy, uv_words, color_words, texture, 1);
        }
        indices++;
    }
    uv_words[0] = D_L08_00161BB0;
    uv_words[1] = D_L08_00161BB4;
    uv_words[2] = D_L08_00161BB8;
    uv_words[3] = D_L08_00161BBC;
    color_words[0] = color_words[1] = color_words[2] = color_words[3] = D_L08_00161AC8;
    texture = get_effect_texture(D_L08_00161BA0 + 0x28);
    for (i = 0; i < 4; i++) {
        sign_x = (i & 1) ? -1.0f : 1.0f;
        sign_y = (i >= 2) ? -1.0f : 1.0f;
        xy[3] = 0xFFFFF000000000L;
        xy[2] = 0xFFFFF000000000L;
        xy[1] = 0xFFFFF000000000L;
        xy[0] = 0xFFFFF000000000L;
        xy[0] += (unsigned int)((truncate_float_to_s32((D_L08_00161B64 + D_L08_00161B6C * sign_y) *
                                                       16.0f)
                                 << 16) -
                                (int)0x80000000);
        xy[0] += (truncate_float_to_s32(D_L08_00161B60 + D_L08_00161B68 * sign_x) << 4) + 0x8000;
        xy[1] +=
            (unsigned int)((truncate_float_to_s32(D_L08_00161B64 * 16.0f) << 16) - (int)0x80000000);
        xy[1] += (truncate_float_to_s32(D_L08_00161B60 + D_L08_00161B68 * sign_x) << 4) + 0x8000;
        xy[2] += (unsigned int)((truncate_float_to_s32((D_L08_00161B64 + D_L08_00161B6C * sign_y) *
                                                       16.0f)
                                 << 16) -
                                (int)0x80000000);
        xy[2] += (truncate_float_to_s32(D_L08_00161B60) << 4) + 0x8000;
        xy[3] +=
            (unsigned int)((truncate_float_to_s32(D_L08_00161B64 * 16.0f) << 16) - (int)0x80000000);
        xy[3] += (truncate_float_to_s32(D_L08_00161B60) << 4) + 0x8000;
        FUN_L02_0020bc88_c(xy, uv_words, color_words, texture, 1);
    }
    texture = get_effect_texture(D_L08_00161BA4);
    uv_words[0] = D_L08_00161BD0;
    uv_words[1] = D_L08_00161BD4;
    uv_words[2] = D_L08_00161BD8;
    uv_words[3] = D_L08_00161BDC;
    if ((float)data->health > 100.0f)
        color =
            func_001FA8A8_2E0258(0x6000FFFF, 0x6000FF00, ((float)data->health - 100.0f) / 100.0f);
    else
        color = func_001FA8A8_2E0258(0x600000FF, 0x6000FFFF, (float)data->health / 100.0f);
    segments = truncate_float_to_s32((1.0f - (float)data->health / 200.0f) * 32.0f);
    for (j = 0; j < 32; j++) {
        float angle, next_angle;
        if (j >= segments) {
            color_words[0] = color_words[1] = color_words[2] = color_words[3] = color;
        } else {
            color_words[0] = color_words[1] = color_words[2] = color_words[3] =
                (color & 0xFFFFFF) + 0x20000000;
        }
        angle = fast_normalize_angle((float)j * 0.19634955f);
        next_angle = fast_add_rotations(angle, 0.19634955f);
        outer_x = truncate_float_to_s32(D_L08_00161B70 * fast_sin(angle));
        outer_y = truncate_float_to_s32(D_L08_00161B74 * -fast_cos(angle));
        inner_x = truncate_float_to_s32(D_L08_00161B70 * fast_sin(next_angle));
        inner_y = truncate_float_to_s32(D_L08_00161B74 * -fast_cos(next_angle));
        xy[3] = 0xFFFFF000000000L;
        xy[2] = 0xFFFFF000000000L;
        xy[1] = 0xFFFFF000000000L;
        xy[0] = 0xFFFFF000000000L;
        xy[0] += (unsigned int)((truncate_float_to_s32((D_L08_00161B64 + (float)inner_y) * 16.0f)
                                 << 16) -
                                (int)0x80000000);
        xy[0] += (truncate_float_to_s32(D_L08_00161B60 + (float)inner_x) << 4) + 0x8000;
        xy[1] +=
            (unsigned int)((truncate_float_to_s32(D_L08_00161B64 * 16.0f) << 16) - (int)0x80000000);
        xy[1] += (truncate_float_to_s32(D_L08_00161B60) << 4) + 0x8000;
        xy[2] += (unsigned int)((truncate_float_to_s32((D_L08_00161B64 + (float)outer_y) * 16.0f)
                                 << 16) -
                                (int)0x80000000);
        xy[2] += (truncate_float_to_s32(D_L08_00161B60 + (float)outer_x) << 4) + 0x8000;
        xy[3] +=
            (unsigned int)((truncate_float_to_s32(D_L08_00161B64 * 16.0f) << 16) - (int)0x80000000);
        xy[3] += (truncate_float_to_s32(D_L08_00161B60) << 4) + 0x8000;
        FUN_L02_0020bc88_c(xy, uv_words, color_words, texture, 1);
    }
    if (scale_game_frames(15) < data->timer) {
        color = func_001FA8A8_2E0258(0x5000FFFF,
                                     0x500000FF,
                                     convert_integer_to_float(data->timer - scale_game_frames(15)) /
                                         (float)scale_game_frames(15));
    } else {
        color = func_001FA8A8_2E0258(0x5000FF00,
                                     0x5000FFFF,
                                     convert_integer_to_float(data->timer) /
                                         (float)scale_game_frames(15));
    }
    uv_words[0] = D_L08_00161BE0;
    uv_words[1] = D_L08_00161BE4;
    uv_words[2] = D_L08_00161BE8;
    uv_words[3] = D_L08_00161BEC;
    color_words[0] = color_words[1] = color_words[2] = color_words[3] = color;
    xy[3] = 0xFFFFF000000000L;
    xy[2] = 0xFFFFF000000000L;
    xy[1] = 0xFFFFF000000000L;
    xy[0] = 0xFFFFF000000000L;
    angle = fast_normalize_angle(fast_add_rotations((*(float(*)[4]) & moby->rot)[2] * 4.0f, 0.0f));
    xy[0] +=
        (unsigned int)((truncate_float_to_s32(fast_sin(angle) * D_L08_00161B7C * 16.0f) << 16) -
                       (int)0x80000000);
    xy[0] += truncate_float_to_s32(fast_cos(angle) * D_L08_00161B78 * 16.0f) + 0x8000;
    angle = fast_normalize_angle(
        fast_add_rotations((*(float(*)[4]) & moby->rot)[2] * 4.0f, 1.5707964f));
    xy[1] +=
        (unsigned int)((truncate_float_to_s32(fast_sin(angle) * D_L08_00161B7C * 16.0f) << 16) -
                       (int)0x80000000);
    xy[1] += truncate_float_to_s32(fast_cos(angle) * D_L08_00161B78 * 16.0f) + 0x8000;
    angle = fast_normalize_angle(
        fast_add_rotations((*(float(*)[4]) & moby->rot)[2] * 4.0f, -1.5707964f));
    xy[2] +=
        (unsigned int)((truncate_float_to_s32(fast_sin(angle) * D_L08_00161B7C * 16.0f) << 16) -
                       (int)0x80000000);
    xy[2] += truncate_float_to_s32(fast_cos(angle) * D_L08_00161B78 * 16.0f) + 0x8000;
    angle = fast_normalize_angle(
        fast_add_rotations((*(float(*)[4]) & moby->rot)[2] * 4.0f, 3.1415927f));
    xy[3] +=
        (unsigned int)((truncate_float_to_s32(fast_sin(angle) * D_L08_00161B7C * 16.0f) << 16) -
                       (int)0x80000000);
    xy[3] += truncate_float_to_s32(fast_cos(angle) * D_L08_00161B78 * 16.0f) + 0x8000;
    FUN_L02_0020bc88_c(xy, uv_words, color_words, get_effect_texture(D_L08_00161BA8), 1);
    xy[3] = 0xFFFFF000000000L;
    xy[2] = 0xFFFFF000000000L;
    xy[1] = 0xFFFFF000000000L;
    xy[0] = 0xFFFFF000000000L;
    angle = fast_normalize_angle(fast_add_rotations((*(float(*)[4]) & moby->rot)[1] * 8.0f, 0.0f));
    xy[0] +=
        (unsigned int)((truncate_float_to_s32(fast_sin(angle) * D_L08_00161B7C * 16.0f) << 16) -
                       (int)0x80000000);
    xy[0] += truncate_float_to_s32(fast_cos(angle) * D_L08_00161B78 * 16.0f) + 0x8000;
    angle = fast_normalize_angle(
        fast_add_rotations((*(float(*)[4]) & moby->rot)[1] * 8.0f, 1.5707964f));
    xy[1] +=
        (unsigned int)((truncate_float_to_s32(fast_sin(angle) * D_L08_00161B7C * 16.0f) << 16) -
                       (int)0x80000000);
    xy[1] += truncate_float_to_s32(fast_cos(angle) * D_L08_00161B78 * 16.0f) + 0x8000;
    angle = fast_normalize_angle(
        fast_add_rotations((*(float(*)[4]) & moby->rot)[1] * 8.0f, -1.5707964f));
    xy[2] +=
        (unsigned int)((truncate_float_to_s32(fast_sin(angle) * D_L08_00161B7C * 16.0f) << 16) -
                       (int)0x80000000);
    xy[2] += truncate_float_to_s32(fast_cos(angle) * D_L08_00161B78 * 16.0f) + 0x8000;
    angle = fast_normalize_angle(
        fast_add_rotations((*(float(*)[4]) & moby->rot)[1] * 8.0f, 3.1415927f));
    xy[3] +=
        (unsigned int)((truncate_float_to_s32(fast_sin(angle) * D_L08_00161B7C * 16.0f) << 16) -
                       (int)0x80000000);
    xy[3] += truncate_float_to_s32(fast_cos(angle) * D_L08_00161B78 * 16.0f) + 0x8000;
    FUN_L02_0020bc88_c(xy, uv_words, color_words, get_effect_texture(D_L08_00161BAC), 1);
}
/* Updates the mounted Batalia cannon: entry/exit, aiming, alternating barrels, waves, and attached visuals. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002E0258.c: func_L08_002E16A0), where it is exact; names translated to the US level program. */

typedef struct CannonData52 CannonData52;

struct CannonData52 {
    char          pad0[0x40];
    short         fire_timer, exit_timer;
    int           pad44;
    float         pitch_step, yaw_step;
    short         pad50, camera_timer, barrel, pad56;
    int           health;
    short         wave_timer;
    unsigned char target_index, target_timer;
    Moby         *linked[8];
    int           indices[4];
    Moby         *guns[2];
    float         range;
    int           platform_index, lock_timer;
    Moby         *visual;
    float         visual_angle;
};

char                 *FUN_L00_0026dd20(void *pos, void *dir, int a, int b, float f);
char                 *create_moby_with_particles(int    owner,
                                                 float *position,
                                                 float *rotation,
                                                 int    value) __asm__("FUN_L08_002e7eb0");
extern char           D_0013CAE0[];
extern float          D_0015ED70;
extern int D_0013CAE4 NOT_SDA; /* no foreign declaration */
extern int            D_0013F350;
extern int            D_0015EDDC;
extern int            D_L08_00161A90 __attribute__((sda));
extern float          D_L08_00161AE8_gp __asm__("D_L08_00161AE8") __attribute__((sda));
extern float          D_L08_00161AEC_gp __asm__("D_L08_00161AEC") __attribute__((sda));
extern float          D_L08_00161AF0_gp __asm__("D_L08_00161AF0") __attribute__((sda));
extern float          D_L08_00161AF4_gp __asm__("D_L08_00161AF4") __attribute__((sda));
extern float          D_L08_00161AF8_gp __asm__("D_L08_00161AF8") __attribute__((sda));
extern float          D_L08_00161AFC_gp __asm__("D_L08_00161AFC") __attribute__((sda));
extern float          D_L08_00161B00_gp __asm__("D_L08_00161B00") __attribute__((sda));
extern int            D_L08_00161B10 __attribute__((sda));
extern int            D_L08_00161B20 __attribute__((sda));
extern int            D_L08_00161B30 __attribute__((sda));
extern int            D_L08_00161B40 __attribute__((sda));
extern int            D_L08_00161B50 __attribute__((sda));
extern int            D_L08_00161BF0 __attribute__((sda));
extern int            FUN_L00_001fefc8();
extern int            divide_vector_xyz() __asm__("FUN_L00_001ff378");
extern int            FUN_L00_00211250();
extern int            FUN_L00_00257b90(int, int);
extern s32            D_0015ED84;
extern u8             D_0014C050[];
extern void           FUN_L08_00230b38(int, int);
void        set_race_camera(void *pos, void *rot, int mode, int flag) __asm__("FUN_L00_00216f90");
void        FUN_L08_00306b30(u8 *object, s32 mode);
void        FUN_L08_00313ba8(void);
extern char D_0013E633_2E16A0[] __asm__("D_0013CAE0");
extern char D_0013A5E0_2E16A0[] __asm__("D_0013CAE4");
extern unsigned char visited52[][16] __asm__("D_0014C050");
extern int           D_0015ED84;
extern short         D_L08_00161A90_gp __asm__("D_L08_00161A90") __attribute__((sda));
extern short         D_L08_00161B10_gp __asm__("D_L08_00161B10") __attribute__((sda));
extern short         D_L08_00161B20_gp __asm__("D_L08_00161B20") __attribute__((sda));
extern short         D_L08_00161B30_gp __asm__("D_L08_00161B30") __attribute__((sda));
extern short         D_L08_00161B40_gp __asm__("D_L08_00161B40") __attribute__((sda));
extern short         D_L08_00161B50_gp __asm__("D_L08_00161B50") __attribute__((sda));
extern short         D_L08_00161BF0_gp __asm__("D_L08_00161BF0") __attribute__((sda));
extern void          func_L00_001FF328_2E16A0(void *, void *, float) __asm__("FUN_L00_001ff378");
extern void          func_L00_00217718_2E16A0(void *, void *, int, int) __asm__("FUN_L00_00216f90");
extern void          func_L08_00307FF0_2E16A0(char *, int) __asm__("FUN_L08_00306b30");
extern float         func_002140F8_2E16A0(float, float) __asm__("FUN_002132a8");
extern float         func_00214D28_2E16A0(float *, float, float) __asm__("FUN_00213ed8");
extern char *func_L00_0026EBC0_2E16A0(char *, char *, int, int, float) __asm__("FUN_L00_0026dd20");
extern void  func_L08_00315068_2E16A0(void) __asm__("FUN_L08_00313ba8");

extern unsigned char l08_cannon_hero_interaction_flags[] __asm__("D_0014095F");
void                 update_batalia_cannon(Moby *moby) __asm__("FUN_L08_002e0328");

void update_batalia_cannon(Moby *moby) {
    enum {
        CANNON_INITIALIZE = 0,
        CANNON_UNMOUNTED_IDLE = 4,
        CANNON_MOUNTED_CONTROL = 5,
        CANNON_REMOVE = 6
    };
    enum {
        CANNON_NO_INACTIVE_INDEXED_TARGETS = 0,
        CANNON_ONE_INACTIVE_INDEXED_TARGET = 1,
        CANNON_TWO_INACTIVE_INDEXED_TARGETS = 2,
        CANNON_THREE_INACTIVE_INDEXED_TARGETS = 3,
        CANNON_ALL_INDEXED_TARGETS_INACTIVE = 4
    };
    float position[4], offset[4], work[4], direction[4], muzzle[4], scatter[4], random_vec[4];
    CannonData52 *data = (*(CannonData52 **)&moby->pvars);
    int           i, target, all_done, dead, best, min_life;
    Moby        **links;
    float         yaw, pitch, magnitude, best_angle;
    qcopy(position, (*(float(*)[4]) & moby->pos));
    position[2] += 0.4f;
    if (fast_dec_timer_s16(&data->exit_timer) && moby->command == 1) {
        qcopy(hero.motion.pos.f, position);
        qcopy(hero.motion.rot.f, (*(float(*)[4]) & moby->rot));
        offset[0] = fast_cos(hero.motion.rot.f[2]) * -0.7f;
        offset[1] = fast_sin(hero.motion.rot.f[2]) * -0.7f;
        offset[2] = 0.0f;
        fast_vec_add(hero.motion.pos.f, hero.motion.pos.f, offset);
        fast_mem_zero16((char *)hero.motion.pos.f + 0x60, 0x90);
        if (data->health == 0) {
            Moby *node;
            (*(float(*)[4]) & moby->rot)[1] = 0.0f;
            moby->command = 2;
            jet_end_mount(0);
            moby->state = 4;
            node = (Moby *)(D_L08_0015FFD8 + (data->platform_index << 8));
            qcopy(direction, (*(float(*)[4]) & node->rot));
            direction[2] = fast_add_rotations(direction[2], 3.1415927f);
            fast_vec_normalize(work, (*(float(*)[16]) & node->rot_row0), 2.5f);
            fast_vec_add(work, work, (*(float(*)[4]) & node->pos));
            func_L00_00217718_2E16A0(work, direction, 0, 1);
            func_L08_00307FF0_2E16A0((char *)node, 0);
            return;
        }
        if (visited52[D_0015ED84][moby->mission] != 0xFF && (hero.ship_flags & 1))
            hero_respawn();
        if (((*(int *)D_0013A5E0_2E16A0 & 0x10) ||
             (*(unsigned char *)(l08_cannon_hero_interaction_flags) & 1)) &&
            visited52[D_0015ED84][moby->mission] == 0xFF) {
            hero.ship_flags = 0;
            hero_set_state(0, 1);
            moby->command = 2;
            (*(float(*)[4]) & moby->rot)[1] = 0.0f;
            euler_rotation_basis((*(float(*)[16]) & moby->rot_row0), (*(float(*)[4]) & moby->rot));
            clear_u64_value(direction);
            direction[2] = (*(float(*)[4]) & moby->rot)[2];
            transform_vector_by_basis(work, &D_L08_00161A90_gp, (*(float(*)[16]) & moby->rot_row0));
            fast_vec_add(work, work, (*(float(*)[4]) & moby->pos));
            func_L00_00217718_2E16A0(work, direction, 0, 1);
            jet_end_mount(0);
            hero.motion.rot.f[2] = fast_add_rotations(hero.motion.rot.f[2], 3.1415927f);
            moby->state = 4;
        } else {
            int *indices;
            int  count;
            if (moby->state != 5) {
                data->camera_timer = 2;
                jet_mount((*(float(*)[4]) & moby->pos), (*(float(*)[4]) & moby->rot), 1, 0, 0);
            }
            moby->state = 5;
            {
                int waiting = 2, activated = 3;
                indices = data->indices;
                for (count = 3; count >= 0; count--) {
                    if (((Moby *)D_L08_0015FFD8)[*indices].state == waiting)
                        ((Moby *)D_L08_0015FFD8)[*indices].state = activated;
                    indices++;
                }
            }
        }
    } else if (moby->command == 2) {
        if (vector_distance_xy(position, (*(float(*)[4]) & hero.moby->pos)) > 1.0f) {
            moby->command = 0;
            data->exit_timer = scale_game_frames(60);
        }
    } else if (visited52[D_0015ED84][moby->mission] == 0xFF) {
        if (vector_distance_xy(position, (*(float(*)[4]) & hero.moby->pos)) < 1.0f &&
            hero.motion.pos.f[2] - (*(float(*)[4]) & moby->pos)[2] < 7.0f &&
            data->exit_timer == 0 && data->health > 0) {
            moby->command = 1;
            hero_set_state(50, 1);
        }
    }
    switch (moby->state) {
    case CANNON_INITIALIZE: {
        Moby *node;
        int  *indices;
        int   count;
        moby->state = 4;
        data->health = 200;
        data->range = 10.0f;
        node = create_moby(0x710);
        data->visual = node;
        if (node) {
            qcopy((*(float(*)[4]) & node->pos), (*(float(*)[4]) & moby->pos));
            qcopy((*(float(*)[4]) & node->rot), (*(float(*)[4]) & moby->rot));
            node->draw_dist = 0x30;
            data->visual->visible = 1;
            (*(long *)&data->visual->spawn_frame) = (*(long *)&moby->spawn_frame);
        }
        (*(float(*)[4]) & moby->rot)[1] = 0.0f;
        data->fire_timer = scale_game_frames(30);
        data->exit_timer = 0;
        data->pitch_step = 0.0f;
        data->yaw_step = 0.0f;
        indices = (int *)&data->linked[7];
        count = 7;
        do {
            *indices-- = 0;
        } while (--count >= 0);
        data->wave_timer = scale_game_frames(180);
        data->target_timer = 0;
        data->target_index = 0;
        if (visited52[D_0015ED84][moby->mission] != 0xFF)
            data->visual_angle = 0.0f;
        else
            data->visual_angle = 1.5707964f;
        break;
    }
    case CANNON_MOUNTED_CONTROL:
        if (data->health < 0)
            data->health = 0;
        work[0] = hero.stick_x;
        work[1] = invert52 ? hero.stick_y : -hero.stick_y;
        magnitude = vector_length_xy(work);
        if (magnitude > 1.0f)
            func_L00_001FF328_2E16A0(work, work, magnitude);
        yaw = D_L08_00161AE8_gp * 0.017453292f * D_0015ED6C * -work[0];
        pitch = -(D_L08_00161AEC_gp * 0.017453292f * D_0015ED6C) * work[1];
        if ((data->yaw_step >= 0.0f && data->yaw_step < yaw) ||
            (data->yaw_step <= 0.0f && yaw < data->yaw_step))
            func_00214D28_2E16A0(
                &data->yaw_step, yaw, D_L08_00161AF0_gp * 0.017453292f * D_0015ED70);
        else
            func_00214D28_2E16A0(
                &data->yaw_step, yaw, D_L08_00161AF8_gp * 0.017453292f * D_0015ED70);
        if ((data->pitch_step >= 0.0f && data->pitch_step < pitch) ||
            (data->pitch_step <= 0.0f && pitch < data->pitch_step))
            func_00214D28_2E16A0(
                &data->pitch_step, pitch, D_L08_00161AF4_gp * 0.017453292f * D_0015ED70);
        else
            func_00214D28_2E16A0(
                &data->pitch_step, pitch, D_L08_00161AFC_gp * 0.017453292f * D_0015ED70);
        (*(float(*)[4]) & moby->rot)[2] =
            fast_add_rotations((*(float(*)[4]) & moby->rot)[2], data->yaw_step);
        (*(float(*)[4]) & moby->rot)[1] =
            fast_add_rotations((*(float(*)[4]) & moby->rot)[1], data->pitch_step);
        if ((*(float(*)[4]) & moby->rot)[1] < -0.7853982f)
            (*(float(*)[4]) & moby->rot)[1] = -0.7853982f;
        else if ((*(float(*)[4]) & moby->rot)[1] > 0.34906584f)
            (*(float(*)[4]) & moby->rot)[1] = 0.34906584f;
        target = find_batalia_cannon_aim_target((char *)moby);
        if (target) {
            if (data->lock_timer < scale_game_frames(30))
                data->lock_timer++;
        } else if (data->lock_timer)
            data->lock_timer--;
        fast_dec_timer_s16(&data->fire_timer);
        if ((*(int *)D_0013CAE0 & 0x20) && data->fire_timer == 0) {
            if (data->barrel == 1) {
                int count;
                build_spherical_offset(direction,
                                       D_L08_00161B00_gp * D_0015ED6C,
                                       (*(float(*)[4]) & moby->rot)[2],
                                       -(*(float(*)[4]) & moby->rot)[1]);
                blend_moby_animation(data->guns[0], 1, 0, 0);
                transform_vector_by_basis(
                    muzzle, &D_L08_00161B30_gp, (*(float(*)[16]) & moby->rot_row0));
                fast_vec_add(muzzle, (*(float(*)[4]) & moby->pos), muzzle);
                create_moby_with_particles(moby, muzzle, direction, target);
                allocate_voice_for_target_entry(0, 0, moby);
                l08_camera.shake_amount = 0.03f;
                l08_camera.shake_frames = scale_game_frames(10);
                l08_camera.shake2_amount = 0.03f;
                l08_camera.shake2_frames = scale_game_frames(7);
                data->barrel = 0;
                data->fire_timer = scale_game_frames(20);
                count = 4;
                do {
                    *(u128 *)random_vec = 0;
                    random_vec[0] = func_002140F8_2E16A0(-1.0f, 1.0f);
                    random_vec[1] = func_002140F8_2E16A0(-1.0f, 1.0f);
                    random_vec[2] = func_002140F8_2E16A0(-1.0f, 1.0f);
                    *(u128 *)scatter = *(u128 *)random_vec;
                    fast_vec_normalize(scatter,
                                       scatter,
                                       fast_vec_length(direction) *
                                           (frame_scale * -0.35000002f + 1.0f));
                    fast_vec_add(scatter, direction, scatter);
                    fast_vec_normalize(scatter,
                                       scatter,
                                       func_002140F8_2E16A0(D_0015ED6C * 3.0f, D_0015ED6C * 6.0f));
                    min_life = scale_game_frames(5);
                    func_L00_0026EBC0_2E16A0((char *)muzzle,
                                             (char *)scatter,
                                             0x5F2F4F6F,
                                             random_int_between(min_life, scale_game_frames(10)),
                                             50000.0f);
                } while (--count >= 0);
                fast_vec_normalize(direction, direction, D_0015ED6C * 0.5f);
                min_life = scale_game_frames(4);
                func_L00_0026EBC0_2E16A0((char *)muzzle,
                                         (char *)direction,
                                         0x2F4F7F7F,
                                         random_int_between(min_life, scale_game_frames(7)),
                                         500000.0f);
            } else {
                int count;
                build_spherical_offset(direction,
                                       D_L08_00161B00_gp * D_0015ED6C,
                                       (*(float(*)[4]) & moby->rot)[2],
                                       -(*(float(*)[4]) & moby->rot)[1]);
                blend_moby_animation(data->guns[1], 1, 0, 0);
                transform_vector_by_basis(
                    muzzle, &D_L08_00161B40_gp, (*(float(*)[16]) & moby->rot_row0));
                fast_vec_add(muzzle, (*(float(*)[4]) & moby->pos), muzzle);
                create_moby_with_particles(
                    moby, muzzle, direction, find_batalia_cannon_aim_target((char *)moby));
                allocate_voice_for_target_entry(0, 0, moby);
                l08_camera.shake_amount = 0.03f;
                l08_camera.shake_frames = scale_game_frames(10);
                l08_camera.shake2_amount = 0.03f;
                l08_camera.shake2_frames = scale_game_frames(7);
                data->barrel = 1;
                data->fire_timer = scale_game_frames(20);
                count = 4;
                do {
                    *(u128 *)random_vec = 0;
                    random_vec[0] = func_002140F8_2E16A0(-1.0f, 1.0f);
                    random_vec[1] = func_002140F8_2E16A0(-1.0f, 1.0f);
                    random_vec[2] = func_002140F8_2E16A0(-1.0f, 1.0f);
                    *(u128 *)scatter = *(u128 *)random_vec;
                    fast_vec_normalize(scatter,
                                       scatter,
                                       fast_vec_length(direction) *
                                           (frame_scale * -0.35000002f + 1.0f));
                    fast_vec_add(scatter, direction, scatter);
                    fast_vec_normalize(scatter,
                                       scatter,
                                       func_002140F8_2E16A0(D_0015ED6C * 3.0f, D_0015ED6C * 6.0f));
                    min_life = scale_game_frames(5);
                    func_L00_0026EBC0_2E16A0((char *)muzzle,
                                             (char *)scatter,
                                             0x5F2F4F6F,
                                             random_int_between(min_life, scale_game_frames(10)),
                                             50000.0f);
                } while (--count >= 0);
                fast_vec_normalize(direction, direction, D_0015ED6C * 0.5f);
                min_life = scale_game_frames(4);
                func_L00_0026EBC0_2E16A0((char *)muzzle,
                                         (char *)direction,
                                         0x2F4F7F7F,
                                         random_int_between(min_life, scale_game_frames(7)),
                                         500000.0f);
            }
        }
        if (fast_dec_timer_s16(&data->camera_timer)) {
            func_L08_00315068_2E16A0();
            transform_vector_by_basis(
                direction, &D_L08_00161B50_gp, (*(float(*)[16]) & moby->rot_row0));
            fast_vec_add(direction, direction, (*(float(*)[4]) & moby->pos));
            set_camera_position(direction);
            set_camera_rotation((*(float(*)[4]) & moby->rot));
        }
        add_draw_callback(draw_batalia_cannon_hud, moby);
        if (fast_dec_timer_s16(&data->wave_timer)) {
            int count;
            dead = 0;
            all_done = 1;
            links = data->linked;
            count = 7;
            do {
                if (*links && (signed char)(*links)->state >= 0)
                    all_done = (*links)->state == 2 ? all_done : 0;
                links++;
            } while (--count >= 0);
            if (all_done) {
                int  cutoff = -1;
                int *indices = data->indices;
                int *walk = indices;
                count = 3;
                do {
                    Moby *node;
                    node = (Moby *)((*walk << 8) + (unsigned int)D_L08_0015FFD8);
                    if ((signed char)node->state <= cutoff)
                        dead++;
                    walk++;
                } while (--count >= 0);
                switch (dead) {
                case CANNON_NO_INACTIVE_INDEXED_TARGETS:
                    data->range = 10.0f;
                    data->wave_timer = scale_game_frames(285);
                    break;
                case CANNON_ONE_INACTIVE_INDEXED_TARGET:
                    data->range = 12.5f;
                    data->wave_timer = scale_game_frames(240);
                    break;
                case CANNON_TWO_INACTIVE_INDEXED_TARGETS:
                    data->range = 15.0f;
                    data->wave_timer = scale_game_frames(195);
                    break;
                case CANNON_THREE_INACTIVE_INDEXED_TARGETS:
                    data->range = 17.5f;
                    data->wave_timer = scale_game_frames(150);
                    break;
                case CANNON_ALL_INDEXED_TARGETS_INACTIVE:
                    if (visited52[D_0015ED84][moby->mission] != 0xFF) {
                        Moby *node;
                        (*(float(*)[4]) & moby->rot)[1] = 0.0f;
                        moby->command = 2;
                        jet_end_mount(0);
                        moby->state = 4;
                        node = (Moby *)(D_L08_0015FFD8 + (data->platform_index << 8));
                        qcopy(muzzle, (*(float(*)[4]) & node->rot));
                        muzzle[2] = fast_add_rotations(muzzle[2], 3.1415927f);
                        fast_vec_normalize(direction, (*(float(*)[16]) & node->rot_row0), 2.5f);
                        fast_vec_add(direction, direction, (*(float(*)[4]) & node->pos));
                        func_L00_00217718_2E16A0(direction, muzzle, 0, 1);
                        func_L08_00307FF0_2E16A0((char *)node, 1);
                        data->visual_angle = 1.5707964f;
                        return;
                    }
                    break;
                }
                if (fast_subtract_rotations_cf(&data->target_timer) ||
                    (signed char)((Moby *)D_L08_0015FFD8)[*(int *)((unsigned int)indices +
                                                                   (data->target_index << 2))]
                            .state < 0) {
                    data->target_timer = 1;
                    best_angle = 3.1415927f;
                    best = -1;
                    for (i = 0; i < 4; i++) {
                        Moby *node;
                        node = (Moby *)(D_L08_0015FFD8 + (indices[i] << 8));
                        if (fast_difference_between_rotations(
                                fast_arc_tan((*(float(*)[4]) & node->pos)[0] - hero.motion.pos.f[0],
                                             (*(float(*)[4]) & node->pos)[1] -
                                                 hero.motion.pos.f[1]),
                                hero.motion.rot.f[2]) < best_angle &&
                            node->state < 0x7F) {
                            best = i;
                            best_angle = fast_difference_between_rotations(
                                fast_arc_tan((*(float(*)[4]) & node->pos)[0] - hero.motion.pos.f[0],
                                             (*(float(*)[4]) & node->pos)[1] -
                                                 hero.motion.pos.f[1]),
                                hero.motion.rot.f[2]);
                        }
                    }
                    data->target_index = best;
                }
                ((Moby *)
                     D_L08_0015FFD8)[*(int *)((unsigned int)indices + (data->target_index << 2))]
                    .command = 1;
            }
        }
        break;
    case CANNON_UNMOUNTED_IDLE:
        break;
    case CANNON_REMOVE:
        mark_moby_for_removal(data->guns[0]);
        mark_moby_for_removal(data->guns[1]);
        mark_moby_for_removal(moby);
        return;
    }
    if (data->guns[0] == 0) {
        Moby *node;
        node = create_moby(0x236);
        data->guns[0] = node;
        if (node) {
            node->update_dist = 0xFF;
            data->guns[0]->draw_dist = 0x7E;
            data->guns[0]->visible = 1;
            (*(long *)&data->guns[0]->spawn_frame) = (*(long *)&moby->spawn_frame);
            transform_vector_by_basis(work, &D_L08_00161B10_gp, (*(float(*)[16]) & moby->rot_row0));
            fast_vec_add(work, (*(float(*)[4]) & moby->pos), work);
            {
                Moby *gun = data->guns[0];
                qcopy((*(float(*)[4]) & gun->pos), work);
                qcopy((*(float(*)[4]) & gun->rot), (*(float(*)[4]) & moby->rot));
                update_entity(gun);
            }
        }
    } else {
        transform_vector_by_basis(work, &D_L08_00161B10_gp, (*(float(*)[16]) & moby->rot_row0));
        fast_vec_add(work, (*(float(*)[4]) & moby->pos), work);
        {
            Moby *gun = data->guns[0];
            qcopy((*(float(*)[4]) & gun->pos), work);
            qcopy((*(float(*)[4]) & gun->rot), (*(float(*)[4]) & moby->rot));
            if ((gun->anim_flags & 2) && gun->prev_seq == 1)
                blend_moby_animation(gun, 0, 0, 0);
        }
    }

    if (data->guns[1] == 0) {
        Moby *node;
        node = create_moby(0x236);
        data->guns[1] = node;
        if (node) {
            node->update_dist = 0xFF;
            data->guns[1]->draw_dist = 0x7E;
            data->guns[1]->visible = 1;
            (*(long *)&data->guns[1]->spawn_frame) = (*(long *)&moby->spawn_frame);
            transform_vector_by_basis(work, &D_L08_00161B20_gp, (*(float(*)[16]) & moby->rot_row0));
            fast_vec_add(work, (*(float(*)[4]) & moby->pos), work);
            {
                Moby *gun = data->guns[1];
                qcopy((*(float(*)[4]) & gun->pos), work);
                qcopy((*(float(*)[4]) & gun->rot), (*(float(*)[4]) & moby->rot));
                update_entity(gun);
            }
        }
    } else {
        transform_vector_by_basis(work, &D_L08_00161B20_gp, (*(float(*)[16]) & moby->rot_row0));
        fast_vec_add(work, (*(float(*)[4]) & moby->pos), work);
        {
            Moby *gun = data->guns[1];
            qcopy((*(float(*)[4]) & gun->pos), work);
            qcopy((*(float(*)[4]) & gun->rot), (*(float(*)[4]) & moby->rot));
            if ((gun->anim_flags & 2) && gun->prev_seq == 1)
                blend_moby_animation(gun, 0, 0, 0);
        }
    }

    if (data->visual) {
        transform_vector_by_basis(work, &D_L08_00161BF0_gp, (*(float(*)[16]) & moby->rot_row0));
        fast_vec_add((*(float(*)[4]) & data->visual->pos), work, (*(float(*)[4]) & moby->pos));
        {
            Moby *visual = data->visual;
            qcopy((*(float(*)[4]) & visual->rot), (*(float(*)[4]) & moby->rot));
            (*(float(*)[4]) & data->visual->rot)[1] =
                fast_add_rotations((*(float(*)[4]) & visual->rot)[1], data->visual_angle);
        }
        update_entity(data->visual);
    }
}

#include "sda.h"

/* Update function for moby classes 441-443 on level 08 (falling debris): picks its spin, falls with a smoke trail, bursts into particles near the ground, then falls on until out of bounds. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002E0258.c: func_L08_002E2A38), where it is exact; names translated to the US level program. */

typedef struct {
    float vel[4]; /* 0x00 */
    float spin_x; /* 0x10 */
    float spin_y; /* 0x14 */
    int   life;   /* 0x18 */
} Data;

typedef struct {
    char          pad0[0x10];
    float         pos[4]; /* 0x10 */
    unsigned char state;  /* 0x20 */
    char          pad21[0x1F];
    float         rot[4]; /* 0x40 */
    char          pad50[0x28];
    Data         *data; /* 0x78 */
} Moby_2E2A38;

typedef struct {
    int v[2];
} Pair_2E2A38;

extern Pair_2E2A38 D_L08_00161C30 NOT_SDA;
extern float                      D_0015ED6C_gp __asm__("D_0015ED6C") MACRO_ADDR;
extern float D_0015ED70           MACRO_ADDR;
extern float                      FUN_001f96b0_c(float) __asm__("FUN_001f96b0");

#include "rnc/overlay/l08/mobys/bolt_thief_data/update_falling_debris_2.h"

extern int D_L08_00161C04 __attribute__((sda));
extern int D_L08_00161C08 __attribute__((sda));

#include "rnc/overlay/l08/mobys/bolt_thief_data/update_falling_debris.h"

extern int  spawn_spark() __asm__("FUN_00218888");
void        spawn_particle(void *pos,
                           void *dir,
                           int   col,
                           int   d,
                           int   n,
                           int   b1A,
                           int   b18,
                           int   b19,
                           float x,
                           float y) __asm__("FUN_L00_0026a9f0");
extern void func_L00_0026B890_2E2A38(
    void *, void *, int, int, int, int, int, int, float, float) __asm__("FUN_L00_0026a9f0");

void update_falling_debris(Moby_2E2A38 *debris_moby) __asm__("FUN_L08_002e16c0");

void update_falling_debris(Moby_2E2A38 *debris_moby) {
    enum {
        FALLING_DEBRIS_INITIALIZE = 1,
        FALLING_DEBRIS_BEFORE_BURST = 2,
        FALLING_DEBRIS_AFTER_BURST = 3
    };
    Data       *debris_data = debris_moby->data;
    float       particle_primary_vector[4];
    Pair_2E2A38 tab;
    float       particle_secondary_vector[4];
    float       particle_spawn_position[4];
    int         i;

    switch (debris_moby->state) {
    case FALLING_DEBRIS_INITIALIZE:
        debris_data->spin_x =
            random_float_between(-(D_0015ED6C_gp * 1.5707964f), D_0015ED6C_gp * 1.5707964f);
        debris_data->spin_y =
            random_float_between(-(D_0015ED6C_gp * 1.5707964f), D_0015ED6C_gp * 1.5707964f);
        debris_data->life = (int)random_float_between((float)scale_game_frames(0x5A),
                                                      (float)scale_game_frames(0x96));
        debris_moby->state = 2;
    case FALLING_DEBRIS_BEFORE_BURST:
        debris_data->vel[2] -= D_0015ED70 * 10.8f * 0.5f;
        fast_vec_add(debris_moby->pos, debris_moby->pos, debris_data);
        debris_moby->rot[0] = fast_add_rotations(debris_moby->rot[0], debris_data->spin_x);
        debris_moby->rot[1] = fast_add_rotations(debris_moby->rot[1], debris_data->spin_y);
        if (debris_moby->pos[0] < 8.0f || debris_moby->pos[1] < 8.0f ||
            debris_moby->pos[0] > 500.0f || debris_moby->pos[1] > 500.0f ||
            debris_moby->pos[2] > 500.0f) {
            mark_moby_for_removal(debris_moby);
            return;
        }
        if (random_integer_below(3) == 0) {
            int *trail_parameter_entry;
            int  trail_frame_count;
            int  trail_secondary_frame_count;
            clear_u64_value(particle_primary_vector);
            tab = D_L08_00161C30;
            trail_parameter_entry = &tab.v[random_integer_below(2)];
            {
                int lo = scale_game_frames(15), hi = scale_game_frames(20);
                trail_frame_count = FUN_L00_00257b90(lo, hi);
            }
            {
                int lo = scale_game_frames(25);
                trail_secondary_frame_count = FUN_L00_00257b90(lo, scale_game_frames(30));
            }
            func_L00_0026B890_2E2A38(debris_moby->pos,
                                     particle_primary_vector,
                                     0x2F3F3F7F,
                                     *trail_parameter_entry,
                                     trail_frame_count,
                                     trail_secondary_frame_count,
                                     0,
                                     0,
                                     400000.0f,
                                     random_float_between(8.0f, 16.0f) * D_0015ED6C_gp);
        }
        if (debris_moby->pos[2] < 15.2f) {
            for (i = 0; i < falling_debris_burst_particle_count; i++) {
                float particle_planar_angle = random_angle_radians();
                int   burst_frame_count;
                int   burst_secondary_frame_count;
                int   burst_tertiary_frame_count;
                qcopy(particle_spawn_position, debris_moby->pos);
                particle_spawn_position[0] += random_float_between(-3.0f, 3.0f);
                particle_spawn_position[1] += random_float_between(-3.0f, 3.0f);
                particle_primary_vector[2] =
                    falling_debris_burst_primary_vertical_scale * D_0015ED6C_gp;
                particle_primary_vector[0] +=
                    fast_cos(particle_planar_angle) * falling_debris_burst_primary_planar_scale;
                particle_primary_vector[1] +=
                    fast_sin(particle_planar_angle) * falling_debris_burst_primary_planar_scale;
                particle_secondary_vector[2] =
                    falling_debris_burst_secondary_vertical_scale * D_0015ED6C_gp;
                particle_secondary_vector[0] +=
                    fast_cos(particle_planar_angle) * falling_debris_burst_secondary_planar_scale;
                particle_secondary_vector[1] +=
                    fast_sin(particle_planar_angle) * falling_debris_burst_secondary_planar_scale;
                particle_primary_vector[3] = falling_debris_burst_primary_vector_w;
                particle_secondary_vector[3] = falling_debris_burst_secondary_vector_w;
                burst_frame_count = truncate_float_to_s32(FUN_001f96b0_c(random_float_between(
                    (float)falling_debris_burst_frame_count_random_min,
                    (float)(falling_debris_burst_frame_count_random_min * 2))));
                burst_secondary_frame_count =
                    truncate_float_to_s32(FUN_001f96b0_c(random_float_between(
                        (float)falling_debris_burst_secondary_frame_count_random_min,
                        (float)(falling_debris_burst_secondary_frame_count_random_min * 2))));
                burst_tertiary_frame_count =
                    truncate_float_to_s32(FUN_001f96b0_c(random_float_between(
                        (float)falling_debris_burst_tertiary_frame_count_random_min,
                        (float)(falling_debris_burst_tertiary_frame_count_random_min * 2))));
                spawn_spark(particle_spawn_position,
                            particle_primary_vector,
                            particle_secondary_vector,
                            D_L08_00161C04,
                            D_L08_00161C08,
                            burst_frame_count,
                            burst_secondary_frame_count,
                            burst_tertiary_frame_count,
                            -1);
            }
            debris_moby->state = 3;
        }
        break;
    case FALLING_DEBRIS_AFTER_BURST:
        debris_data->vel[2] -= D_0015ED70 * 10.8f * 0.5f;
        fast_vec_add(debris_moby->pos, debris_moby->pos, debris_data);
        debris_moby->rot[0] = fast_add_rotations(debris_moby->rot[0], debris_data->spin_x);
        debris_moby->rot[1] = fast_add_rotations(debris_moby->rot[1], debris_data->spin_y);
        if (debris_moby->pos[0] < 8.0f || debris_moby->pos[1] < 8.0f ||
            debris_moby->pos[2] < 8.0f || debris_moby->pos[0] > 500.0f ||
            debris_moby->pos[1] > 500.0f) {
            mark_moby_for_removal(debris_moby);
        }
        break;
    }
}
/* Builds the target board: spawns the frame, the three rows of eight targets
 * and their mounts, and records each piece with the piece it hangs from. */

typedef struct {
    struct Moby *moby;
    struct Moby *anchor;
    s32          joint;  /* joint on this piece */
    s32          pjoint; /* joint on the anchor it hangs from */
} TargetSlot;

typedef struct {
    u8         pad0[0x60];
    TargetSlot slots[34];
    u8         pad280[0xC];
    s16        points;
} TargetBoardVars;

void spawn_gunship_parts(struct Moby *moby) __asm__("FUN_L08_002e1d70");

void spawn_gunship_parts(struct Moby *moby) {
    TargetBoardVars *vars = (TargetBoardVars *)moby->pvars;
    struct Moby     *child_moby;
    s32              i;
    s32              pjoint;

    vars->slots[0].moby = moby;
    vars->slots[0].anchor = 0;
    vars->slots[0].joint = 0;
    vars->slots[0].pjoint = 0;
    vars->slots[3].moby = spawn_joint_aligned_moby_part((char *)moby, 0x1C1, 0, 2);
    vars->slots[3].anchor = moby;
    vars->slots[3].joint = 0;
    vars->slots[3].pjoint = 2;
    pjoint = 3;
    vars->slots[1].moby = spawn_joint_aligned_moby_part((char *)moby, 0x1C2, 0, pjoint);
    vars->slots[1].anchor = moby;
    vars->slots[1].joint = 0;
    vars->slots[1].pjoint = pjoint;
    child_moby = spawn_joint_aligned_moby_part((char *)moby, 0x1C2, 0, 4);
    vars->slots[2].moby = child_moby;
    child_moby->flags |= 0x8000;
    vars->slots[2].anchor = moby;
    vars->slots[2].joint = 0;
    vars->slots[2].pjoint = 4;
    vars->slots[7].moby = spawn_joint_aligned_moby_part((char *)moby, 0x1C0, 0, 0);
    vars->slots[7].anchor = moby;
    vars->slots[7].joint = 0;
    vars->slots[7].pjoint = 0;
    vars->slots[8].moby = spawn_joint_aligned_moby_part((char *)moby, 0x1C0, 1, 1);
    vars->slots[8].anchor = moby;
    vars->slots[8].joint = 1;
    vars->slots[8].pjoint = 1;
    vars->slots[9].moby = spawn_joint_aligned_moby_part((char *)vars->slots[8].moby, 0x1C0, 1, 0);
    vars->slots[9].anchor = vars->slots[8].moby;
    vars->slots[9].joint = 1;
    vars->slots[9].pjoint = 0;
    vars->slots[5].moby = spawn_joint_aligned_moby_part((char *)vars->slots[9].moby, 0x1BD, 0, 0);
    vars->slots[5].anchor = vars->slots[9].moby;
    vars->slots[5].joint = 0;
    vars->slots[5].pjoint = 0;
    vars->slots[6].moby = spawn_joint_aligned_moby_part((char *)vars->slots[7].moby, 0x1BA, 1, 1);
    vars->slots[6].anchor = vars->slots[7].moby;
    vars->slots[6].joint = 1;
    vars->slots[6].pjoint = 1;
    vars->slots[4].moby = spawn_joint_aligned_moby_part((char *)moby, 0x1B9, 0, 0);
    vars->slots[4].anchor = vars->slots[6].moby;
    vars->slots[4].joint = 0;
    vars->slots[4].pjoint = 0;
    for (i = 0; i < 8; i++) {
        vars->slots[10 + i].moby =
            spawn_joint_aligned_moby_part((char *)vars->slots[7].moby, 0x1BB, 0, i + 2);
        vars->slots[10 + i].anchor = vars->slots[7].moby;
        vars->slots[10 + i].joint = 0;
        vars->slots[10 + i].pjoint = i + 2;
    }
    for (i = 0; i < 8; i++) {
        vars->slots[18 + i].moby =
            spawn_joint_aligned_moby_part((char *)vars->slots[8].moby, 0x1BB, 0, i + 2);
        vars->slots[18 + i].anchor = vars->slots[8].moby;
        vars->slots[18 + i].joint = 0;
        vars->slots[18 + i].pjoint = i + 2;
    }
    for (i = 0; i < 8; i++) {
        vars->slots[26 + i].moby =
            spawn_joint_aligned_moby_part((char *)vars->slots[9].moby, 0x1BB, 0, i + 2);
        vars->slots[26 + i].anchor = vars->slots[9].moby;
        vars->slots[26 + i].joint = 0;
        vars->slots[26 + i].pjoint = i + 2;
    }
}

#include "rnc/hero/hero.h"

extern float gunship_part_spin_speed_degrees_gp __asm__("D_L08_00161C3C") __attribute__((sda));

typedef struct {
    struct Moby *part; /* moved along with the anchor's joint */
    struct Moby *anchor;
    int          part_joint;
    int          anchor_joint;
} PartLink;

typedef struct {
    u8       pad0[0x60];
    PartLink links[34]; /* 0x60: entry 0 is unused */
} LinkedPartsVars;

/* Keeps the 33 linked parts glued to their anchors' joints and facing the moby, spins three of them (the middle
   one backwards), turns link 3 towards the hero, then updates every part. */
void update_linked_moby_parts(struct Moby *moby) __asm__("FUN_L08_002e2078");

void update_linked_moby_parts(struct Moby *moby) {
    LinkedPartsVars *linked_parts_vars = (LinkedPartsVars *)moby->pvars;
    PartLink        *links = linked_parts_vars->links;
    float            part_joint_position[4];
    float            anchor_joint_position[4];
    float            joint_alignment_offset[4];
    int              i;

    for (i = 1; i < 34; i++) {
        struct Moby *part = linked_parts_vars->links[i].part;
        if (part != 0) {
            part->rot.z = moby->rot.z;
            moby_point_position(part, linked_parts_vars->links[i].part_joint, part_joint_position);
            moby_point_position(linked_parts_vars->links[i].anchor,
                                linked_parts_vars->links[i].anchor_joint,
                                anchor_joint_position);
            fast_vec_sub(joint_alignment_offset, anchor_joint_position, part_joint_position);
            fast_vec_add(&part->pos, &part->pos, joint_alignment_offset);
        }
    }
    if (linked_parts_vars->links[7].part != 0) {
        linked_parts_vars->links[7].part->rot.x =
            fast_add_rotations(linked_parts_vars->links[7].part->rot.x,
                               gunship_part_spin_speed_degrees_gp * DEG_TO_RAD * D_0015ED6C);
    }
    if (linked_parts_vars->links[8].part != 0) {
        linked_parts_vars->links[8].part->rot.x =
            fast_add_rotations(linked_parts_vars->links[8].part->rot.x,
                               -(gunship_part_spin_speed_degrees_gp * DEG_TO_RAD * D_0015ED6C));
    }
    if (linked_parts_vars->links[9].part != 0) {
        linked_parts_vars->links[9].part->rot.x =
            fast_add_rotations(linked_parts_vars->links[9].part->rot.x,
                               gunship_part_spin_speed_degrees_gp * DEG_TO_RAD * D_0015ED6C);
    }
    if (linked_parts_vars->links[3].part != 0) {
        linked_parts_vars->links[3].part->rot.z =
            fast_arc_tan(hero.motion.pos.f[0] - linked_parts_vars->links[3].part->pos.x,
                         hero.motion.pos.f[1] - linked_parts_vars->links[3].part->pos.y);
    }
    for (i = 1; i < 34; i++) {
        if (links[i].part != 0) {
            update_entity(links[i].part);
        }
    }
}
/* Selects a damaged child moby, launches it, and advances the parent's state. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002E0258.c: func_L08_002E35C8), where it is exact; names translated to the US level program. */

extern void *FUN_L00_0025a420(void *, int, int);

void process_gunship_part_hits(struct Moby *moby) __asm__("FUN_L08_002e2250");

void process_gunship_part_hits(struct Moby *moby) {
    char *data = *(char **)((char *)moby + 0x78);
    char *entries = data + 0x60;
    char *part_entry_cursor = entries;
    int   i;
    int   score = 0;
    int   state = -1;
    int   chosen = 0;
    char *base;
    for (i = 0; i < 34; i++, part_entry_cursor += 16) {
        char *part_moby = *(char **)part_entry_cursor;
        if (part_moby != 0 && FUN_L00_0025a420(part_moby, 0x10000, 0) != 0) {
            if (i < 7) {
                if (score <= 0) {
                    score = 1;
                    state = 1;
                }
            } else if (i < 10) {
                if (score < 2) {
                    score = 2;
                    state = 1;
                }
            } else {
                if (score < 4) {
                    score += 4;
                    chosen = i;
                    state = 2;
                }
            }
            (*(unsigned char **)part_entry_cursor)[0xA4] = 0xFF;
        }
    }
    *(short *)(data + 0x28C) += score;
    if (*(short *)(data + 0x28C) >= 8) {
        state = 0;
        *(short *)(data + 0x28C) = *(short *)(data + 0x28C) % 8;
        if (chosen != 0) {
            int   off = chosen * 16;
            char *entry;
            char *v;
            base = entries;
            entry = base + off;
            v = *(char **)(*(char **)entry + 0x78);
            cart_burst(moby, *(char **)entry + 0x10, -1, 2.0f, 13.0f);
            fast_vec_sub(v, *(char **)entry + 0x10, *(char **)(data + off + 0x64) + 0x10);
            fast_vec_normalize(v, v, D_0015ED6C * 10.0f);
            (*(char **)entry)[0x20] = 1;
            *(char **)entry = 0;
            return;
        } else {
            int start = truncate_float_to_s32(random_float_between(0.0f, 23.0f));
            for (i = 0; i < 24; i++) {
                int   off = ((i + start) % 24 + 10) * 16;
                char *entry;
                char *v;
                base = entries;
                entry = base + off;
                if (*(char **)entry != 0) {
                    v = *(char **)(*(char **)entry + 0x78);
                    cart_burst(moby, *(char **)entry + 0x10, -1, 2.0f, 13.0f);
                    fast_vec_sub(v, *(char **)entry + 0x10, *(char **)(data + off + 0x64) + 0x10);
                    fast_vec_normalize(v, v, D_0015ED6C * 10.0f);
                    (*(char **)entry)[0x20] = 1;
                    *(char **)entry = 0;
                    return;
                }
            }
            ((char *)moby)[0x20] = 99;
        }
    }
    if (state >= 0)
        allocate_voice_for_target_entry(state, 0, moby);
}

#include "rnc/overlay/swarm_path.h"

/* Gun turret on a gunship: cruises one path firing at its target, or circles a second path firing at random. */
typedef struct {
    SwarmPathFollower follow[2];    /* 0x00: [0] cruising (state 1), [1] circling (state 2) */
    PartLink          links[34];    /* 0x60: entry 0 is unused */
    s32               target_index; /* 0x280: moby slot of the target */
    f32               turn_speed;   /* 0x284 */
    s32               fire_timer;   /* 0x288 */
    s16               pad28C;
    s16               sound; /* 0x28E */
} GunshipVars;

extern s32 gunship_updates_disabled_gp __asm__("D_L08_00161C4C") __attribute__((sda));
extern f32 gunship_path_speed_gp __asm__("D_L08_00161C38") __attribute__((sda));
extern f32 gunship_turn_acceleration_degrees_gp __asm__("D_L08_00161C44") __attribute__((sda));
extern f32 gunship_max_turn_speed_degrees_gp __asm__("D_L08_00161C48") __attribute__((sda));
extern struct Moby *D_L08_0015FFD8_m __asm__("D_L08_0015FFD8");

extern f32  scale_time(f32) __asm__("FUN_001f96b0");
extern void FUN_L00_00258278(struct Moby *, f32, f32 *, f32, f32, f32);
extern struct Moby *
spawn_class_1b3_projectile(void *, struct Moby *, void *, f32, f32) __asm__("FUN_L08_002de3e0");
extern void FUN_L08_002e2250_u(struct Moby *) __asm__("FUN_L08_002e2250");

void update_gunship(struct Moby *moby) __asm__("FUN_L08_002e24e8");

void update_gunship(struct Moby *moby) {
    enum {
        GUNSHIP_INITIALIZE = 0,
        GUNSHIP_FOLLOW_PRIMARY_PATH_AND_FIRE_AT_TARGET = 1,
        GUNSHIP_FOLLOW_SECONDARY_PATH_AND_FIRE_RANDOMLY = 2,
        GUNSHIP_BEGIN_PRIMARY_PATH = 3,
        GUNSHIP_BREAK_APART_AND_REMOVE = 0x63
    };
    GunshipVars       *vars = (GunshipVars *)moby->pvars;
    SwarmPathFollower *follower;
    struct Moby       *target;
    struct Moby       *shot;
    struct Moby       *bolt;
    SwarmTargetVars   *target_vars;
    Vec4               point;
    Vec4               step;
    Vec4               muzzle;
    Vec4               velocity;
    s32                waypoint;
    s32                next;
    s32                after;
    s32                i;
    s32                slot;
    f32                distance;
    f32                heading;
    f32                next_heading;
    f32                turn;
    f32                pitch;
    f32                yaw;

    if (gunship_updates_disabled_gp != 0) {
        return;
    }
    if (!voice_is_playing(moby, vars->sound)) {
        vars->sound = allocate_voice_for_target_entry(3, 4, moby);
    }
    switch (moby->state) {
    case GUNSHIP_INITIALIZE:
        if (D_0014C050[moby->mission + current_level_index * 16] == 0xFF) {
            mark_moby_for_removal(moby);
            return;
        }
        vars->follow[0].path = l08_paths[vars->follow[0].path_id];
        vars->follow[1].path = l08_paths[vars->follow[1].path_id];
        vars->fire_timer = truncate_float_to_s32(scale_time(random_float_between(300.0f, 600.0f)));
        qcopy(&moby->pos, &vars->follow[1].path->points[0]);
        spawn_gunship_parts(moby);
        moby->state = 2;
        moby->rot.z = fast_arc_tan(vars->follow[1].path->points[1].f[0] - moby->pos.x,
                                   vars->follow[1].path->points[1].f[1] - moby->pos.y);
        moby->flags |= 0x4000;
        break;
    case GUNSHIP_FOLLOW_PRIMARY_PATH_AND_FIRE_AT_TARGET:
        waypoint =
            (vars->follow[0].point + vars->follow[0].path->count + vars->follow[0].direction) %
            vars->follow[0].path->count;
        target = &D_L08_0015FFD8_m[vars->target_index];
        FUN_L00_00258278(moby,
                         fast_arc_tan(vars->follow[0].path->points[waypoint].f[0] - moby->pos.x,
                                      vars->follow[0].path->points[waypoint].f[1] - moby->pos.y),
                         &vars->turn_speed,
                         0.004f,
                         0.3f,
                         0.02f);
        FUN_L08_002e2250_u(moby);
        update_linked_moby_parts(moby);
        qcopy(&point, &vars->follow[0].path->points[waypoint]);
        if (fast_vec_dist(&moby->pos, &point) < 0.5f) {
            vars->follow[0].point = waypoint;
        }
        fast_vec_sub(&step, &point, &moby->pos);
        if (fast_vec_length(&step) > gunship_path_speed_gp * frame_time) {
            fast_vec_normalize(&step, &step, gunship_path_speed_gp * frame_time);
        }
        fast_vec_add(&moby->pos, &moby->pos, &step);
        if (moby->command == 0) {
            break;
        }
        moby->command = 0;
        if (random_integer_below(0xFF) & 1) {
            moby_point_position(vars->links[3].part, 1, &muzzle);
        } else {
            moby_point_position(vars->links[4].part, 1, &muzzle);
        }
        pitch = random_float_between(0.17453292f, 0.5235988f);
        yaw = fast_add_rotations(
            fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y), 3.1415927f);
        yaw = fast_add_rotations(yaw, random_float_between(-0.2617994f, 0.7853982f));
        if (yaw > 0.0f && yaw < 0.34f) {
            yaw = 0.34f;
        } else if (yaw <= 0.0f && yaw > -0.34f) {
            yaw = -0.34f;
        }
        fast_vec_normalize(
            &velocity, &step, (gunship_path_speed_gp + gunship_path_speed_gp) * frame_time);
        shot = spawn_class_1b3_projectile(&muzzle, target, &velocity, pitch, yaw);
        shot->scale = shot->pclass->scale / 5.0f;
        target_vars = (SwarmTargetVars *)target->pvars;
        for (slot = 0; slot < 8; slot++) {
            if (target_vars->shots[slot] == NULL) {
                target_vars->shots[slot] = shot;
                break;
            }
        }
        break;
    case GUNSHIP_FOLLOW_SECONDARY_PATH_AND_FIRE_RANDOMLY:
        follower = &vars->follow[1];
        next = (follower->point + 1) % follower->path->count;
        after = (follower->point + 2) % follower->path->count;
        heading = fast_arc_tan(
            follower->path->points[next].f[0] - follower->path->points[follower->point].f[0],
            follower->path->points[next].f[1] - follower->path->points[follower->point].f[1]);
        next_heading =
            fast_arc_tan(follower->path->points[after].f[0] - follower->path->points[next].f[0],
                         follower->path->points[after].f[1] - follower->path->points[next].f[1]);
        turn = fast_subtract_rotations(heading, next_heading);
        distance = fast_vec_dist(&moby->pos, &follower->path->points[next]);
        distance /=
            fast_vec_dist(&follower->path->points[follower->point], &follower->path->points[next]);
        /* the blended heading is computed but unused: the turn below steers towards the next point */
        fast_add_rotations(turn * distance, next_heading);
        heading = fast_arc_tan(follower->path->points[next].f[0] - moby->pos.x,
                               follower->path->points[next].f[1] - moby->pos.y);
        approach_angle(&moby->rot.z,
                       &vars->turn_speed,
                       heading,
                       gunship_turn_acceleration_degrees_gp * DEG_TO_RAD * frame_time_sq,
                       gunship_turn_acceleration_degrees_gp * DEG_TO_RAD * frame_time_sq,
                       gunship_max_turn_speed_degrees_gp * DEG_TO_RAD * frame_time);
        qcopy(&point, &follower->path->points[next]);
        if (fast_vec_dist(&moby->pos, &point) < 0.5f) {
            follower->point = next;
        }
        fast_vec_sub(&step, &point, &moby->pos);
        if (fast_vec_length(&step) > gunship_path_speed_gp * frame_time) {
            fast_vec_normalize(&step, &step, gunship_path_speed_gp * frame_time);
        }
        fast_vec_add(&moby->pos, &moby->pos, &step);
        update_linked_moby_parts(moby);
        if (!fast_dec_timer(&vars->fire_timer)) {
            break;
        }
        vars->fire_timer = truncate_float_to_s32(scale_time(random_float_between(300.0f, 600.0f)));
        moby_point_position(vars->links[4].part, 1, &muzzle);
        pitch = random_float_between(-0.5235988f, -0.2617994f);
        yaw = random_angle_radians();
        fast_vec_normalize(
            &velocity, &step, (gunship_path_speed_gp + gunship_path_speed_gp) * frame_time);
        bolt = spawn_class_1b3_projectile(&muzzle, NULL, &velocity, pitch, yaw);
        if (bolt != NULL) {
            bolt->rot.z = vars->links[4].part->rot.z;
            bolt->state = 5;
            bolt->rot.y = 0.0f;
        }
        break;
    case GUNSHIP_BEGIN_PRIMARY_PATH: {
        LevelPath *path;

        moby->state = 1;
        path = vars->follow[0].path;
        qcopy(&moby->pos, &path->points[0]);
        moby->rot.z =
            fast_arc_tan(path->points[1].f[0] - moby->pos.x, path->points[1].f[1] - moby->pos.y);
        moby->rot.x = 0.0f;
        update_linked_moby_parts(moby);
        break;
    }
    case GUNSHIP_BREAK_APART_AND_REMOVE:
        for (i = 1; i < 34; i++) {
            struct Moby *part = vars->links[i].part;
            if (part != NULL) {
                Vec4 *drift = (Vec4 *)part->pvars;
                cart_burst(moby, &part->pos, -1, 3.0f, 13.0f);
                fast_vec_sub(drift, &part->pos, &moby->pos);
                fast_vec_normalize(drift, drift, frame_time * 5.0f);
                part->state = 1;
                vars->links[i].part = NULL;
            }
        }
        cart_burst(moby, &moby->pos, -1, 5.0f, 13.0f);
        mark_moby_for_removal(moby);
        break;
    }
}

#include "sda.h"

/* updates matching objects in the selected object range */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002E0258.c: func_L08_002E4118), where it is exact; names translated to the US level program. */

void build_bolt_thief_target_list(struct Moby *moby) __asm__("FUN_L08_002e2da0");

void build_bolt_thief_target_list(struct Moby *moby) {
    char  *data = (char *)moby->pvars;
    int    oclass = 0x279;
    char  *base = D_L08_0015FFD8;
    int   *ids = (int *)(data + 0x130);
    char **output = (char **)(data + 0x158);
    int    i = 9;
    for (;;) {
        char *other = (char *)((*ids << 8) + (unsigned int)base);
        if (*(short *)(other + 0xA6) == oclass) {
            *output++ = other;
        }
        --i;
        if (i < 0)
            return;
        ++ids;
    }
}

extern float D_0013F3D4;
extern float bolt_thief_waypoint_speed_gp __asm__("D_L08_00161C50") __attribute__((sda));
extern char  D_001413D0[];
extern void  FUN_L00_00263ac8(float, struct Moby *, int, void *);
extern void  FUN_L00_0025a120(struct Moby *);
extern void
trooper_start_knockback(struct Moby *, void *, float, int, int, int) __asm__("FUN_L00_0025c558");
extern void FUN_L00_00257470(struct Moby *, int, int);
extern void trooper_anim_finish(struct Moby *, void *) __asm__("FUN_L00_0025d538");
extern void FUN_L00_0025f730(void *, float);
extern int  find_listed_moby_by_data_id(struct Moby *) __asm__("FUN_L00_0025f878");
extern void
begin_bolt_attraction(struct Moby *, struct Moby *, float, float) __asm__("FUN_L00_002a7210");
extern void FUN_00212f90(struct Moby *, int, int, int);

typedef struct {
    char              pad0[0x29];
    unsigned char     b29;
    char              pad2A[0x16];
    Vec4              v40;
    char              pad50[0x10];
    char              x60[7];
    unsigned char     b67;
    char              pad68[8];
    char              x70[0x10];
    float             f80, f84, f88, f8C;
    int               w90, w94;
    char              pad98[0x15];
    unsigned char     bAD;
    char              padAE[0x22];
    SwarmPathFollower follow[2];
    char              pad130[0x28];
    struct Moby      *list[0x32];
    int               w220;
    float             f224;
    char              pad228[4];
    int               w22C;
    char              pad230[0x30];
    unsigned char     x260[1];
} BoltThiefVars;

void bolt_thief_update(struct Moby *moby) __asm__("FUN_L08_002e2df0");

void bolt_thief_update(struct Moby *moby) {
    enum {
        BOLT_THIEF_INITIALIZE = 0,
        BOLT_THIEF_WAIT_FOR_HERO_NEAR_TRIGGER = 1,
        BOLT_THIEF_FOLLOW_PRIMARY_PATH_TO_COLLECTION_POINT = 2,
        BOLT_THIEF_WAIT_FOR_ANIMATION_BEFORE_PULLING_PICKUPS = 3,
        BOLT_THIEF_PULL_PICKUPS_TO_JOINT = 4,
        BOLT_THIEF_FOLLOW_PRIMARY_PATH_TO_EXIT = 5,
        BOLT_THIEF_FOLLOW_SECONDARY_PATH = 6,
        BOLT_THIEF_FACE_HERO = 7,
        BOLT_THIEF_KNOCKBACK = 8
    };
    Vec4           point;
    Vec4           step;
    BoltThiefVars *data;

    data = moby->pvars;
    FUN_L00_00263ac8(2.5f, moby, 1, data->x260);
    if (moby->visible != 0) {
        if (fast_vec_dist(&moby->pos, D_L08_001675C0) < 29.0f) {
            FUN_L00_0025a120(moby);
            moby->shadow_range = 0x17;
        }
    }
    if (FUN_L00_0025a420(moby, 0x210000, 0) != 0 && moby->state != 8) {
        int i;

        for (i = 0; i < 0x32; i++) {
            struct Moby *other = data->list[i];

            if (other != 0 && other->state == 3) {
                other->state = 1;
            }
        }
        {
            float speed8 = D_0015ED6C * 8.0f;
            float speed10 = D_0015ED6C * 10.0f;
            data->f80 = 0.008f;
            data->f84 = 0.0005f;
            data->w94 = 1;
            data->f88 = speed8;
            data->f8C = speed10;
            data->bAD = 0;
        }
        trooper_start_knockback(
            moby,
            data->x70,
            fast_arc_tan(moby->pos.x - hero.motion.pos.f[0], moby->pos.y - hero.motion.pos.f[1]),
            4,
            1,
            0);
        data->b67 = 0x78;
        FUN_L00_00257470(moby, 0, -1);
        moby->state = 8;
    }
    moby->hit_slot = 0xFF;
    switch (moby->state) {
    case BOLT_THIEF_INITIALIZE:
        data->follow[0].path = l08_paths[data->follow[0].path_id];
        qcopy(&moby->pos, &data->follow[0].path->points[0]);
        data->follow[0].point = 1;
        if (data->follow[1].path_id >= 0) {
            data->follow[1].path = l08_paths[data->follow[1].path_id];
            data->follow[1].point = 1;
        }
        moby->flags |= 0x1000;
        data->b29 = 0;
        moby->spawn_frame = *(u64 *)(*(char **)D_001413D0 + 0x38);
        clear_u64_value(&data->v40);
        moby->state = 1;
        if (moby->prev_seq != 1) {
            FUN_00212f90(moby, 1, 0, 0);
        }
        break;
    case BOLT_THIEF_WAIT_FOR_HERO_NEAR_TRIGGER:
        qcopy(&point, &l08_paths[data->w220]->points[0]);
        if (fast_vec_dist(hero.motion.pos.f, &point) < 24.0f) {
            build_bolt_thief_target_list((char *)moby);
            moby->state = 5;
            if (moby->prev_seq != 3) {
                FUN_00212f90(moby, 3, 0, 6);
            }
        }
        break;
    case BOLT_THIEF_FOLLOW_PRIMARY_PATH_TO_COLLECTION_POINT: {
        SwarmPathFollower *follower;
        LevelPath         *path;
        int                next;
        float              heading;

        follower = &data->follow[0];
        next =
            (follower->point + follower->path->count + follower->direction) % follower->path->count;
        heading = fast_arc_tan(follower->path->points[next].f[0] - moby->pos.x,
                               follower->path->points[next].f[1] - moby->pos.y);
        FUN_L00_00258278(moby, heading, &data->f224, 0.01f, 0.3f, 0.1f);
        path = follower->path;
        qcopy(&point, &path->points[next]);
        if (path->points[next].f[3] == 0.0f) {
            moby->state = 3;
            if (moby->prev_seq != 2) {
                FUN_00212f90(moby, 2, 0, 6);
            }
        } else {
            fast_vec_sub(&step, &point, &moby->pos);
            fast_vec_normalize(&step, &step, bolt_thief_waypoint_speed_gp * D_0015ED6C);
            fast_vec_add(&moby->pos, &moby->pos, &step);
            if (fast_vec_dist(&moby->pos, &point) < 0.5f) {
                follower->point = next;
            }
        }
        break;
    }
    case BOLT_THIEF_WAIT_FOR_ANIMATION_BEFORE_PULLING_PICKUPS:
        moby->anim_speed = 3.0f;
        if (moby->anim_flags & 2) {
            int i;

            build_bolt_thief_target_list((char *)moby);
            for (i = 0; i < 0x32; i++) {
                struct Moby *other = data->list[i];

                if (other != 0 && (signed char)other->state >= 0) {
                    other->state = 3;
                }
            }
            moby->state = 4;
            if (moby->prev_seq != 6) {
                FUN_00212f90(moby, 6, 0, 6);
            }
            moby->anim_speed = 1.0f;
        }
        break;
    case BOLT_THIEF_PULL_PICKUPS_TO_JOINT: {
        int active_count;
        int i;

        active_count = 0;
        for (i = 0; i < 0x32; i++) {
            struct Moby *other = data->list[i];

            if (other != 0 && (signed char)other->state >= 0) {
                moby_point_position(moby, 0, &step);
                active_count++;
                fast_vec_sub(&point, &step, &other->pos);
                FUN_L00_0025f730(&point, D_0015ED6C * 4.0f);
                fast_vec_add(&other->pos, &other->pos, &point);
                if (fast_vec_length(&point) < 2.0f * D_0015ED6C) {
                    other->state = 4;
                    data->list[i] = 0;
                    data->w22C++;
                }
            }
        }
        if (active_count == 0) {
            moby->state = 5;
            if (moby->prev_seq != 3) {
                FUN_00212f90(moby, 3, 0, 6);
            }
        }
        break;
    }
    case BOLT_THIEF_FOLLOW_PRIMARY_PATH_TO_EXIT: {
        SwarmPathFollower *follower;
        int                next;
        int                found;
        int                i;
        float              heading;

        follower = &data->follow[0];
        found = 0;
        next =
            (follower->point + follower->path->count + follower->direction) % follower->path->count;
        heading = fast_arc_tan(follower->path->points[next].f[0] - moby->pos.x,
                               follower->path->points[next].f[1] - moby->pos.y);
        FUN_L00_00258278(moby, heading, &data->f224, 0.01f, 0.3f, 0.1f);
        qcopy(&point, &follower->path->points[next]);
        for (i = 0; i < 0x32; i++) {
            struct Moby *other = data->list[i];

            if (other != 0 && (signed char)other->state >= 0) {
                if (vector_distance_xy(&moby->pos, &other->pos) < 6.0f) {
                    if (moby->prev_seq != 6) {
                        FUN_00212f90(moby, 6, 0, scale_game_frames(0x14));
                    }
                    begin_bolt_attraction(data->list[i], moby, 0.0f, 0.0f);
                    data->list[i] = 0;
                }
                found = 1;
            }
        }
        if (found == 0 && moby->prev_seq != 3) {
            FUN_00212f90(moby, 3, 0, scale_game_frames(0x14));
        }
        if (follower->path->count - 1 == next) {
            if (data->follow[1].path_id >= 0) {
                moby->state = 6;
                break;
            }
            mark_moby_for_removal(moby);
            return;
        }
        fast_vec_sub(&step, &point, &moby->pos);
        fast_vec_normalize(&step, &step, bolt_thief_waypoint_speed_gp * D_0015ED6C);
        fast_vec_add(&moby->pos, &moby->pos, &step);
        if (fast_vec_dist(&moby->pos, &point) < 0.5f) {
            follower->point = next;
        }
        break;
    }
    case BOLT_THIEF_FOLLOW_SECONDARY_PATH: {
        SwarmPathFollower *follower;
        int                next;
        float              heading;

        follower = &data->follow[1];
        next =
            (follower->point + follower->path->count + follower->direction) % follower->path->count;
        heading = fast_arc_tan(follower->path->points[next].f[0] - moby->pos.x,
                               follower->path->points[next].f[1] - moby->pos.y);
        FUN_L00_00258278(moby, heading, &data->f224, 0.01f, 0.3f, 0.1f);
        qcopy(&point, &follower->path->points[next]);
        fast_vec_sub(&step, &point, &moby->pos);
        fast_vec_normalize(&step, &step, bolt_thief_waypoint_speed_gp * D_0015ED6C);
        fast_vec_add(&moby->pos, &moby->pos, &step);
        if (fast_vec_dist(&moby->pos, &point) < 0.5f) {
            follower->point = next;
        }
        if (fast_vec_dist(hero.motion.pos.f, &moby->pos) > 19.0f) {
            moby->state = 7;
        }
        break;
    }
    case BOLT_THIEF_FACE_HERO: {
        float *hero_position;
        float  distance;
        float  heading;

        heading =
            fast_arc_tan(hero.motion.pos.f[0] - moby->pos.x, hero.motion.pos.f[1] - moby->pos.y);
        hero_position = hero.motion.pos.f;
        FUN_L00_00258278(moby, heading, &data->f224, 0.01f, 0.3f, 0.1f);
        distance = fast_vec_dist(hero_position, &moby->pos);
        if (distance < 17.0f || find_listed_moby_by_data_id(moby) != 0) {
            SwarmPathFollower *follower;
            int                next;

            follower = &data->follow[1];
            next = (follower->point + follower->path->count + follower->direction) %
                   follower->path->count;
            moby->state = 6;
            if (fast_vec_dist(hero_position, &follower->path->points[next]) < distance) {
                follower->direction *= -1;
            }
        }
        break;
    }
    case BOLT_THIEF_KNOCKBACK:
        if (mine_fall(moby, data->x70) & 3) {
            cart_burst(moby, &moby->pos, -1, 1.0f, 13.0f);
            mark_moby_for_removal(moby);
            return;
        }
        if (moby->pos.z < 5.0f) {
            mark_moby_for_removal(moby);
            return;
        }
        break;
    }
    trooper_anim_finish(moby, data->x60);
}

#include "rnc/overlay/hud.h"

/* Batalia turret bolt: rides its turret, flies to the hero when shot and is collected, falls and bounces when the turret dies. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002E0258.c: func_L08_002E4BE8), where it is exact; names translated to the US level program. */

typedef struct Hero Hero;

typedef int u128_4BE8_p __attribute__((mode(TI)));

typedef struct {
    float anchor[4];              /* 0x00 */
    float v10[4];                 /* 0x10 */
    float v20[4];                 /* 0x20 */
    float v30[4];                 /* 0x30 */
    float velocity[4];            /* 0x40 */
    float spin_x, spin_y, spin_z; /* 0x50 */
    int   pad5C;
    float f60, f64; /* 0x60 */
    int   timer;    /* 0x68 */
    short s6C, s6E; /* 0x6C */
    union {
        int   i;
        float f;
    } u70;
    float f74; /* 0x74 */
    union {
        int   i;
        char *p;
    } u78;
    union {
        int   i;
        float f;
    } u7C;
} Vars_4BE8;

typedef struct {
    char  pad0[0x18];
    int   moby; /* 0x18 */
    char  pad1C[4];
    float pos[4]; /* 0x20 */
    char  pad30[0x10];
    float n[4]; /* 0x40 */
} Coll_4BE8;

typedef struct {
    u8 pad_00_p[0xB1];
    u8 id;
} BoltSource_4BE8;

typedef struct {
    unsigned char pad[0x454];
    unsigned char killed[1];
} KillFlags_4BE8;

typedef struct {
    float pos[4]; /* 0x00 */
    char  pad10[0x10];
    float n[4]; /* 0x20 */
} CollPos_4BE8;

extern float          D_L08_00174660[];
extern KillFlags_4BE8 l08_bolt_pickup_collection_marks_by_save_id __asm__("D_L08_001BADD0");
extern KillFlags_4BE8
    l08_bolt_pickup_mission_gated_collection_marks_by_save_id __asm__("D_L08_001BBA30");
extern struct Moby *D_L08_0015FFDC;
extern f32          fast_vec_dist(void *, void *);
extern float l08_bolt_pickup_collection_target_speed __asm__("D_L08_00161C88") __attribute__((sda));
extern float l08_bolt_pickup_collection_acceleration __asm__("D_L08_00161C8C") __attribute__((sda));
extern float l08_bolt_pickup_collection_launch_spin_speed_degrees __asm__("D_L08_00161C94")
    __attribute__((sda));
extern float l08_bolt_pickup_detached_fall_spin_speed_degrees __asm__("D_L08_00161C98")
    __attribute__((sda));
extern float l08_bolt_pickup_default_surface_roll_speed_degrees __asm__("D_L08_00161CA0")
    __attribute__((sda));
extern float l08_five_bolt_pickup_surface_orbit_speed_degrees __asm__("D_L08_00161CA4")
    __attribute__((sda));
extern float l08_ten_bolt_pickup_surface_orbit_speed_degrees __asm__("D_L08_00161CAC")
    __attribute__((sda));
extern float D_L08_00161CB4_gp __asm__("D_L08_00161CB4") __attribute__((sda));
extern float D_L08_00161CB8;
extern float D_L08_00161CBC_gp __asm__("D_L08_00161CBC") __attribute__((sda));
extern float l08_bolt_pickup_surface_return_base_duration __asm__("D_L08_00161CD0")
    __attribute__((sda));
extern float FUN_L04_001f1978(float);
extern float step_angle_toward_target(float *, float, float, int) __asm__("FUN_L00_0025bc00");
extern int   l08_bolt_pickup_collection_scale_fade_enabled __asm__("D_L08_00161CC4")
    __attribute__((sda));
extern int l08_bolt_pickup_initial_rotating_quad_color __asm__("D_L08_00161CCC")
    __attribute__((sda));
extern int l08_bolt_pickup_last_collection_sound_frame __asm__("D_L08_00161CD4")
    __attribute__((sda));
extern int D_L08_00174658;
extern int collision_line(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern int coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern int
transform_attachment_pose(void *, int, void *, void *, void *, void *) __asm__("FUN_L00_002604f0");
extern int
queue_animation_update(s32, s32, void *, void *, void *, void *, s32) __asm__("FUN_001ff308");
extern s32            level_bolts[] __asm__("D_0013DF38");
extern unsigned char *FUN_L00_002603d0(int);
extern unsigned char *FUN_L00_00272060(
    float *pos, int a, float f0, float f1, int col, int mode, int b, float *vel, float f2);
extern unsigned char D_0014BEC0[][4];
extern unsigned char D_L08_0015FC88_macro[] __asm__("D_L08_0015FC88") MACRO_ADDR;
extern void          FUN_L00_0025f730_c(void *a, float x) __asm__("FUN_L00_0025f730");
extern void convert_pose_to_reference_local(void *, int, void *, void *, void *, void *) __asm__("FUN_L00_00260668");
extern void build_axis_roll_matrix(void *, void *, float) __asm__("FUN_L08_0026a0f8");
extern void FUN_L08_002e4e90_c(char *m, float *out, float *rot) __asm__("FUN_L08_002e4e90");
extern void FUN_L08_002e5188_c(char *m) __asm__("FUN_L08_002e5188");
int         draw_bolt_counter_hud_panel(HudElem *m) __asm__("FUN_L00_00239df8");
void        initialize_bolt_counter_hud(HudElem *e) __asm__("FUN_L00_00239cc8");
void        update_bolt_counter_hud_animation_and_visibility(void *o) __asm__("FUN_L00_00239d00");
extern char D_0013F3D0[];
extern char D_0013F490[];
extern void func_L00_0023A658_4BE8(void) __asm__("FUN_L08_002444a8");
extern void func_L00_0023A690_4BE8(void) __asm__("FUN_L08_002444e0");
extern void func_L00_0023A788_4BE8(void) __asm__("FUN_L08_002445d8");
extern int  D_L08_00174658_lui __asm__("D_L08_00174658") NOT_SDA;

void update_batalia_bolt_pickup(struct Moby *moby) __asm__("FUN_L08_002e3870");

void update_batalia_bolt_pickup(struct Moby *moby) {
    enum {
        BOLT_PICKUP_INITIALIZE = 0,
        BOLT_PICKUP_WAIT_FOR_HERO = 1,
        BOLT_PICKUP_TURN_TOWARD_HERO = 2,
        BOLT_PICKUP_FLY_TO_HERO_AND_CREDIT_BOLTS = 3,
        BOLT_PICKUP_FALL_AND_BOUNCE = 4,
        BOLT_PICKUP_RETURN_TO_SURFACE = 5
    };

    char *collection_motion_reference;
    float direction[4];
    float point[4];
    float local[4];
    float scratch[16];

    switch (moby->state) {
    case BOLT_PICKUP_INITIALIZE: {
        Vars_4BE8 *vars = (Vars_4BE8 *)moby->pvars;
        int        linked;

        switch (moby->oclass) {
        case MOBY_CLASS_ONE_BOLT_PICKUP:
            moby->scale = moby->pclass->scale * D_L08_00161CB4_gp;
            break;
        case MOBY_CLASS_FIVE_BOLT_PICKUP:
            moby->scale = moby->pclass->scale * D_L08_00161CB8;
            break;
        case MOBY_CLASS_TEN_BOLT_PICKUP:
            moby->scale = moby->pclass->scale * D_L08_00161CBC_gp;
            break;
        }
        linked = 0;
        if (l08_bolt_pickup_last_collection_sound_frame > D_L08_0015F5CC) {
            l08_bolt_pickup_last_collection_sound_frame = D_L08_0015F5CC;
        }
        moby->flags &= ~0x100;
        qcopy(vars->anchor, &moby->pos);
        if (vars->s6E == 0) {
            fast_vec_normalize(point, &moby->rot_row2, 0.1f);
            fast_vec_add(point, point, &moby->pos);
            fast_vec_scale(local, &moby->rot_row2, -2.0f);
            fast_vec_add(local, local, &moby->pos);
            if (collision_line(point, local, 2, 0, 0) != 0) {
                CollPos_4BE8 *hit = (CollPos_4BE8 *)D_L08_00174660;
                qcopy(vars->anchor, hit->pos);
                linked = ((int *)hit)[-2];
                fast_vec_normalize(direction, hit->n, 1.0f);
            } else {
                qcopy(vars->anchor, &moby->pos);
                clear_u64_value(direction);
                direction[2] = 1.0f;
            }
        }
        set_batalia_bolt_pickup_surface_rotation((char *)moby, direction);
        vars->f60 = random_angle_radians();
        vars->f64 = moby->rot.x;
        FUN_L08_002e4e90_c((char *)moby, (float *)&moby->pos, (float *)&moby->rot);
        if (linked != 0 && FUN_L00_002603d0(linked) != 0) {
            vars->u7C.i = linked;
            convert_pose_to_reference_local(
                moby, linked, vars->anchor, vars->v10, vars->v20, vars->v30);
        }
        moby->state = 1;
        vars->s6C = 0;
        vars->u70.i =
            truncate_float_to_s32(random_float_between(0.0f, (float)scale_game_frames(600)));
        vars->u78.i = l08_bolt_pickup_initial_rotating_quad_color;
        break;
    }
    case BOLT_PICKUP_WAIT_FOR_HERO: {
        Vars_4BE8 *vars = (Vars_4BE8 *)moby->pvars;

        if (vars->u7C.i != 0) {
            transform_attachment_pose(
                moby, vars->u7C.i, vars->v20, vars->v30, vars->anchor, vars->v10);
            if ((signed char)((struct Moby *)vars->u7C.i)->state < 0) {
                vars = (Vars_4BE8 *)moby->pvars;
                clear_u64_value(vars->velocity);
                build_axis_roll_matrix(vars,
                                       &moby->rot_row1,
                                       l08_bolt_pickup_detached_fall_spin_speed_degrees *
                                           0.017453292f * frame_time);
                moby->state = 4;
                break;
            }
        }
        if (absolute_float(moby->pos.z - hero.motion.pos.f[2]) < hero.reach_distance &&
            vector_distance_xy(&moby->pos, hero.motion.pos.f) < hero.reach_height) {
            vars->timer = scale_game_frames(1);
            moby->state = 2;
        }
        if (moby->visible == 0) {
            if (moby > D_L08_0015FFDC &&
                vector_distance_xy(&moby->pos, ((char *)&D_0013F3D0)) > 24.0f) {
                mark_moby_for_removal(moby);
                return;
            }
            break;
        }
        switch (moby->oclass) {
        case MOBY_CLASS_TEN_BOLT_PICKUP:
            vars->f60 = fast_add_rotations(vars->f60,
                                           l08_ten_bolt_pickup_surface_orbit_speed_degrees *
                                               0.017453292f * frame_time);
            vars->f64 = fast_add_rotations(
                vars->f64, ten_bolt_pickup_surface_roll_speed_degrees * 0.017453292f * frame_time);
            break;
        case MOBY_CLASS_FIVE_BOLT_PICKUP:
            vars->f60 = fast_add_rotations(vars->f60,
                                           l08_five_bolt_pickup_surface_orbit_speed_degrees *
                                               0.017453292f * frame_time);
            vars->f64 = fast_add_rotations(
                vars->f64, five_bolt_pickup_surface_roll_speed_degrees * 0.017453292f * frame_time);
            break;
        default:
            vars->f60 = fast_add_rotations(vars->f60,
                                           bolt_pickup_default_surface_orbit_speed_degrees *
                                               0.017453292f * frame_time);
            vars->f64 = fast_add_rotations(vars->f64,
                                           l08_bolt_pickup_default_surface_roll_speed_degrees *
                                               0.017453292f * frame_time);
            break;
        }
        FUN_L08_002e4e90_c((char *)moby, (float *)&moby->pos, (float *)&moby->rot);
        if (vars->s6C != 0) {
            vars->f74 = fast_add_rotations(vars->f74, frame_time * 5.2359877f);
            if (vars->u70.i > 12) {
                vars->u78.i += 0x5000000;
            } else {
                vars->u78.i -= 0x5000000;
            }
            if (fast_dec_timer(&vars->u70.i) != 0) {
                vars->s6C = 0;
                vars->u70.i = truncate_float_to_s32(random_float_between(
                    (float)scale_game_frames(300), (float)scale_game_frames(600)));
            } else if (moby->visible != 0 && bolt_pickup_rotating_quad_enabled != 0) {
                add_draw_callback(FUN_L08_002e5188_c, moby);
                return;
            }
        } else if (fast_dec_timer(&vars->u70.i) != 0) {
            vars->u70.i = 24;
            vars->f74 = random_angle_radians();
            vars->s6C = 1;
        }
        break;
    }
    case BOLT_PICKUP_TURN_TOWARD_HERO: {
        Vars_4BE8 *vars = (Vars_4BE8 *)moby->pvars;

        euler_rotation_matrix(scratch, vars->v10);
        qcopy(direction, scratch + 8);
        if (fast_dec_timer(&vars->timer) != 0) {
            Vars_4BE8 *vars = (Vars_4BE8 *)moby->pvars;
            float      deg_to_rad = 0.017453292f;
            char      *hero_pos = D_0013F3D0;

            fast_vec_sub(point, &moby->pos, hero_pos);
            fast_vec_normalize(point, point, 1.0f);
            fast_vec_cross(point, direction, point);
            build_axis_roll_matrix(vars,
                                   point,
                                   l08_bolt_pickup_collection_launch_spin_speed_degrees *
                                       deg_to_rad * frame_time);
            moby->flags |= 0x100;
            fast_vec_normalize(vars->velocity, direction, frame_time * 8.0f);
            vars->f74 = 0.0f;
            vars->u70.f = frame_time * 8.0f;
            vars->u78.p = *(char **)(hero_pos + 0x2000);
            vars->u7C.f = 0.5f;
            if (l08_bolt_pickup_last_collection_sound_frame + scale_game_frames(3) <
                D_L08_0015F5CC) {
                l08_bolt_pickup_last_collection_sound_frame = D_L08_0015F5CC;
                allocate_voice_for_target_entry(0, 0x20, moby);
                vars->f64 = 0.0f;
            } else {
                vars->f64 = random_float_between(-30.0f, 30.0f) * deg_to_rad;
            }
            moby->state = 3;
        } else {
            if (scale_game_frames(10) < vars->timer) {
                float goal_yaw = fast_add_rotations(
                    vars->f60, fast_sin((float)(vars->timer % 8 - 4) * 0.7854f) * 0.17453292f);

                step_angle_toward_target(&vars->f60, goal_yaw, 0.5235988f, 0);
            } else {
                float goal_yaw =
                    fast_add_rotations(fast_arc_tan(hero.motion.pos.f[0] - moby->pos.x,
                                                    hero.motion.pos.f[1] - moby->pos.y),
                                       1.5707964f);

                step_angle_toward_target(&vars->f60, goal_yaw, 0.08726646f, 0);
                if (fast_difference_between_rotations(vars->f60, goal_yaw) < 0.08726646f) {
                    vars->timer = 0;
                }
            }
            FUN_L08_002e4e90_c((char *)moby, (float *)&moby->pos, (float *)&moby->rot);
        }
        break;
    }
    case BOLT_PICKUP_FLY_TO_HERO_AND_CREDIT_BOLTS: {
        Vars_4BE8 *vars = (Vars_4BE8 *)moby->pvars;
        float      collection_target_distance;
        float      fade_range;

        if (vars->u78.p == 0) {
            break;
        }
        fast_vec_normalize(point, vars->u78.p + 0xE0, vars->u7C.f);
        fast_vec_add(point, point, vars->u78.p + 0x10);
        fast_vec_sub(direction, point, &moby->pos);
        if (vars->f64 != 0.0f) {
            float sine = fast_sin(vars->f64);
            float cosine = fast_cos(vars->f64);

            direction[0] = cosine * direction[0] + sine * direction[1];
            direction[1] = cosine * direction[1] - sine * direction[0];
        }
        collection_target_distance = fast_vec_dist(point, &moby->pos);
        fast_vec_normalize(vars->velocity, vars->velocity, vars->u70.f);
        fast_vec_add(&moby->pos, &moby->pos, vars->velocity);
        fast_vec_normalize(direction, direction, vars->f74);
        fast_vec_add(&moby->pos, &moby->pos, direction);
        collection_motion_reference = D_0013F490;
        fast_vec_add(&moby->pos, &moby->pos, collection_motion_reference);
        fast_vec_sub(local, &moby->pos, collection_motion_reference + 0x160);
        transform_vector(local, local, collection_motion_reference - 0x100);
        if (((struct Moby *)vars->u78.p)->oclass != 0 || local[2] > 0.0f) {
            func_00214D28_2E16A0(
                &vars->u70.f, 0.0f, l08_bolt_pickup_collection_acceleration * frame_time_sq);
            func_00214D28_2E16A0(&vars->f74,
                                 l08_bolt_pickup_collection_target_speed * frame_time,
                                 l08_bolt_pickup_collection_acceleration * frame_time_sq);
        } else {
            fast_vec_normalize(scratch, collection_motion_reference - 0x120, vars->u70.f);
            fast_vec_sub(scratch + 4, scratch, vars->velocity);
            FUN_L00_0025f730_c(scratch + 4,
                               l08_bolt_pickup_collection_acceleration * frame_time_sq);
            fast_vec_add(vars->velocity, vars->velocity, scratch + 4);
        }
        if (collection_target_distance < vars->f74) {
            vars->f74 = collection_target_distance;
        }
        fade_range = vars->f74 * (float)scale_game_frames(20);
        if (collection_target_distance < fade_range) {
            if (fast_vec_dist(point, &moby->pos) > 0.52f) {
                if (bolt_pickup_collection_alpha_fade_enabled != 0) {
                    moby->alpha = (fast_vec_dist(point, &moby->pos) + fade_range) * 128.0f /
                                  (fade_range + fade_range);
                }
                if (l08_bolt_pickup_collection_scale_fade_enabled != 0) {
                    moby->scale = moby->pclass->scale * D_L08_00161CB4_gp *
                                  (fast_vec_dist(point, &moby->pos) + fade_range) /
                                  (fade_range + fade_range);
                }
            } else {
                int              value;
                int              spin;
                float            shrink;
                float            size;
                BoltSource_4BE8 *source;

                switch (moby->oclass) {
                case MOBY_CLASS_COLLECTED_COUNT_BOLT_BONUS_PICKUP: {
                    int i;
                    int j;
                    int n;

                    D_0014BEC0[current_level_index][moby->command] = 1;
                    n = 0;
                    for (i = 0; i < 20; i++) {
                        for (j = 0; j < 4; j++) {
                            if (D_0014BEC0[i][j] != 0) {
                                n++;
                            }
                        }
                    }
                    value = n * 100;
                    break;
                }
                case MOBY_CLASS_TEN_BOLT_PICKUP:
                    value = 10;
                    break;
                case MOBY_CLASS_FIVE_BOLT_PICKUP:
                    value = 5;
                    break;
                default:
                    value = 1;
                    break;
                }
                shrink = 0.7f;
                random_vector(scratch, frame_time * shrink, frame_time);
                fast_vec_scale(scratch + 4, scratch, 10.0f);
                fast_vec_add(scratch + 4, scratch + 4, &moby->pos);
                spin = random_integer_below(2);
                if (spin == 0) {
                    spin--;
                }
                size = random_float_between(0.4f, 0.5f);
                FUN_L00_00272060(scratch + 4,
                                 scale_game_frames(0x19),
                                 size * 0.2f,
                                 size,
                                 0x7F207F7F,
                                 0,
                                 spin,
                                 scratch,
                                 0.0f);
                spin = -spin;
                FUN_L00_00272060(scratch + 4,
                                 scale_game_frames(0x19),
                                 size * 0.14f,
                                 size * shrink,
                                 0x7F7F7F7F,
                                 1,
                                 spin,
                                 scratch,
                                 0.0f);
                source = moby->parent;
                if (source && !(source->id & 0x80)) {
                    bolt_source_stats[current_level_index][source->id].bolts += value;
                }
                current_bolt_count += value;
                level_bolts[current_level_index] += value;
                queue_animation_update(2,
                                       0x754E,
                                       func_L00_0023A658_4BE8,
                                       func_L00_0023A690_4BE8,
                                       func_L00_0023A788_4BE8,
                                       &current_bolt_count,
                                       9999999);
                if (moby < D_L08_0015FFDC) {
                    l08_bolt_pickup_collection_marks_by_save_id.killed[(short)moby->save_id] =
                        moby->mission + 2;
                    if (moby->mission == 0xFF ||
                        (D_L08_0015FC88_macro[moby->mission] != 0xFF &&
                         D_0014C050[moby->mission + current_level_index * 16] == 0xFF)) {
                        l08_bolt_pickup_mission_gated_collection_marks_by_save_id
                            .killed[(short)moby->save_id] = moby->mission + 2;
                    }
                }
                mark_moby_for_removal(moby);
                return;
            }
        }
        multiply_matrix3x3(&moby->rot_row0, vars, &moby->rot_row0);
        break;
    }
    case BOLT_PICKUP_FALL_AND_BOUNCE: {
        Vars_4BE8 *vars = (Vars_4BE8 *)moby->pvars;

        fast_vec_add(&moby->pos, &moby->pos, vars->velocity);
        vars->velocity[2] -= frame_time_sq * 10.8f;
        multiply_matrix3x3(&moby->rot_row0, vars, &moby->rot_row0);
        if (moby->pos.z < 2.0f || moby->pos.x > 510.0f || moby->pos.x < 2.0f ||
            moby->pos.y > 510.0f || moby->pos.y < 2.0f) {
            int              value;
            BoltSource_4BE8 *source;

            switch (moby->oclass) {
            case MOBY_CLASS_TEN_BOLT_PICKUP:
                value = 10;
                break;
            case MOBY_CLASS_FIVE_BOLT_PICKUP:
                value = 5;
                break;
            default:
                value = 1;
                break;
            }
            source = moby->parent;
            if (source && !(source->id & 0x80)) {
                bolt_source_stats[current_level_index][source->id].bolts += value;
            }
            current_bolt_count += value;
            level_bolts[current_level_index] += value;
            mark_moby_for_removal(moby);
            return;
        }
        if (moby->pos.z < vars->f60 - 4.0f && moby->pos.z < hero.motion.pos.f[2] - 4.0f) {
            Vars_4BE8 *vars = (Vars_4BE8 *)moby->pvars;

            *(u128_4BE8_p *)vars->v10 = *(u128_4BE8_p *)((char *)&hero + 0x90);
            vars->timer = scale_game_frames(1);
            moby->state = 2;
            break;
        }
        if (coll_sphere(0.25f, &moby->pos, 0x22, moby) != 0) {
            fast_vec_sub(direction, &moby->pos, ((CollPos_4BE8 *)D_L08_00174660)->pos);
            reflect_vector(vars->velocity, vars->velocity, direction);
            if (fast_arc_tan(direction[2], vector_length_xy(direction)) < 0.7853982f &&
                fast_arc_tan(((Coll_4BE8 *)((char *)D_L08_00174660 - 0x20))->n[2],
                             vector_length_xy(((CollPos_4BE8 *)D_L08_00174660)->n)) < 0.7853982f) {
                Coll_4BE8 *coll = (Coll_4BE8 *)((char *)D_L08_00174660 - 0x20);
                int        frames;
                float      per_frame;

                moby->flags &= ~0x100;
                vars = (Vars_4BE8 *)moby->pvars;
                frames = truncate_float_to_s32(
                    FUN_L04_001f1978(l08_bolt_pickup_surface_return_base_duration +
                                     random_float_between(-10.0f, 10.0f)));
                per_frame = 1.0f / convert_integer_to_float(frames);
                qcopy(vars->anchor, coll->pos);
                matrix_to_euler(&moby->rot_row0, &moby->rot);
                fast_vec_normalize(direction, coll->n, 1.0f);
                set_batalia_bolt_pickup_surface_rotation((char *)moby, direction);
                vars->f60 = random_angle_radians();
                vars->f64 = random_angle_radians();
                FUN_L08_002e4e90_c((char *)moby, vars->v20, vars->v30);
                fast_vec_sub(vars->velocity, vars->v20, &moby->pos);
                fast_vec_scale(vars->velocity, vars->velocity, per_frame);
                if (coll->moby != 0) {
                    unsigned char *carrier = FUN_L00_002603d0(coll->moby);

                    if (carrier != 0) {
                        fast_vec_add(vars->velocity, vars->velocity, carrier + 0x10);
                        fast_vec_scale(point, carrier + 0x10, convert_integer_to_float(frames));
                        fast_vec_add(vars->v20, vars->v20, point);
                        fast_vec_add(vars->anchor, vars->anchor, point);
                    }
                }
                vars->velocity[2] += frame_time_sq * 10.8f * (float)(frames >> 1);
                vars->spin_x = fast_subtract_rotations(vars->v30[0], moby->rot.x) * per_frame;
                vars->spin_y = fast_subtract_rotations(vars->v30[1], moby->rot.y) * per_frame;
                vars->spin_z = fast_subtract_rotations(vars->v30[2], moby->rot.z) * per_frame;
                moby->state = 5;
                vars->timer = frames;
                vars->u7C.i = D_L08_00174658_lui;
                if (absolute_float(moby->pos.z - hero.motion.pos.f[2]) < hero.reach_distance &&
                    vector_distance_xy(&moby->pos, hero.motion.pos.f) < hero.reach_height) {
                    Vars_4BE8 *vars = (Vars_4BE8 *)moby->pvars;

                    vars->timer = scale_game_frames(1);
                    moby->state = 2;
                }
            }
        }
        break;
    }
    case BOLT_PICKUP_RETURN_TO_SURFACE: {
        Vars_4BE8 *vars = (Vars_4BE8 *)moby->pvars;

        moby->rot.x = fast_add_rotations(moby->rot.x, vars->spin_x);
        moby->rot.y = fast_add_rotations(moby->rot.y, vars->spin_y);
        moby->rot.z = fast_add_rotations(moby->rot.z, vars->spin_z);
        fast_vec_add(&moby->pos, &moby->pos, vars->velocity);
        vars->velocity[2] -= frame_time_sq * 10.8f;
        if (fast_dec_timer(&vars->timer) != 0) {
            int   frames;
            float per_frame;

            if (absolute_float(moby->pos.z - hero.motion.pos.f[2]) < hero.reach_distance &&
                vector_distance_xy(&moby->pos, hero.motion.pos.f) < hero.reach_height) {
                vars->timer = scale_game_frames(1);
                moby->state = 2;
                break;
            }
            if (moby->command != 0) {
                moby->state = 1;
                if (vars->u7C.i != 0) {
                    Vars_4BE8 *vars = (Vars_4BE8 *)moby->pvars;

                    convert_pose_to_reference_local(
                        moby, vars->u7C.i, vars->anchor, vars->v10, vars->v20, vars->v30);
                } else {
                    qcopy(&moby->pos, vars->v20);
                    qcopy(&moby->rot, vars->v30);
                }
                break;
            }
            frames = truncate_float_to_s32((float)scale_game_frames(20));
            per_frame = 1.0f / convert_integer_to_float(frames);
            moby->command++;
            vars->spin_x = fast_subtract_rotations(vars->v30[0], moby->rot.x) * per_frame;
            vars->spin_y = fast_subtract_rotations(vars->v30[1], moby->rot.y) * per_frame;
            vars->spin_z = fast_subtract_rotations(vars->v30[2], moby->rot.z) * per_frame;
            fast_vec_sub(vars->velocity, vars->v20, &moby->pos);
            fast_vec_scale(vars->velocity, vars->velocity, per_frame);
            vars->timer = frames;
            if (vars->u7C.i != 0) {
                unsigned char *carrier = FUN_L00_002603d0(vars->u7C.i);

                if (carrier != 0) {
                    fast_vec_add(vars->velocity, vars->velocity, carrier + 0x10);
                    fast_vec_scale(direction, carrier + 0x10, convert_integer_to_float(frames));
                    fast_vec_add(vars->v20, vars->v20, direction);
                    fast_vec_add(vars->anchor, vars->anchor, direction);
                }
            }
            vars->velocity[2] += frame_time_sq * 10.8f * (float)(frames >> 1);
        }
        break;
    }
    }
}

#include "qcopy.h"

/* aims a turret: builds its barrel frame from the base rotation, yaw and pitch, writes the barrel rotation
 * and the muzzle point (offset per turret class) */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002E0258.c: func_L08_002E6208), where it is exact; names translated to the US level program. */

extern float bolt_pickup_default_surface_base_tilt_degrees_gp __asm__("D_L08_00161C64")
    __attribute__((sda));
extern float bolt_pickup_default_surface_planar_offset_per_scale_gp __asm__("D_L08_00161C7C")
    __attribute__((sda));
extern float five_bolt_pickup_surface_planar_offset_per_scale_gp __asm__("D_L08_00161C80")
    __attribute__((sda));
extern float ten_bolt_pickup_surface_normal_offset_per_scale_gp __asm__("D_L08_00161C78")
    __attribute__((sda));
extern float D_L08_00161CB8 __attribute__((sda));
extern float D_L08_00161CBC __attribute__((sda));
extern float ten_bolt_pickup_surface_planar_offset_per_scale_gp __asm__("D_L08_00161C84")
    __attribute__((sda));
extern float five_bolt_pickup_surface_base_tilt_degrees_gp __asm__("D_L08_00161C68")
    __attribute__((sda));
extern float ten_bolt_pickup_surface_base_tilt_degrees_gp __asm__("D_L08_00161C6C")
    __attribute__((sda));
extern float D_L08_00161CB4 __attribute__((sda));
extern float bolt_pickup_default_surface_normal_offset_per_scale_gp __asm__("D_L08_00161C70")
    __attribute__((sda));
extern float five_bolt_pickup_surface_normal_offset_per_scale_gp __asm__("D_L08_00161C74")
    __attribute__((sda));

void compute_batalia_bolt_pickup_surface_pose(
    struct Moby *moby, float *out, float *output_euler_angles) __asm__("FUN_L08_002e4e90");

void compute_batalia_bolt_pickup_surface_pose(struct Moby *moby,
                                              float       *out,

                                              float *output_euler_angles) {
    float output_rotation_matrix[16];
    float base_rotation_matrix[16];
    float up[4];
    float side[4];
    float fwd[4];
    float planar_offset_direction[4];
    float u[4];
    float v[4];
    char *data = (char *)moby->pvars;
    float sx, sy, sz;
    float basis_tilt_radians;
    switch (moby->oclass) {
    case MOBY_CLASS_FIVE_BOLT_PICKUP:
        sx = five_bolt_pickup_surface_base_tilt_degrees_gp * D_L08_00161CB8;
        sy = five_bolt_pickup_surface_normal_offset_per_scale_gp * D_L08_00161CB8;
        sz = five_bolt_pickup_surface_planar_offset_per_scale_gp * D_L08_00161CB8;
        break;
    case MOBY_CLASS_TEN_BOLT_PICKUP:
        sx = ten_bolt_pickup_surface_base_tilt_degrees_gp * D_L08_00161CBC;
        sy = ten_bolt_pickup_surface_normal_offset_per_scale_gp * D_L08_00161CBC;
        sz = ten_bolt_pickup_surface_planar_offset_per_scale_gp * D_L08_00161CBC;
        break;
    default:
        sx = bolt_pickup_default_surface_base_tilt_degrees_gp * D_L08_00161CB4;
        sy = bolt_pickup_default_surface_normal_offset_per_scale_gp * D_L08_00161CB4;
        sz = bolt_pickup_default_surface_planar_offset_per_scale_gp * D_L08_00161CB4;
        break;
    }
    euler_rotation_matrix(base_rotation_matrix, (float *)(data + 0x10));
    fast_vec_normalize(u, base_rotation_matrix, -fast_sin(*(float *)(data + 0x60)));
    fast_vec_normalize(v, base_rotation_matrix + 4, fast_cos(*(float *)(data + 0x60)));
    fast_vec_add(planar_offset_direction, u, v);
    basis_tilt_radians = sx * DEG_TO_RAD;
    fast_vec_normalize(u, planar_offset_direction, fast_cos(basis_tilt_radians));
    fast_vec_normalize(v, base_rotation_matrix + 8, fast_sin(basis_tilt_radians));
    fast_vec_add(up, u, v);
    fast_vec_normalize(u, base_rotation_matrix, -fast_cos(*(float *)(data + 0x60)));
    fast_vec_normalize(v, base_rotation_matrix + 4, -fast_sin(*(float *)(data + 0x60)));
    fast_vec_add(fwd, u, v);
    fast_vec_cross(side, up, fwd);
    fast_vec_normalize(u, side, fast_cos(*(float *)(data + 0x64)));
    fast_vec_normalize(v, fwd, fast_sin(*(float *)(data + 0x64)));
    fast_vec_add(output_rotation_matrix, u, v);
    fast_vec_normalize(u, side, -fast_sin(*(float *)(data + 0x64)));
    fast_vec_normalize(v, fwd, fast_cos(*(float *)(data + 0x64)));
    fast_vec_add(output_rotation_matrix + 4, u, v);
    qcopy(output_rotation_matrix + 8, up);
    output_rotation_matrix[11] = 0.0f;
    clear_u64_value(output_rotation_matrix + 12);
    output_rotation_matrix[15] = 1.0f;
    matrix_to_euler(output_rotation_matrix, output_euler_angles);
    output_euler_angles[3] = 0.0f;
    fast_vec_normalize(u, base_rotation_matrix + 8, sy);
    fast_vec_add(out, data, u);
    fast_vec_normalize(v, planar_offset_direction, sz);
    fast_vec_add(out, out, v);
}

typedef struct {
    s64 vertices[4];
    u32 colors[4];
    u32 texture_coordinates[4];
} Quad_5188;

extern void draw_geometry_quad_5188(void *, void *, void *, u64, int) __asm__("FUN_L02_0020bc88");

#include "rnc/overlay/l08/mobys/bolt_thief_data/draw_batalia_bolt_pickup_rotating_quad.h"

extern char D_L08_0016D240[] NOT_SDA;

void draw_batalia_bolt_pickup_rotating_quad(struct Moby *moby) __asm__("FUN_L08_002e5188");

void draw_batalia_bolt_pickup_rotating_quad(struct Moby *moby) {
    char     *data = (char *)moby->pvars;
    Quad_5188 quad;
    float     pos[4];
    float     direction[4];
    float     length;
    float     radius;
    float     x0, x1, x2, x3, y0, y1, y2, y3;
    int       base_x, base_y;
    s64       base_z;
    u32       color;
    char     *projection_constants;

    if (*(short *)(data + 0x6c) == 0)
        return;

    qcopy(pos, &moby->pos);
    fast_vec_normalize(
        direction, (char *)moby + 0xe0, bolt_pickup_quad_center_offset_along_moby_e0);
    fast_vec_add(pos, pos, direction);
    fast_vec_normalize(
        direction, (char *)moby + 0xc0, bolt_pickup_quad_center_offset_along_moby_c0);
    fast_vec_add(pos, pos, direction);
    fast_vec_sub(pos, pos, D_L08_001675C0);
    length = fast_vec_length(pos);
    pos[3] = 1.0f;
    fast_vec_scale(pos, pos, 1024.0f);
    transform_vector(pos, pos, D_L08_001675C0 - 0x100);
    projection_constants = D_L08_0016D240;
    fast_vec_scale(pos, pos, *(float *)(projection_constants + 0x210) / pos[3]);

    base_x = truncate_float_to_s32(pos[0] * 16.0f) + 0x8000;
    base_y = (truncate_float_to_s32(pos[1] * 16.0f) + 0x8000) << 16;
    base_z = (s64)truncate_float_to_s32(pos[2] * 0.9997f + *(float *)(projection_constants + 0x1a8))
             << 32;
    if (length > 16.0f)
        length = 16.0f;
    else if (length < 4.0f)
        length = 4.0f;
    radius = convert_integer_to_float(*(signed char *)(data + 0x7b) + 16) * 0.015625f *
             (50.0f - length * 2.5f);
    x0 = radius * fast_sin(*(float *)(data + 0x74));
    y0 = radius * fast_cos(*(float *)(data + 0x74));
    x1 = radius * fast_sin(fast_add_rotations(*(float *)(data + 0x74), 1.57079637f));
    y1 = radius * fast_cos(fast_add_rotations(*(float *)(data + 0x74), 1.57079637f));
    x2 = radius * fast_sin(fast_add_rotations(*(float *)(data + 0x74), 4.71238899f));
    y2 = radius * fast_cos(fast_add_rotations(*(float *)(data + 0x74), 4.71238899f));
    x3 = radius * fast_sin(fast_add_rotations(*(float *)(data + 0x74), 3.14159274f));
    y3 = radius * fast_cos(fast_add_rotations(*(float *)(data + 0x74), 3.14159274f));

    quad.texture_coordinates[0] = 0;
    quad.texture_coordinates[1] = 0x200;
    quad.texture_coordinates[2] = 0x2000000;
    quad.texture_coordinates[3] = 0x2000200;
    color = *(u32 *)(data + 0x78);
    quad.colors[3] = color;
    quad.colors[2] = color;
    quad.colors[1] = color;
    quad.colors[0] = color;
    quad.vertices[3] = base_z;
    quad.vertices[2] = base_z;
    quad.vertices[1] = base_z;
    quad.vertices[0] = base_z;
    quad.vertices[0] += (truncate_float_to_s32(y0) << 20) + base_y;
    quad.vertices[0] += (truncate_float_to_s32(x0) << 4) + base_x;
    quad.vertices[1] += (truncate_float_to_s32(y1) << 20) + base_y;
    quad.vertices[1] += (truncate_float_to_s32(x1) << 4) + base_x;
    quad.vertices[2] += (truncate_float_to_s32(y2) << 20) + base_y;
    quad.vertices[2] += (truncate_float_to_s32(x2) << 4) + base_x;
    quad.vertices[3] += (truncate_float_to_s32(y3) << 20) + base_y;
    quad.vertices[3] += (truncate_float_to_s32(x3) << 4) + base_x;
    vu1_add_g_sregister(0x42, 0x8000000048LL);
    draw_geometry_quad_5188(
        quad.vertices, quad.texture_coordinates, quad.colors, get_effect_texture(0x13), 1);
    vu1_add_g_sregister(0x42, 0x8000000044LL);
}
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002E0258.c: func_L08_002E9CB0), where it is exact; names translated to the US level program. */

void process_shrub_part_hits(struct Moby *parent_moby) __asm__("FUN_L08_002e88d8");

void process_shrub_part_hits(struct Moby *parent_moby) {
    char  *data = ((char *)parent_moby->pvars);
    char  *base = data + 0x60;
    char **part_entry_cursor = (char **)base;
    char **sq;
    int    weighted_hit_score = 0;
    int    selected_hit_part_index = 0;
    int    hit_sound_id = -1;
    int    i;
    for (i = 0; i < 13; i++, part_entry_cursor += 4) {
        if (*part_entry_cursor != 0) {
            if (find_moby_hit(*part_entry_cursor, 0x10000, 0) != 0) {
                if (i < 3) {
                    if (weighted_hit_score <= 0) {
                        weighted_hit_score = 1;
                        hit_sound_id = 1;
                    }
                } else if (i < 5) {
                    if (weighted_hit_score < 2) {
                        weighted_hit_score = 2;
                        hit_sound_id = 1;
                    }
                } else {
                    if (weighted_hit_score < 4) {
                        weighted_hit_score += 4;
                        selected_hit_part_index = i;
                        hit_sound_id = 2;
                    }
                }
                ((unsigned char *)*part_entry_cursor)[0xA4] = 0xFF;
            }
        }
    }
    *(unsigned short *)(data + 0x13C) += weighted_hit_score;
    if (*(short *)(data + 0x13C) >= 8) {
        *(short *)(data + 0x13C) = *(short *)(data + 0x13C) % 8;
        if (selected_hit_part_index != 0) {
            int    selected_part_byte_offset = selected_hit_part_index * 16;
            char **selected_part_entry = (char **)(base + selected_part_byte_offset);
            char  *selected_part_moby = *selected_part_entry;
            char  *d2;
            d2 = ((char *)MOBY(selected_part_moby)->pvars);
            cart_burst(parent_moby, selected_part_moby + 0x10, -1, 2.0f, 13.0f);
            fast_vec_sub(d2,
                         *selected_part_entry + 0x10,
                         *(char **)(selected_part_byte_offset + data + 0x64) + 0x10);
            fast_vec_normalize(d2, d2, D_0015ED6C * 10.0f);
            (*selected_part_entry)[0x20] = 1;
            *selected_part_entry = 0;
            return;
        } else {
            int   random_search_start = truncate_float_to_s32(random_float_between(0.0f, 23.0f));
            char *part_table_base = base;
            for (i = 0; i < 8; i++) {
                int k = (i + random_search_start) % 8 + 5;
                int fallback_part_byte_offset = k * 16;
                base = part_table_base + fallback_part_byte_offset;
                sq = (char **)base;
                if (*sq != 0) {
                    char *d2 = *(char **)(*sq + 0x78);
                    cart_burst(parent_moby, *sq + 0x10, -1, 2.0f, 13.0f);
                    fast_vec_sub(
                        d2, *sq + 0x10, *(char **)(data + fallback_part_byte_offset + 0x64) + 0x10);
                    fast_vec_normalize(d2, d2, D_0015ED6C * 10.0f);
                    (*sq)[0x20] = 1;
                    *sq = 0;
                    return;
                }
            }
            parent_moby->state = 0x63;
        }
    }
    if (hit_sound_id >= 0)
        allocate_voice_for_target_entry(hit_sound_id, 0, (int)parent_moby);
}
/* Flying gunship: cruises one path and fires at its target, or circles a second path and fires at random. */

typedef struct {
    SwarmPathFollower follow[2];    /* 0x00: [0] cruising (state 1), [1] circling (state 2) */
    SwarmPart         root;         /* 0x60 */
    SwarmPart         parts[12];    /* 0x70 */
    s32               target_index; /* 0x130: moby slot of the target */
    f32               turn_speed;   /* 0x134 */
    s16               can_skip;     /* 0x138: non-zero lets the circling jump ahead (see state 2) */
    s16               fire_timer;   /* 0x13A */
    s16               pad13C;
    s16               sound; /* 0x13E */
} SwarmVars;

extern s32                shrub_updates_disabled_gp __asm__("D_L08_00161D30") __attribute__((sda));
extern f32                shrub_path_speed_gp __asm__("D_L08_00161D24") __attribute__((sda));
extern s32                level_game_mode __asm__("D_L08_0015F5C4");
extern s64 D_L08_0016D090 NOT_SDA;

extern void spawn_shrub_parts(char *) __asm__("FUN_L08_002e8788");
extern void update_shrub_parts(struct Moby *) __asm__("FUN_L08_002e8ba0");

void shrub_update(struct Moby *moby) __asm__("FUN_L08_002e8cd0");

void shrub_update(struct Moby *moby) {
    enum {
        SHRUB_INITIALIZE = 0,
        SHRUB_FOLLOW_PRIMARY_PATH_AND_FIRE_AT_TARGET = 1,
        SHRUB_FOLLOW_SECONDARY_PATH_AND_FIRE_RANDOMLY = 2,
        SHRUB_BEGIN_PRIMARY_PATH = 3,
        SHRUB_BREAK_APART_AND_REMOVE = 0x63
    };
    SwarmVars         *vars = (SwarmVars *)moby->pvars;
    SwarmPathFollower *follower;
    struct Moby       *target;
    struct Moby       *shot;
    struct Moby       *bolt;
    SwarmTargetVars   *target_vars;
    Vec4               point;
    Vec4               step;
    Vec4               muzzle;
    Vec4               velocity;
    s32                waypoint;
    s32                next;
    s32                after;
    s32                i;
    s32                slot;
    f32                distance;
    f32                heading;
    f32                next_heading;
    f32                turn;
    f32                pitch;
    f32                yaw;

    if (shrub_updates_disabled_gp != 0) {
        return;
    }
    if (!voice_is_playing(moby, vars->sound)) {
        vars->sound = allocate_voice_for_target_entry(3, 4, moby);
    }
    switch (moby->state) {
    case SHRUB_INITIALIZE:
        if (D_0014C050[moby->mission + current_level_index * 16] == 0xFF) {
            mark_moby_for_removal(moby);
            return;
        }
        vars->follow[0].path = l08_paths[vars->follow[0].path_id];
        vars->follow[1].path = l08_paths[vars->follow[1].path_id];
        vars->fire_timer = truncate_float_to_s32(scale_time(random_float_between(300.0f, 600.0f)));
        moby->flags |= 0x4000;
        qcopy(&moby->pos, &vars->follow[1].path->points[0]);
        spawn_shrub_parts((char *)moby);
        moby->state = 2;
        moby->rot.z = fast_arc_tan(vars->follow[1].path->points[1].f[0] - moby->pos.x,
                                   vars->follow[1].path->points[1].f[1] - moby->pos.y);
        break;
    case SHRUB_FOLLOW_PRIMARY_PATH_AND_FIRE_AT_TARGET:
        waypoint =
            (vars->follow[0].point + vars->follow[0].path->count + vars->follow[0].direction) %
            vars->follow[0].path->count;
        target = &D_L08_0015FFD8_m[vars->target_index];
        FUN_L00_00258278(moby,
                         fast_arc_tan(vars->follow[0].path->points[waypoint].f[0] - moby->pos.x,
                                      vars->follow[0].path->points[waypoint].f[1] - moby->pos.y),
                         &vars->turn_speed,
                         0.004f,
                         0.3f,
                         0.02f);
        process_shrub_part_hits(moby);
        update_shrub_parts((char *)moby);
        qcopy(&point, &vars->follow[0].path->points[waypoint]);
        if (fast_vec_dist(&moby->pos, &point) < 0.5f) {
            vars->follow[0].point = waypoint;
        }
        fast_vec_sub(&step, &point, &moby->pos);
        if (fast_vec_length(&step) > shrub_path_speed_gp * frame_time) {
            fast_vec_normalize(&step, &step, shrub_path_speed_gp * frame_time);
        }
        fast_vec_add(&moby->pos, &moby->pos, &step);
        if (moby->command == 0) {
            break;
        }
        moby->command = 0;
        moby_point_position(vars->parts[3].moby, 1, &muzzle);
        pitch = random_float_between(0.17453292f, 0.5235988f);
        yaw = fast_add_rotations(
            fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y), 3.1415927f);
        yaw = fast_add_rotations(yaw, random_float_between(-0.2617994f, 0.7853982f));
        if (yaw > 0.0f && yaw < 0.34f) {
            yaw = 0.34f;
        } else if (yaw <= 0.0f && yaw > -0.34f) {
            yaw = -0.34f;
        }
        fast_vec_normalize(
            &velocity, &step, (shrub_path_speed_gp + shrub_path_speed_gp) * frame_time);
        shot = spawn_class_1b3_projectile(&muzzle, target, &velocity, pitch, yaw);
        shot->scale = shot->pclass->scale / 5.0f;
        target_vars = (SwarmTargetVars *)target->pvars;
        for (slot = 0; slot < 8; slot++) {
            if (target_vars->shots[slot] == NULL) {
                target_vars->shots[slot] = shot;
                break;
            }
        }
        break;
    case SHRUB_FOLLOW_SECONDARY_PATH_AND_FIRE_RANDOMLY:
        follower = &vars->follow[1];
        if (level_game_mode == 2 && vars->can_skip != 0 && D_L08_0016D090 == 0x200000000) {
            qcopy(&moby->pos, &follower->path->points[15]);
            follower->point = 15;
            moby->rot.z = fast_arc_tan(follower->path->points[11].f[0] - moby->pos.x,
                                       follower->path->points[11].f[1] - moby->pos.y);
        }
        next = (follower->point + 1) % follower->path->count;
        after = (follower->point + 2) % follower->path->count;
        heading = fast_arc_tan(
            follower->path->points[next].f[0] - follower->path->points[follower->point].f[0],
            follower->path->points[next].f[1] - follower->path->points[follower->point].f[1]);
        next_heading =
            fast_arc_tan(follower->path->points[after].f[0] - follower->path->points[next].f[0],
                         follower->path->points[after].f[1] - follower->path->points[next].f[1]);
        turn = fast_subtract_rotations(heading, next_heading);
        distance = fast_vec_dist(&moby->pos, &follower->path->points[next]);
        distance /=
            fast_vec_dist(&follower->path->points[follower->point], &follower->path->points[next]);
        moby->rot.z = fast_add_rotations(turn * distance, next_heading);
        qcopy(&point, &follower->path->points[next]);
        if (fast_vec_dist(&moby->pos, &point) < 0.5f) {
            follower->point = next;
        }
        fast_vec_sub(&step, &point, &moby->pos);
        if (fast_vec_length(&step) > shrub_path_speed_gp * frame_time) {
            fast_vec_normalize(&step, &step, shrub_path_speed_gp * frame_time);
        }
        fast_vec_add(&moby->pos, &moby->pos, &step);
        update_shrub_parts((char *)moby);
        if (!fast_dec_timer_s16(&vars->fire_timer)) {
            break;
        }
        vars->fire_timer = truncate_float_to_s32(scale_time(random_float_between(300.0f, 600.0f)));
        moby_point_position(vars->parts[3].moby, 1, &muzzle);
        pitch = random_float_between(-0.5235988f, -0.2617994f);
        yaw = random_angle_radians();
        fast_vec_normalize(
            &velocity, &step, (shrub_path_speed_gp + shrub_path_speed_gp) * frame_time);
        bolt = spawn_class_1b3_projectile(&muzzle, NULL, &velocity, pitch, yaw);
        if (bolt != NULL) {
            bolt->rot.z = vars->parts[3].moby->rot.z;
            bolt->state = 5;
            bolt->rot.y = 0.0f;
        }
        break;
    case SHRUB_BEGIN_PRIMARY_PATH: {
        LevelPath *path;

        moby->state = 1;
        path = vars->follow[0].path;
        qcopy(&moby->pos, &path->points[0]);
        moby->rot.z =
            fast_arc_tan(path->points[1].f[0] - moby->pos.x, path->points[1].f[1] - moby->pos.y);
        update_shrub_parts((char *)moby);
        break;
    }
    case SHRUB_BREAK_APART_AND_REMOVE:
        for (i = 0; i < 12; i++) {
            struct Moby *part = vars->parts[i].moby;
            if (part != NULL) {
                Vec4 *drift = (Vec4 *)part->pvars;
                cart_burst(moby, &part->pos, -1, 3.0f, 13.0f);
                fast_vec_sub(drift, &part->pos, &moby->pos);
                fast_vec_normalize(drift, drift, frame_time * 5.0f);
                part->state = 1;
                vars->parts[i].moby = NULL;
            }
        }
        cart_burst(moby, &moby->pos, -1, 5.0f, 13.0f);
        mark_moby_for_removal(moby);
        break;
    }
}
/* Builds the moby's part hierarchy: body, head, arms and eight trailing segments. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002E0258.c: func_L08_002EA930), where it is exact; names translated to the US level program. */

void *
FUN_L08_002e1c98_c2(char *parent, int oClass, int joint, int pjoint) __asm__("FUN_L08_002e1c98");

void spawn_joint_linked_moby_parts(struct Moby *moby) __asm__("FUN_L08_002e9558");

void spawn_joint_linked_moby_parts(struct Moby *moby) {
    char        *data = ((char *)moby->pvars);
    char        *anchor_joint_cursor = data + 0xFC;
    int          i = 0;
    struct Moby *child_moby;
    char        *attachment_anchor_moby;
    struct Moby *root_joint_attachment_moby;
    struct Moby *anchor_joint_attachment_moby;
    *(char **)(data + 0x60) = moby;
    *(int *)(data + 0x64) = 0;
    *(int *)(data + 0x68) = 0;
    *(int *)(data + 0x6C) = 0;
    *(char **)(data + 0x90) = FUN_L08_002e1c98_c2(moby, 0x1C0, 0, 0);
    *(char **)(data + 0x94) = moby;
    *(int *)(data + 0x98) = 0;
    *(int *)(data + 0x9C) = 0;
    child_moby = FUN_L08_002e1c98_c2(moby, 0x1BD, 0, 2);
    *(char **)(data + 0x70) = child_moby;
    *(int *)(data + 0x7C) = 2;
    *(char **)(data + 0x74) = moby;
    *(int *)(data + 0x78) = 0;
    child_moby->scale = *(float *)(((char *)child_moby->pclass) + 0x24) * 0.75f;
    attachment_anchor_moby = FUN_L08_002e1c98_c2(*(char **)(data + 0x90), 0x1BA, 1, 1);
    *(char **)(data + 0x80) = attachment_anchor_moby;
    *(char **)(data + 0x84) = *(char **)(data + 0x90);
    *(int *)(data + 0x88) = 1;
    *(int *)(data + 0x8C) = 1;
    *(char **)(data + 0xA0) = FUN_L08_002e1c98_c2(attachment_anchor_moby, 0x1B9, 0, 0);
    *(char **)(data + 0xA4) = *(char **)(data + 0x80);
    *(int *)(data + 0xA8) = 0;
    *(int *)(data + 0xAC) = 0;
    *(char **)(data + 0xB0) = FUN_L08_002e1c98_c2(moby, 0x1D0, 0, 1);
    *(char **)(data + 0xB4) = moby;
    *(int *)(data + 0xB8) = 0;
    *(int *)(data + 0xBC) = 1;
    root_joint_attachment_moby = FUN_L08_002e1c98_c2(moby, 0x1D0, 0, 1);
    *(char **)(data + 0xC0) = root_joint_attachment_moby;
    *(char **)(data + 0xC4) = moby;
    *(int *)(data + 0xC8) = 0;
    *(int *)(data + 0xCC) = 1;
    root_joint_attachment_moby->flags |= 0x8000;
    *(char **)(data + 0xD0) = FUN_L08_002e1c98_c2(*(char **)(data + 0x80), 0x1D1, 0, 0);
    *(char **)(data + 0xD4) = *(char **)(data + 0x80);
    *(int *)(data + 0xD8) = 0;
    *(int *)(data + 0xDC) = 0;
    anchor_joint_attachment_moby = FUN_L08_002e1c98_c2(*(char **)(data + 0x80), 0x1D1, 0, 0);
    *(char **)(data + 0xE0) = anchor_joint_attachment_moby;
    *(char **)(data + 0xE4) = *(char **)(data + 0x80);
    *(int *)(data + 0xE8) = 0;
    *(int *)(data + 0xEC) = 0;
    anchor_joint_attachment_moby->flags |= 0x8000;
    do {
        int j = i + 2;
        i++;
        *(char **)(anchor_joint_cursor - 0xC) =
            FUN_L08_002e1c98_c2(*(char **)(data + 0x90), 0x1BB, 0, j);
        *(char **)(anchor_joint_cursor - 8) = *(char **)(data + 0x90);
        *(int *)(anchor_joint_cursor - 4) = 0;
        *(int *)anchor_joint_cursor = j;
        anchor_joint_cursor += 0x10;
    } while (i < 8);
}

void process_linked_moby_part_hits(struct Moby *moby) __asm__("FUN_L08_002e9758");

void process_linked_moby_part_hits(struct Moby *moby) {
    int        selected = 0;
    char      *data = (char *)moby->pvars;
    SwarmPart *slots = (SwarmPart *)(data + 0x60);
    SwarmPart *slot = slots;
    int        i = 0;
    int        hits = 0;
    int        sound = -1;
    int        total;
    u8         mark = 0xff;

    do {
        if (slot->moby != 0) {
            if (find_moby_hit(slot->moby, 0x10000, 0) != 0) {
                if (i < 3) {
                    if (hits < 1) {
                        hits = 1;
                        sound = 1;
                        goto mark_hit;
                    }
                } else if (i < 9) {
                    if (hits < 2) {
                        hits = 2;
                        sound = 1;
                        goto mark_hit;
                    }
                } else if (hits < 4) {
                    hits += 4;
                    selected = i;
                    sound = 2;
                    goto mark_hit;
                }
            mark_hit:
                slot->moby->hit_slot = mark;
            }
        }
        i++;
        slot++;
    } while (i < 17);
    total = *(u16 *)(data + 0x17c) + hits;
    *(s16 *)(data + 0x17c) = total;
    if ((s16)total >= 12) {
        *(s16 *)(data + 0x17c) = (s16)total % 12;
        if (selected != 0) {
            SwarmPart *chosen = &slots[selected];
            u8        *vector;
            char      *position = data;
            vector = chosen->moby->pvars;
            cart_burst(moby, &chosen->moby->pos, -1, 2.0f, 13.0f);
            position += selected * 16;
            fast_vec_sub(vector, &chosen->moby->pos, &(*(struct Moby **)(position + 0x64))->pos);
            fast_vec_normalize(vector, vector, D_0015ED6C * 10.0f);
            chosen->moby->state = 1;
            chosen->moby = 0;
            return;
        }
        {
            int        start = truncate_float_to_s32(random_float_between(0.0f, 23.0f));
            SwarmPart *base = slots;
            i = 0;
            do {
                int        index = (i + start) % 8 + 9;
                SwarmPart *candidate;
                slots = &base[index];
                candidate = slots;
                if (candidate->moby != 0) {
                    u8   *vector = candidate->moby->pvars;
                    char *position = data;
                    cart_burst(moby, &candidate->moby->pos, -1, 2.0f, 13.0f);
                    position += index * 16;
                    fast_vec_sub(
                        vector, &candidate->moby->pos, &(*(struct Moby **)(position + 0x64))->pos);
                    fast_vec_normalize(vector, vector, D_0015ED6C * 10.0f);
                    candidate->moby->state = 1;
                    candidate->moby = 0;
                    return;
                }
                i++;
            } while (i < 8);
            moby->state = 99;
        }
    }
    if (sound >= 0)
        allocate_voice_for_target_entry(sound, 0, moby);
}

Pair_2E2A38 D_L08_00161C30 = {
    {0x2f000000, 0x3f000000}
};

int D_L08_00161AD4 = 0x600000ff;

int D_L08_00161AD8 = 0x600080ff;

int D_L08_00161ADC = 0x60ffffff;
