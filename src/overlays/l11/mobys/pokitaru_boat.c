/* Level 11 moby class pokitaru_boat: steering, joint effects and its target group. */
#include "types.h"

#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/mobys/moby.h"
#include "rnc/game/item_state.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/mobys/attach_manipulator.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/overlay/level_labels.h"

#include "qcopy.h"

extern float
approach_angle(float *p, float a, float *v, float b, float c, float d) __asm__("FUN_L00_0025be00");
extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern float camera_pos_b358[] __asm__("D_L11_001677C0");
extern int   D_L11_0015FFD8;

#include "rnc/overlay/level_path.h"
#include "rnc/overlay/l11/mobys/puffoid.h"

extern short *D_L11_001AC240[];
extern int    D_L11_0015FFF4;

extern float D_0013F3D8_lui __asm__("D_0013F3D8") NOT_SDA;
extern void
advance_accelerated_scalar(float *, float *, float, float, float, float) __asm__("FUN_00213f38");
extern void approach_value(float *, float, float) __asm__("FUN_00213ed8");

struct PokPv {
    char  pad00[0x60];
    char  ctl60[0x3C];
    int   flags;
    int   path_a0;
    int   pad_a4;
    int   idx_a8;
    int   idx_ac;
    int   flag_b0;
    int   pad_b4;
    int   flag_b8;
    int   pad_bc;
    int   flag_c0;
    int  *path_c4;
    int   slot_c8;
    int   slot_cc;
    float approach_d0;
    int   seg_d4;
    float vel_d8[2];
    float angle_e0;
    float height_e4;
    float spr_e8;
    float spr_ec;
    float wobble_f0[4];
};

extern void *D_0013F64C_lui __asm__("D_0013F64C") NOT_SDA;
extern float random_angle_radians(void) __asm__("FUN_00213308");
extern void  scale_vec_309ac0(float *, float) __asm__("FUN_L00_0025f730");
extern void  update_moby_rotation_oscillation(void *, void *, void *, float, float, float) __asm__("FUN_L00_00262b80");
extern void follow_camera_set_distance(int, float, float) __asm__("FUN_L00_002e8450");
extern void follow_camera_set_height(float, float) __asm__("FUN_L00_002e84b8");
void        update_pokitaru_boat_joint_effects(struct Moby *moby) __asm__("FUN_L11_0030a500");

extern void update_pokitaru_boat_joint_effects(struct Moby *) __asm__("FUN_L11_0030a500");
int         is_pokitaru_boat_target_group_clear(struct Moby *moby) __asm__("FUN_L11_0030a850");

extern int is_pokitaru_boat_target_group_clear(struct Moby *) __asm__("FUN_L11_0030a850");

#include "rnc/overlay/l11/mobys/pokitaru_boat_globals.h"

extern float pokitaru_boat_path_speed_gp __asm__("D_L11_00161DD4") __attribute__((sda));
extern float pokitaru_boat_turn_acceleration_degrees_gp __asm__("D_L11_00161DD8")
    __attribute__((sda));
extern float pokitaru_boat_max_turn_speed_degrees_gp __asm__("D_L11_00161DDC") __attribute__((sda));

#include "rnc/overlay/l11/mobys/pokitaru_boat_data/pokitaru_boat_update.h"

void pokitaru_boat_update(struct Moby *moby) __asm__("FUN_L11_00309ac0");

/* Moby class 1075 */
void pokitaru_boat_update(struct Moby *moby) {
    enum {
        POKITARU_BOAT_INITIALIZE = 0,
        POKITARU_BOAT_WAIT_FOR_ACTIVATION = 1,
        POKITARU_BOAT_FOLLOW_PATH = 2,
        POKITARU_BOAT_WAIT_FOR_GROUP_CLEAR = 3,
        POKITARU_BOAT_RISE_ABOVE_PATH = 4,
        POKITARU_BOAT_ADJUST_HEIGHT_FOR_HERO = 5
    };
    struct PokPv *boat_vars = (struct PokPv *)moby->pvars;
    float         start_pos[4];
    float         start_rot[4];
    float         delta[4];
    float         target[4];
    float         path_movement[4];
    float         spare_a[4];
    float         spare_b[4];
    float         point_offset_from_boat[4];

    qcopy(start_pos, &moby->pos);
    qcopy(start_rot, &moby->rot);
    if (moby->state != 0) {
        moby->pos.z = boat_vars->height_e4;
    }
    update_pokitaru_boat_joint_effects(moby);

    switch (moby->state) {
    case POKITARU_BOAT_INITIALIZE: {
        int *tab_a8;
        int *tab_ac;
        int *path;
        int  n;
        int  part_angle_index;

        moby->draw_dist = 0xFF;
        if (moby->prev_seq != 1) {
            blend_moby_animation(moby, 1, 0, 1);
        }
        tab_a8 = (int *)l11_paths[boat_vars->idx_a8];
        tab_ac = (int *)l11_paths[boat_vars->idx_ac];
        transpose_rotation_matrix(target, &moby->rot_row0);
        for (n = 0; n < tab_a8[0]; n++) {
            fast_vec_sub(point_offset_from_boat, (char *)tab_a8 + 0x10 + n * 16, &moby->pos);
            transform_vector((char *)tab_ac + 0x10 + n * 16, point_offset_from_boat, target);
        }
        path = (int *)l11_paths[boat_vars->path_a0];
        boat_vars->path_c4 = path;
        qcopy(&moby->pos, (char *)path + 0x10);
        boat_vars->seg_d4 = 1;
        moby->state = 1;
        for (part_angle_index = 0; part_angle_index < 4; part_angle_index++) {
            boat_vars->wobble_f0[part_angle_index] = random_angle_radians();
        }
        boat_vars->slot_c8 = -1;
        boat_vars->slot_cc = -1;
        boat_vars->flags |= 1;
        break;
    }
    case POKITARU_BOAT_WAIT_FOR_ACTIVATION:
        if (boat_vars->flag_b8 != 0) {
            int   *path = (int *)l11_paths[boat_vars->path_a0];
            int    n;
            float *last;

            boat_vars->path_c4 = path;
            n = path[0];
            qcopy(&moby->pos, (char *)path + n * 16 - 16);
            last = (float *)((char *)path + (n - 1) * 16);
            moby->rot.z = fast_arc_tan(last[4] - moby->pos.x, last[5] - moby->pos.y);
            moby->state = 5;
        }
        break;
    case POKITARU_BOAT_FOLLOW_PATH: {
        int *tab_a8;
        int *tab_ac;
        int  i;

        qcopy(target, (char *)boat_vars->path_c4 + 0x10 + boat_vars->seg_d4 * 16);
        if (D_0013F64C_lui == moby) {
            if (!voice_is_playing(moby, boat_vars->slot_cc)) {
                boat_vars->slot_cc = allocate_voice_for_target_entry(1, 4, moby);
            }
            if (voice_is_playing(moby, boat_vars->slot_c8)) {
                if (boat_vars->slot_c8 != -1) {
                    unsigned char *ent = (unsigned char *)&voice_pool + boat_vars->slot_c8 * 0x70;
                    if (*(void **)(ent + 0x88) == moby && ent[0x74] != 0) {
                        release_voice_slot(boat_vars->slot_c8);
                    }
                }
                boat_vars->slot_c8 = -1;
            }
            approach_value(&boat_vars->approach_d0,
                           pokitaru_boat_path_speed_gp * frame_time,
                           frame_time_sq * 6.0f);
        } else {
            if (!voice_is_playing(moby, boat_vars->slot_c8)) {
                boat_vars->slot_c8 = allocate_voice_for_target_entry(0, 4, moby);
            }
            if (voice_is_playing(moby, boat_vars->slot_cc)) {
                if (boat_vars->slot_cc != -1) {
                    unsigned char *ent = (unsigned char *)&voice_pool + boat_vars->slot_cc * 0x70;
                    if (*(void **)(ent + 0x88) == moby && ent[0x74] != 0) {
                        release_voice_slot(boat_vars->slot_cc);
                    }
                }
                boat_vars->slot_cc = -1;
            }
            approach_value(&boat_vars->approach_d0, 0.0f, frame_time_sq * 6.0f);
        }
        fast_vec_sub(path_movement, target, &moby->pos);
        scale_vec_309ac0(path_movement, boat_vars->approach_d0);
        fast_vec_add(&moby->pos, &moby->pos, path_movement);
        if (boat_vars->seg_d4 < boat_vars->path_c4[0] - 1) {
            if (fast_vec_dist(&moby->pos, target) < pokitaru_boat_path_speed_gp * frame_time) {
                float *pt;
                boat_vars->seg_d4 += 1;
                pt = (float *)((char *)boat_vars->path_c4 + boat_vars->seg_d4 * 16);
                if (pt[7] == 37.0f) {
                    boat_vars->flag_c0 = 1;
                }
            }
        } else if (vector_length_xy(path_movement) < 0.001f) {
            if (voice_is_playing(moby, boat_vars->slot_cc)) {
                if (boat_vars->slot_cc != -1) {
                    unsigned char *ent = (unsigned char *)&voice_pool + boat_vars->slot_cc * 0x70;
                    if (*(void **)(ent + 0x88) == moby && ent[0x74] != 0) {
                        release_voice_slot(boat_vars->slot_cc);
                    }
                }
                boat_vars->slot_cc = -1;
            }
            if (voice_is_playing(moby, boat_vars->slot_c8)) {
                if (boat_vars->slot_c8 != -1) {
                    unsigned char *ent = (unsigned char *)&voice_pool + boat_vars->slot_c8 * 0x70;
                    if (*(void **)(ent + 0x88) == moby && ent[0x74] != 0) {
                        release_voice_slot(boat_vars->slot_c8);
                    }
                }
                boat_vars->slot_c8 = -1;
            }
            moby->state = 3;
        }
        approach_angle(&moby->rot.z,
                       fast_arc_tan(target[0] - moby->pos.x, target[1] - moby->pos.y),
                       boat_vars->vel_d8,
                       pokitaru_boat_turn_acceleration_degrees_gp * 0.017453292f * frame_time_sq,
                       pokitaru_boat_turn_acceleration_degrees_gp * 0.017453292f * frame_time_sq,
                       pokitaru_boat_max_turn_speed_degrees_gp * 0.017453292f * frame_time);
        if (boat_vars->flag_c0 != 0) {
            follow_camera_set_distance(0, pokitaru_boat_camera_distance_gp, 0.003f);
            follow_camera_set_height(pokitaru_boat_camera_height_gp, 0.003f);
        }
        tab_a8 = (int *)l11_paths[boat_vars->idx_a8];
        tab_ac = (int *)l11_paths[boat_vars->idx_ac];
        for (i = 0; i < tab_a8[0]; i++) {
            transform_vector_by_basis(
                (char *)tab_a8 + 0x10 + i * 16, (char *)tab_ac + 0x10 + i * 16, &moby->rot_row0);
            fast_vec_add(
                (char *)tab_a8 + 0x10 + i * 16, (char *)tab_a8 + 0x10 + i * 16, &moby->pos);
        }
        break;
    }
    case POKITARU_BOAT_WAIT_FOR_GROUP_CLEAR:
        if (is_pokitaru_boat_target_group_clear(moby) != 0) {
            if (boat_vars->flag_b0 != 0) {
                boat_vars->approach_d0 = 0.0f;
                moby->state = 4;
            } else {
                moby->state = 5;
            }
        }
        break;
    case POKITARU_BOAT_RISE_ABOVE_PATH:
        qcopy(target, (char *)boat_vars->path_c4 + 0x10 + boat_vars->seg_d4 * 16);
        advance_accelerated_scalar(&moby->pos.z,
                                   &boat_vars->approach_d0,
                                   target[2] + 6.0f,
                                   frame_time_sq,
                                   frame_time_sq,
                                   frame_time);
        if (target[2] + 5.9f <= moby->pos.z) {
            moby->state = 5;
        }
        break;
    case POKITARU_BOAT_ADJUST_HEIGHT_FOR_HERO:
        qcopy(target, (char *)boat_vars->path_c4 + 0x10 + boat_vars->seg_d4 * 16);
        if (boat_vars->flag_b0 != 0) {
            if (D_0013F3D8_lui < moby->pos.z) {
                advance_accelerated_scalar(&moby->pos.z,
                                           &boat_vars->approach_d0,
                                           target[2] + 1.0f,
                                           frame_time_sq,
                                           frame_time_sq,
                                           frame_time);
            } else {
                advance_accelerated_scalar(&moby->pos.z,
                                           &boat_vars->approach_d0,
                                           target[2] + 8.0f,
                                           frame_time_sq,
                                           frame_time_sq,
                                           frame_time);
            }
        }
        break;
    }

    {
        float ft = frame_time;
        float spin = ft * 1.5707964f;
        float tilt_a = ft * 0.43633232f;
        float tilt_b = ft * 0.61086524f;
        float amp;

        boat_vars->height_e4 = moby->pos.z;
        boat_vars->angle_e0 = fast_add_rotations(boat_vars->angle_e0, spin);
        amp = 0.13f;
        moby->pos.z = moby->pos.z + fast_sin(boat_vars->angle_e0) * amp;
        update_moby_rotation_oscillation(
            moby, &boat_vars->spr_e8, &boat_vars->spr_ec, 0.0418879017f, tilt_a, tilt_b);
    }
    fast_vec_sub(delta, &moby->pos, start_pos);
    update_entity_matrix(boat_vars->ctl60, delta, start_rot, &moby->rot);
}

extern void FUN_L00_001fff28(void *, int, float);
extern void FUN_L00_0026f080(float *, float *, float, float);

struct PartA500 {
    u8 unk0;
    u8 active;
    u8 pad2[0xE];
    u8 joint[0x30];
};

struct DataA500 {
    u8              pad0[0xF0];
    float           angle[4];
    struct PartA500 parts[4];
};

void update_pokitaru_boat_joint_effects(struct Moby *moby) {
    struct DataA500 *moby_vars = (struct DataA500 *)moby->pvars;
    float            joint_world_position[4];
    float            pos[4];
    float            initial_velocity[4];
    float            speed;
    float            step;
    float            heading;
    int              i;
    int              iterations_per_joint;
    int              j;
    int              k;
    float            life;

    for (i = 0; i < 4; i++) {
        if (!((u8 *)moby_vars)[i * 0x40 + 0x101])
            attach_manipulator(moby, i, &moby_vars->parts[i]);
        step = frame_time * 12.217304f;
        if (moby->state == 1)
            step = frame_time * 2.6179938f;
        if (((u8 *)moby_vars)[i * 0x40 + 0x101]) {
            moby_vars->angle[i] = fast_add_rotations(moby_vars->angle[i], step);
            FUN_L00_001fff28(moby_vars->parts[i].joint, 0, moby_vars->angle[i]);
        }
    }
    if (vector_distance_xy(&moby->pos, camera_pos_b358) > 38.0f)
        return;
    if (moby->state == 1 && (level_frame & 1))
        return;
    if (!(moby->pos.z < 224.5f))
        return;
    for (j = 0; j < 4; j++) {
        moby_point_position(moby, j, joint_world_position);
        iterations_per_joint = moby->state == 1 ? 1 : 2;
        for (k = 0; k < iterations_per_joint; k++) {
            pos[0] = joint_world_position[0] + randf_sym(0.0f, 0.3f);
            pos[1] = joint_world_position[1] + randf_sym(0.0f, 0.3f);
            pos[2] = joint_world_position[2] + randf_sym(0.0f, 0.3f);
            heading = fast_add_rotations(moby->rot.z, randf_sym(0.0f, 0.122173049f) + 3.1415927f);
            speed = random_float_between(2.0f, 5.0f) * frame_time;
            if (moby->state == 1)
                speed *= 0.4f;
            initial_velocity[0] = fast_cos(heading) * speed;
            initial_velocity[1] = fast_sin(heading) * speed;
            initial_velocity[2] = 0.0f;
            initial_velocity[2] = random_float_between(-2.0f, 2.0f) * frame_time;
            life = random_float_between(6300.0f, 14700.0f);
            FUN_L00_0026f080(pos, initial_velocity, life, 222.5f);
        }
    }
}

/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_002CC828.c: func_L11_0030BC20), where it is exact; names translated to the US level program. */
/* no foreign declaration */
extern s32 D_L11_0015FFF4;

int is_pokitaru_boat_target_group_clear(struct Moby *moby) {
    int    moby_group_index = *(short *)((char *)moby->pvars + 0xB4);
    short *group_entry;
    if (moby_group_index < 0 || *(int *)&D_L11_0015FFF4 < moby_group_index)
        return 1;
    group_entry = D_L11_001AC240[moby_group_index];
    if (group_entry == 0)
        return 1;
    do {
        char *group_moby = (char *)(((*group_entry & 0x7FFF) << 8) + D_L11_0015FFD8);
        if (group_moby[0x20] >= 0 && MOBY(group_moby)->oclass == 0x4DE) {
            if (is_moby_in_bob_target_exclusion_state(group_moby) == 0)
                return 0;
        }
    } while (*group_entry++ >= 0);
    return 1;
}
