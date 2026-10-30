/* Level 09 moby classes 0x49e-0x4a5: airship part, its spawner and the nearest-hull search. */
/* Its own unit: it reads the spawn counter D_L09_001613C8 through gp, where update_help_hints in help_hints.c reads it through lui. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "asm.h"
#include "qcopy.h"
#include "sda.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/mobys/animation/get_moby_joint_matrices.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/overlay/quad.h"

extern s32  fptodp(f32) __asm__("FUN_00120478");
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");

typedef struct Data {
    float        vel[4];
    char         pad10[0x10];
    float        f20;
    float        f24;
    short        h28;
    short        h2A;
    struct Moby *parent;
    float        off[4];
    float        f40;
    float        f44;
    float        f48;
    float        f4C;
    float        f50;
    float        f54;
    float        f58;
    float        f5C;
    float        f60;
    float        f64;
    int          f68;
    struct Moby *parts[9];
} Data;

typedef struct {
    char          pad0[0x38];
    unsigned char b38;
    unsigned char b39;
} Flags;

typedef struct {
    int v[2];
} Pair_2c26c8;

/* one scratch slot of the frame: a joint matrix in state 0, two vectors (rows 0 and 1) elsewhere, and the effect id pair in state 4 */
typedef union {
    float       m[4][4];
    Pair_2c26c8 pair;
} Buf;

extern int  debug_print() __asm__("FUN_001e93b0");
extern void update_moby_vertical_oscillation(float, float, char *, float *, float *) __asm__("FUN_L00_00262b00");
extern void update_moby_rotation_oscillation(void *, char *, char *, float, float, float) __asm__("FUN_L00_00262b80");
extern void mark_moby_for_removal_cf(void *, int, int) __asm__("FUN_L00_00257470");
extern void cart_burst(void *, void *, int, float, float) __asm__("FUN_L00_0025f090");
extern char *
spawn_spark(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");
extern int  random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern void spawn_particle(void *, void *, int, int, int, int, int, int, float, float) __asm__("FUN_L00_0026a9f0");
extern float scale_time(float) __asm__("FUN_001f96b0");

extern char                       airship_part_missing_parent_format[] __asm__("D_L09_002082A0");
extern Pair_2c26c8 D_L09_001613D0 NOT_SDA;

#include "rnc/overlay/l09/mobys/airship_part_globals.h"

extern int airship_part_low_altitude_effect_count_gp __asm__("D_L09_00161368") __attribute__((sda));
extern int D_L09_0016136C __attribute__((sda));
extern int D_L09_00161370 __attribute__((sda));
extern float D_L09_00161374 __attribute__((sda));
extern float D_L09_00161378 __attribute__((sda));
extern float D_L09_0016137C __attribute__((sda));
extern float D_L09_00161380 __attribute__((sda));
extern int   D_L09_00161384 __attribute__((sda));
extern int   D_L09_00161388 __attribute__((sda));
extern int   D_L09_0016138C __attribute__((sda));
extern float D_L09_00161390 __attribute__((sda));
extern float D_L09_00161394 __attribute__((sda));

#include "rnc/overlay/l09/mobys/airship_part_data/update_airship_part_3.h"

extern float airship_part_initial_tumble_x_speed_limit_degrees_gp __asm__("D_L09_001613B0")
    __attribute__((sda));
extern float airship_part_initial_tumble_y_speed_limit_degrees_gp __asm__("D_L09_001613B4")
    __attribute__((sda));

#include "rnc/overlay/l09/mobys/airship_part_data/update_airship_part_2.h"

extern float airship_hull_breakup_wait_x_rotation_speed_degrees_gp __asm__("D_L09_001613BC")
    __attribute__((sda));

#include "rnc/overlay/l09/mobys/airship_part_data/update_airship_part.h"
#include "rnc/globals.h"

extern f32 frame_time_sq_macro __asm__("D_0015ED70") MACRO_ADDR;
extern int D_L09_001613C8 __attribute__((sda));
/* Spawns an airship part of a class at a point, placed and rotated with the hull. */
/* Ported from rac1-decomp (src/overlays/l09_gaspar/vendor_002C2B08.c: func_L09_002C3690), where it is exact; names translated to the US level program. */
char *spawn_airship_part(struct Moby *rotation_source_moby,
                         char        *spawn_reference_position,
                         int          spawned_moby_class) __asm__("FUN_L09_002c2500");

/* Returns the moby of a class nearest to a point, and its distance. */
/* Ported from rac1-decomp (src/overlays/l09_gaspar/vendor_002C2B08.c: func_L09_002C37A0), where it is exact; names translated to the US level program. */
char *find_nearest_moby_by_class(void  *search_position,
                                 int    type,
                                 float *nearest_distance_out) __asm__("FUN_L09_002c2610");

char *spawn_airship_part(struct Moby *rotation_source_moby,
                         char        *spawn_reference_position,

                         int spawned_moby_class) {
    char *spawned_moby = create_moby(spawned_moby_class);
    if (spawned_moby) {
        OvlQuad point_position;
        OvlQuad position_offset;
        char   *spawned_position = spawned_moby + 0x10;
        char   *spawned_vars;
        ((unsigned char *)spawned_moby)[0x30] = 0xFF;
        MOBY(spawned_moby)->draw_dist = 0xFF;
        spawned_vars = ((char *)MOBY(spawned_moby)->pvars);
        spawned_moby[0x31] = 1;
        spawned_moby[0x20] = 1;
        qcopy(spawned_position, spawn_reference_position);
        qcopy(spawned_vars + 0x10, spawn_reference_position);
        *(char **)(spawned_vars + 0x2C) = rotation_source_moby;
        update_entity(spawned_moby);
        moby_point_position(spawned_moby, 0, &point_position);
        fast_vec_sub(&position_offset, spawn_reference_position, &point_position);
        transform_vector_by_basis(
            &position_offset, &position_offset, &rotation_source_moby->rot_row0);
        fast_vec_add(spawned_position, spawn_reference_position, &position_offset);
        qcopy(spawned_moby + 0x40, &rotation_source_moby->rot);
        *(OvlQuad *)&MOBY(spawned_moby)->rot_row0 = *(OvlQuad *)&rotation_source_moby->rot_row0;
        *(OvlQuad *)&MOBY(spawned_moby)->rot_row1 = *(OvlQuad *)&rotation_source_moby->rot_row1;
        *(OvlQuad *)&MOBY(spawned_moby)->rot_row2 = *(OvlQuad *)&rotation_source_moby->rot_row2;
        update_entity(spawned_moby);
    }
    return spawned_moby;
}

char *find_nearest_moby_by_class(void *search_position, int type, float *nearest_distance_out) {
    char *best = 0;
    float bestd = 100000.0f;
    char *candidate_moby;
    for (candidate_moby = D_L09_0015FFE4; candidate_moby != 0;
         candidate_moby = ((char *)MOBY(candidate_moby)->next)) {
        if (MOBY(candidate_moby)->oclass == type) {
            float candidate_distance = fast_vec_dist(search_position, candidate_moby + 0x10);
            if (candidate_distance < bestd) {
                bestd = candidate_distance;
                best = candidate_moby;
            }
        }
    }
    if (nearest_distance_out != 0) {
        *nearest_distance_out = bestd;
    }
    return best;
}

/* Update for the airship parts (classes 0x49E-0x4A5): attach to the hull, ride it, then break off, tumble away and fall with smoke and sparks. */
void update_airship_part(struct Moby *moby) __asm__("FUN_L09_002c26c8");

void update_airship_part(struct Moby *moby) {
    enum {
        AIRSHIP_PART_STATE_INITIALIZE = 0,
        AIRSHIP_PART_STATE_WAIT_FOR_BREAKUP_COMMAND = 1,
        AIRSHIP_PART_STATE_WAIT_FOR_BREAKUP_GATE = 2,
        AIRSHIP_PART_STATE_BEGIN_BREAKUP = 3,
        AIRSHIP_PART_STATE_FALLING_WITH_EFFECTS = 4,
        AIRSHIP_PART_STATE_FALLING_AFTER_DAMPING = 5
    };
    float point_offset_and_effect_scratch[4];
    Buf   transform_and_effect_scratch;
    float rotm[4][4];
    float nearest_parent_distance;
    int   joint;
    Data *part_vars = (*(Data **)&moby->pvars);

    D_L09_001613C8++;
    switch (moby->state) {
    case AIRSHIP_PART_STATE_INITIALIZE: {
        float joint_offset_scale;

        moby_point_position(moby, 0, point_offset_and_effect_scratch);
        if (part_vars->f68 != -1 &&
            ((Flags *)(alternate_item_available + part_vars->f68))->b39 != 0) {
            mark_moby_for_removal(moby);
            return;
        }
        moby->draw_dist = 0x3FF;
        if (moby->oclass == 0x4A0) {
            if (part_vars->h28 != 0) {
                int i;
                moby->draw_dist = 0xFF;
                part_vars->parent = 0;
                for (i = 0x49E; i < 0x4A6; i++) {
                    if (i != 0x4A0) {
                        spawn_airship_part(
                            (char *)moby, (char *)point_offset_and_effect_scratch, i);
                    }
                }
            }
        } else {
            struct Moby *p;
            nearest_parent_distance = 100000.0f;
            p = (struct Moby *)find_nearest_moby_by_class(
                ((f32 *)&moby->pos), 0x4A0, &nearest_parent_distance);
            if (nearest_parent_distance < 20.0f) {
                part_vars->parent = p;
                (*(Data **)&p->pvars)->parts[moby->oclass - 0x49E] = moby;
            } else {
                debug_print(airship_part_missing_parent_format,
                            (*(short *)&moby->save_id),
                            moby->oclass,
                            fptodp(nearest_parent_distance));
            }
        }
        joint = 0;
        joint_offset_scale = moby->scale * 0.0009765625f;
        get_moby_joint_matrices(moby, 1, &joint, transform_and_effect_scratch.m);
        fast_vec_scale(part_vars->off, transform_and_effect_scratch.m[3], joint_offset_scale);
        moby->state = 1;
        break;
    }
    case AIRSHIP_PART_STATE_WAIT_FOR_BREAKUP_COMMAND:
    case AIRSHIP_PART_STATE_WAIT_FOR_BREAKUP_GATE: {
        if (moby->oclass == 0x4A0) {
            update_moby_vertical_oscillation(part_vars->f48,
                                             part_vars->f4C * 0.017453292f * frame_time,
                                             (char *)moby,
                                             &part_vars->f40,
                                             &part_vars->f44);
            update_moby_rotation_oscillation(moby,
                                             (char *)&part_vars->f50,
                                             (char *)&part_vars->f54,
                                             part_vars->f58 * 0.017453292f,
                                             part_vars->f5C * 0.017453292f * frame_time,
                                             part_vars->f60 * 0.017453292f * frame_time);
        }
        if (moby->oclass != 0x4A0 && part_vars->parent != 0 && part_vars->parent->state != 3) {
            struct Moby *p = part_vars->parent;
            qcopy(((f32 *)&moby->rot), ((f32 *)&p->rot));
            euler_rotation_matrix(rotm, ((f32 *)&p->rot));
            transform_vector(point_offset_and_effect_scratch, part_vars->off, rotm);
            moby_point_position(part_vars->parent, 0, transform_and_effect_scratch.m[0]);
            fast_vec_sub(((f32 *)&moby->pos),
                         transform_and_effect_scratch.m[0],
                         point_offset_and_effect_scratch);
        }
        if (part_vars->parent != 0) {
            moby->flags &= ~1;
            moby->flags |= part_vars->parent->flags & 1;
            moby->command = part_vars->parent->command;
        }
        if (moby->state == 2) {
            if (moby->oclass == 0x4A0) {
                moby->rot.x =
                    fast_add_rotations(moby->rot.x,
                                       airship_hull_breakup_wait_x_rotation_speed_degrees_gp *
                                           0.017453292f * frame_time);
            }
            if (fast_dec_timer_s16(&part_vars->h2A) != 0) {
                moby->state = 3;
            }
        } else if (moby->command == 1) {
            int i;
            moby->update_dist = 0xFF;
            part_vars->h2A =
                random_int_between(scale_game_frames(airship_part_breakup_delay_range_start_gp),
                                   scale_game_frames(airship_part_breakup_delay_range_end_gp));
            for (i = 0; i < 9; i++) {
                if (part_vars->parts[i] != 0) {
                    part_vars->parts[i]->update_dist = 0xFF;
                }
            }
            moby->state = 2;
        }
        break;
    }
    case AIRSHIP_PART_STATE_BEGIN_BREAKUP: {
        fast_dec_timer_s16(&part_vars->h2A);
        ((Flags *)(alternate_item_available))->b38 = 1;
        if (part_vars->f68 != -1) {
            ((Flags *)(alternate_item_available + part_vars->f68))->b39 = 1;
        }
        mark_moby_for_removal_cf(moby, 0x200, -1);
        cart_burst(moby, ((f32 *)&moby->pos), -1, 4.0f, 13.0f);
        if (moby->oclass == 0x4A0) {
            allocate_voice_for_target_entry(2, 0, (int)moby);
        }
        moby_point_position(moby, 0, point_offset_and_effect_scratch);
        fast_vec_sub(part_vars->vel, ((f32 *)&moby->pos), point_offset_and_effect_scratch);
        fast_vec_scale(part_vars->vel,
                       part_vars->vel,
                       airship_part_breakup_offset_velocity_scale_gp * frame_time / 20.0f);
        fast_vec_sub(
            transform_and_effect_scratch.m[0], point_offset_and_effect_scratch, &hero.motion.pos);
        if (moby->oclass >= 0x4A1 && moby->oclass <= 0x4A2) {
            fast_vec_normalize(transform_and_effect_scratch.m[0],
                               transform_and_effect_scratch.m[0],
                               airship_part_breakup_away_from_hero_speed_override_gp * frame_time);
        } else {
            fast_vec_normalize(transform_and_effect_scratch.m[0],
                               transform_and_effect_scratch.m[0],
                               airship_part_breakup_away_from_hero_speed_gp * frame_time);
        }
        fast_vec_add(part_vars->vel, part_vars->vel, transform_and_effect_scratch.m[0]);
        part_vars->vel[2] = 0.0f;
        part_vars->f20 = random_float_between(
            -(airship_part_initial_tumble_x_speed_limit_degrees_gp * 0.017453292f * frame_time),
            airship_part_initial_tumble_x_speed_limit_degrees_gp * 0.017453292f * frame_time);
        part_vars->f24 = random_float_between(
            -(airship_part_initial_tumble_y_speed_limit_degrees_gp * 0.017453292f * frame_time),
            airship_part_initial_tumble_y_speed_limit_degrees_gp * 0.017453292f * frame_time);
        part_vars->h2A = random_float_between(scale_game_frames(0x5A), scale_game_frames(0x96));
        part_vars->f64 = random_float_between(
            airship_part_initial_downward_acceleration_range_start_gp * frame_time_sq_macro,
            airship_part_initial_downward_acceleration_range_end_gp * frame_time_sq_macro);
        if (moby->oclass >= 0x4A1 && moby->oclass <= 0x4A2) {
            part_vars->f20 = part_vars->f20 / part_vars->f20 * (1.5707964f / part_vars->h2A);
            part_vars->f64 = random_float_between(
                airship_part_downward_acceleration_override_range_start_gp * frame_time_sq_macro,
                airship_part_downward_acceleration_override_range_end_gp * frame_time_sq_macro);
        }
        moby->collision = 0;
        moby->state = 4;
        break;
    }
    case AIRSHIP_PART_STATE_FALLING_WITH_EFFECTS:
        part_vars->vel[2] -= part_vars->f64;
        fast_vec_add(((f32 *)&moby->pos), ((f32 *)&moby->pos), part_vars->vel);
        moby->rot.x = fast_add_rotations(moby->rot.x, part_vars->f20);
        moby->rot.y = fast_add_rotations(moby->rot.y, part_vars->f24);
        if (moby->pos.x < 12.0f || moby->pos.x > 1011.0f || moby->pos.y < 12.0f ||
            moby->pos.y > 1011.0f || moby->pos.z < 12.0f || moby->pos.z > 1011.0f) {
            mark_moby_for_removal(moby);
            return;
        }
        if (random_integer_below(3) == 0) {
            clear_u64_value(point_offset_and_effect_scratch);
            transform_and_effect_scratch.pair = D_L09_001613D0;
            spawn_particle(((f32 *)&moby->pos),
                           point_offset_and_effect_scratch,
                           0x2F3F3F7F,
                           transform_and_effect_scratch.pair.v[random_integer_below(2)],
                           random_int_between(scale_game_frames(0xF), scale_game_frames(0x14)),
                           random_int_between(scale_game_frames(0x19), scale_game_frames(0x1E)),
                           0,
                           0,
                           400000.0f,
                           random_float_between(8.0f, 16.0f) * frame_time);
        }
        if (moby->pos.z < 25.0f) {
            int i;
            for (i = 0; i < airship_part_low_altitude_effect_count_gp; i++) {
                float effect_direction_angle = random_angle_radians();
                qcopy(transform_and_effect_scratch.m[1], ((f32 *)&moby->pos));
                transform_and_effect_scratch.m[1][0] += random_float_between(-3.0f, 3.0f);
                transform_and_effect_scratch.m[1][1] += random_float_between(-3.0f, 3.0f);
                point_offset_and_effect_scratch[2] = D_L09_00161374 * frame_time;
                point_offset_and_effect_scratch[0] +=
                    fast_cos(effect_direction_angle) * D_L09_0016137C;
                point_offset_and_effect_scratch[1] +=
                    fast_sin(effect_direction_angle) * D_L09_0016137C;
                transform_and_effect_scratch.m[0][2] = D_L09_00161378 * frame_time;
                transform_and_effect_scratch.m[0][0] +=
                    fast_cos(effect_direction_angle) * D_L09_00161380;
                transform_and_effect_scratch.m[0][1] +=
                    fast_sin(effect_direction_angle) * D_L09_00161380;
                point_offset_and_effect_scratch[3] = D_L09_00161390;
                transform_and_effect_scratch.m[0][3] = D_L09_00161394;
                spawn_spark(transform_and_effect_scratch.m[1],
                            point_offset_and_effect_scratch,
                            transform_and_effect_scratch.m[0],
                            D_L09_0016136C,
                            D_L09_00161370,
                            truncate_float_to_s32(scale_time(
                                random_float_between(D_L09_00161384, D_L09_00161384 * 2))),
                            truncate_float_to_s32(scale_time(
                                random_float_between(D_L09_00161388, D_L09_00161388 * 2))),
                            truncate_float_to_s32(scale_time(
                                random_float_between(D_L09_0016138C, D_L09_0016138C * 2))),
                            -1);
            }
            fast_vec_scale(part_vars->vel, part_vars->vel, 0.45f);
            part_vars->f20 *= 0.9f;
            part_vars->f24 *= 0.9f;
            allocate_voice_for_target_entry(0, 0, (int)moby);
            moby->state = 5;
        }
        break;
    case AIRSHIP_PART_STATE_FALLING_AFTER_DAMPING:
        part_vars->vel[2] -= frame_time_sq_macro * 10.8f / 3.0f;
        fast_vec_add(((f32 *)&moby->pos), ((f32 *)&moby->pos), part_vars->vel);
        moby->rot.x = fast_add_rotations(moby->rot.x, part_vars->f20);
        moby->rot.y = fast_add_rotations(moby->rot.y, part_vars->f24);
        if (moby->pos.x < 12.0f || moby->pos.x > 1011.0f || moby->pos.y < 12.0f ||
            moby->pos.y > 1011.0f || moby->pos.z < 12.0f || moby->pos.z > 1011.0f) {
            mark_moby_for_removal(moby);
        }
        break;
    }
}

Pair_2c26c8 D_L09_001613D0 = {
    {0x2f000000, 0x3f000000}
};
