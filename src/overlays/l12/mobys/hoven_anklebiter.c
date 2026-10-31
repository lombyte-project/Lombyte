/* Level 12 moby class hoven_anklebiter: its suck cannon callbacks, pursuit, movement, sparks, damage and alerts. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/overlay/shared/mobys/spawn_type_52_particle_near_moby.h"
#include "rnc/overlay/shared/mobys/update_object_wander_movement.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/math/interpolation.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/overlay/l12/mobys/hoven_anklebiter.h"

/* Suck cannon suction method: state 8 while sucked (alerting its group), back to 1 when released; no suction in states 9, 11 and 13. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002E29C8), where it is exact; names translated to the US level program. */

extern int  process_moby_suck_cannon_suction(void *) __asm__("FUN_L00_002db8f8");
extern void FUN_L01_0026e090(int, int);

int process_hoven_anklebiter_suck_cannon_suction(struct Moby *moby) __asm__("FUN_L12_002e16b8");

int process_hoven_anklebiter_suck_cannon_suction(struct Moby *moby) {
    int active;
    if (moby->state == 9 || moby->state == 11 || moby->state == 13)
        return 0;
    active = process_moby_suck_cannon_suction(moby);
    if (active) {
        if (moby->group != 0xFF)
            FUN_L01_0026e090(moby->group, 1);
        moby->state = 8;
    } else if (moby->state == 8) {
        moby->state = 1;
    }
    return active;
}

extern int store_moby_in_suck_cannon(void *) __asm__("FUN_L00_002dbb20");

/* Suck cannon store method: state 8 while the suck cannon holds the moby, back to 1 when it lets go. */
int store_hoven_anklebiter_in_suck_cannon(struct Moby *a) __asm__("FUN_L12_002e1758");

int store_hoven_anklebiter_in_suck_cannon(struct Moby *carried_moby) {
    int suck_cannon_store_result = store_moby_in_suck_cannon(carried_moby);
    if (suck_cannon_store_result == 0) {
        if (carried_moby->state == 0x8)
            carried_moby->state = 1;
    } else {
        carried_moby->state = 0x8;
    }
    return suck_cannon_store_result;
}

/* Suck cannon cancel method: cancels the suction and moves the moby to state 8 unless it is in state 9 or 11. */
/* Ported from rac1-decomp (src/overlays/l12/vendor_002C0310.c: func_L12_002E2B08), where it is exact; names translated to the US level program. */

extern int cancel_suck_cannon_suction(struct Moby *) __asm__("FUN_L00_002dbe20");

int cancel_hoven_anklebiter_suck_cannon_suction(struct Moby *moby) __asm__("FUN_L12_002e17f8");

int cancel_hoven_anklebiter_suck_cannon_suction(struct Moby *moby) {
    int was_stored_in_suck_cannon = cancel_suck_cannon_suction(moby);
    if (moby->state == 9 || moby->state == 11) {
        return 0;
    }
    moby->state = 8;
    return was_stored_in_suck_cannon;
}

extern char D_L12_00167240[];

#include "rnc/overlay/level_path.h"

#include "sda.h"

#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002E2B88), where it is exact; names translated to the US level program. */

typedef union {
    long long quad;
    float     f[4];
} L12Vector;

void ease_hoven_anklebiter_height_to_ground(char *moby) __asm__("FUN_L12_002e1878");

/* Moves the moby along its path at the given speed and keeps it on the path; returns the move result, or 2 when it has been stuck too long. */

extern void ease_hoven_anklebiter_height_to_ground(char *) __asm__("FUN_L12_002e1878");

void ease_hoven_anklebiter_height_to_ground(char *moby) {
    L12Vector position;
    float     probed_ground_height;
    qcopy(&position, moby + 0x10);
    position.f[2] += 0.5f;
    probed_ground_height = probe_ground_height(&position, 0, 0.5f);
    approach_value(probed_ground_height, frame_time_sq * 27.0f, moby + 0x18);
}

/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002E2BF8), where it is exact; names translated to the US level program. */

extern int random_integer_below(int);
extern int scale_game_frames(int);

void select_hoven_anklebiter_pursuit_animation(struct Moby *moby) __asm__("FUN_L12_002e18e8");

/* Updates a Hoven anklebiter's (moby class 238) idle, pursuit, melee, leap, landing and death states. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002E2EF0), where it is exact; names translated to the US level program. */

typedef struct Moby Moby;

void select_hoven_anklebiter_pursuit_animation(struct Moby *moby) {
    enum {
        ANKLEBITER_PURSUIT_ANIMATION_VARIANT_A = 0,
        ANKLEBITER_PURSUIT_ANIMATION_VARIANT_B = 1
    };
    switch (random_integer_below(2)) {
    case ANKLEBITER_PURSUIT_ANIMATION_VARIANT_A:
        if (moby->prev_seq != 3)
            blend_moby_animation(moby, 3, 0, scale_game_frames(8));
        break;
    case ANKLEBITER_PURSUIT_ANIMATION_VARIANT_B:
        if (moby->prev_seq != 0xF)
            blend_moby_animation(moby, 0xF, 0, scale_game_frames(8));
        break;
    default:
        if (moby->prev_seq != 0x10)
            blend_moby_animation(moby, 0x10, 0, scale_game_frames(8));
        break;
    }
}

#include "rnc/overlay/quad.h"

/* Scatters 27 particles around a point with random offsets and velocities. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002E2CB8), where it is exact; names translated to the US level program. */

extern char *
spawn_spark(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");

#include "rnc/overlay/l12/mobys/hoven_anklebiter_globals.h"

extern float spark_scatter_vertical_z_range_start_gp __asm__("D_L12_00161888") __attribute__((sda));
extern float spark_scatter_vertical_z_range_end_gp __asm__("D_L12_0016188C") __attribute__((sda));
extern float spark_scatter_radial_z_range_start_gp __asm__("D_L12_00161890") __attribute__((sda));
extern float spark_scatter_radial_z_range_end_gp __asm__("D_L12_00161894") __attribute__((sda));
extern float spark_scatter_radial_xy_scale_range_start_gp __asm__("D_L12_00161898")
    __attribute__((sda));
extern float spark_scatter_radial_xy_scale_range_end_gp __asm__("D_L12_0016189C")
    __attribute__((sda));
extern float spark_scatter_vertical_vector_w_gp __asm__("D_L12_001618A0") __attribute__((sda));
extern float spark_scatter_radial_vector_w_gp __asm__("D_L12_001618A4") __attribute__((sda));
extern int spark_scatter_frame_count_range_start_gp __asm__("D_L12_001618AC") __attribute__((sda));
extern int spark_scatter_frame_count_range_end_gp __asm__("D_L12_001618B0") __attribute__((sda));
extern int spark_scatter_second_frame_count_range_start_gp __asm__("D_L12_001618B4")
    __attribute__((sda));
extern int spark_scatter_second_frame_count_range_end_gp __asm__("D_L12_001618B8")
    __attribute__((sda));
extern int spark_scatter_third_frame_count_range_start_gp __asm__("D_L12_001618BC")
    __attribute__((sda));
extern int spark_scatter_third_frame_count_range_end_gp __asm__("D_L12_001618C0")
    __attribute__((sda));
int         D_L12_001618C4 = 0xa0f0e8e0;
int         D_L12_001618C8 = 0x20f0e8e0;
int         D_L12_001618CC = 0x10017;
void        spawn_spark_scatter(void *spark_scatter_origin) __asm__("FUN_L12_002e19a8");
void        spawn_spark_scatter(void *pos) __asm__("FUN_L12_002e19a8");
extern void spawn_spark_scatter(void *) __asm__("FUN_L12_002e19a8");

void spawn_spark_scatter(void *spark_scatter_origin) {
    float base_position[4];
    float spark_position[4];
    float vertical_spark_vector[4];
    float radial_spark_vector[4];
    float radial_angle_radians;
    int   i;
    int   frame_count, second_frame_count, third_frame_count;
    *(OvlQuad *)base_position = *(OvlQuad *)spark_scatter_origin;
    i = 0x1A;
    do {
        radial_angle_radians = random_angle_radians();
        qcopy(spark_position, base_position);
        spark_position[2] = spark_position[2] + random_float_between(-0.25f, -0.75f);
        spark_position[0] = spark_position[0] + random_float_between(-0.25f, 0.25f);
        spark_position[1] = spark_position[1] + random_float_between(-0.25f, 0.25f);
        clear_u64_value(vertical_spark_vector);
        radial_spark_vector[0] = fast_cos(radial_angle_radians) *
                                 (random_float_between(spark_scatter_radial_xy_scale_range_start_gp,
                                                       spark_scatter_radial_xy_scale_range_end_gp) *
                                  frame_time);
        radial_spark_vector[1] = fast_sin(radial_angle_radians) *
                                 (random_float_between(spark_scatter_radial_xy_scale_range_start_gp,
                                                       spark_scatter_radial_xy_scale_range_end_gp) *
                                  frame_time);
        *(int *)&radial_spark_vector[2] = 0;
        vertical_spark_vector[2] = random_float_between(spark_scatter_vertical_z_range_start_gp,
                                                        spark_scatter_vertical_z_range_end_gp) *
                                   frame_time;
        radial_spark_vector[2] = random_float_between(spark_scatter_radial_z_range_start_gp,
                                                      spark_scatter_radial_z_range_end_gp) *
                                 frame_time;
        vertical_spark_vector[3] = spark_scatter_vertical_vector_w_gp;
        radial_spark_vector[3] = spark_scatter_radial_vector_w_gp;
        frame_count = truncate_float_to_s32(
            random_float_between((float)spark_scatter_frame_count_range_start_gp,
                                 (float)spark_scatter_frame_count_range_end_gp));
        second_frame_count = truncate_float_to_s32(
            random_float_between((float)spark_scatter_second_frame_count_range_start_gp,
                                 (float)spark_scatter_second_frame_count_range_end_gp));
        third_frame_count = truncate_float_to_s32(
            random_float_between((float)spark_scatter_third_frame_count_range_start_gp,
                                 (float)spark_scatter_third_frame_count_range_end_gp));
        spawn_spark(spark_position,
                    vertical_spark_vector,
                    radial_spark_vector,
                    D_L12_001618C4,
                    D_L12_001618C8,
                    frame_count,
                    second_frame_count,
                    third_frame_count,
                    D_L12_001618CC);
    } while (--i >= 0);
}

#include "rnc/math/vector.h"

extern unsigned char D_0013F3D0_r[] __asm__("D_0013F3D0");
int                  D_L12_001618E8 = 0xa030303;
extern float         D_0015ED64;
extern float         D_0015ED6C;
extern float         D_0015ED70;
extern float hoven_anklebiter_leap_launch_speed_gp __asm__("D_L12_001618D8") __attribute__((sda));
extern float hoven_anklebiter_leap_gravity_gp __asm__("D_L12_001618DC") __attribute__((sda));
extern void
    spawn_nearby_effect(void *, int, int, void *, void *, float) __asm__("FUN_L00_00259bc8");
f32 compute_interpolated_record_value(void *anim) __asm__("FUN_0020c9e0");
int step_moby_motion(void *moby, void *motion) __asm__("FUN_L00_0025c698");
extern char D_0013E633_2E2EF0[] __asm__("D_001413D0");
extern void process_hoven_anklebiter_damage_and_alerts(struct Moby *) __asm__("FUN_L12_002e3528");
extern int  FUN_L01_00274b78_c(char *, char *, float) __asm__("FUN_L01_00274b78");
extern void func_L01_0026E8E0_2E2EF0(char *) __asm__("FUN_L01_0026d930");
extern int  random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern int  rand(void) __asm__("FUN_001160d8");
extern int  func_L00_0025A208_2E2EF0(int *, int, int, int) __asm__("FUN_L00_002591d0");
extern int  func_L00_0025A2F0_2E2EF0(int *, int, int, int) __asm__("FUN_L00_002592b8");
extern void func_L01_0026F040_2E2EF0(int, int) __asm__("FUN_L01_0026e090");
extern void approach_w(char *, float *, float, float, float, float) __asm__("FUN_L00_00258278");
extern void select_hoven_anklebiter_pursuit_animation(struct Moby *) __asm__("FUN_L12_002e18e8");
extern float
func_L00_0025BC48_2E2EF0(void *, void *, void *, float, float) __asm__("FUN_L00_0025abf0");
extern void  setup_fall54(float, char *, float *, int, int, int) __asm__("FUN_L00_0025c558");
extern int   step_moby_motion(void *, void *) __asm__("FUN_L00_0025c698");
extern int   move_hoven_anklebiter_toward_heading(struct Moby *, float) __asm__("FUN_L12_002e2eb8");
extern int   is_value_within_interpolated_window(char *, float) __asm__("FUN_00214cc8");
extern int   point_in_path_xy(void *, void *, int) __asm__("FUN_L00_00259740");
extern int   func_L00_002DDEA0_2E2EF0(void *, void *) __asm__("FUN_L00_002dc9f0");
extern float compute_interpolated_record_value(void *) __asm__("FUN_0020c9e0");
extern void  mark_moby_for_removal_cf(void *, int, int) __asm__("FUN_L00_00257470");
extern void  spawn_hoven_anklebiter_death_sparks(struct Moby *) __asm__("FUN_L12_002e3098");
extern void func_L00_00263950_2E2EF0(char *, char *, int, float, float) __asm__("FUN_L00_002628d8");
extern void func_L00_0025B178_2E2EF0(void *) __asm__("FUN_L00_0025a120");

void hoven_anklebiter_update(AnklebiterMoby *moby) __asm__("FUN_L12_002e1be0");

#include "rnc/overlay/level_globals.h"

extern float fast_subtract_rotations(float, float);

/* Moby class 238 */
void hoven_anklebiter_update(AnklebiterMoby *moby) {
    enum {
        ANKLEBITER_INITIALIZE = 0,
        ANKLEBITER_IDLE = 1,
        ANKLEBITER_FACE_TARGET_BEFORE_PURSUIT = 3,
        ANKLEBITER_PURSUE_TARGET = 4,
        ANKLEBITER_MELEE_ATTACK = 5,
        ANKLEBITER_RETURN_HOME = 6,
        ANKLEBITER_KNOCKBACK_FALL = 7,
        ANKLEBITER_CONTROLLED_MOTION_UNTIL_RETURN_HOME = 8,
        ANKLEBITER_DORMANT = 9,
        ANKLEBITER_RECOVER_AFTER_KNOCKBACK = 10,
        ANKLEBITER_WAIT_FOR_LEAP_TRIGGER = 11,
        ANKLEBITER_LEAP_TO_HOME = 12,
        ANKLEBITER_ACTIVATE_FROM_DORMANCY = 13,
        ANKLEBITER_LAND_AFTER_LEAP = 14,
        ANKLEBITER_DEATH_EFFECTS_AND_REMOVE = 15,
        ANKLEBITER_DEATH_FALL = 99
    };
    TargetReport    target_report_or_vector_scratch;
    AnklebiterMoby *iterator;
    float           trajectory;
    AnklebiterData *data = moby->data;
    AnklebiterMoby *target;
    float           angle;
    int             count, timer_range_min_frames, blocked, hit;
    process_hoven_anklebiter_damage_and_alerts((struct Moby *)moby);
    manipulator_set_scale(data->collision_data, 2.2f);
    FUN_L01_00274b78_c((char *)moby, (char *)&target_report_or_vector_scratch, data->range);
    target = target_report_or_vector_scratch.moby;
    if (!target)
        target = *(AnklebiterMoby **)D_0013E633_2E2EF0;
    if (moby->pos[2] < 26.0f) {
        mark_moby_for_removal(moby);
        return;
    }
    switch (moby->state) {
    case ANKLEBITER_INITIALIZE:
        if (data->sleep) {
            qcopy(data->home, moby->pos);
            moby->state = 9;
            if (moby->anim != 12)
                blend_moby_animation(moby, 12, 0, 3);
            moby->collision = 0;
            moby->flags = (moby->flags | 0x41) & 0xEFFF;
        } else if (data->home[3] != 0.0f) {
            moby->state = 11;
            moby->joints = 0;
            moby->flags &= 0xEFFF;
            moby->collision = 0;
        } else {
            qcopy(data->home, moby->pos);
            moby->state = 1;
            if (moby->anim != 0)
                blend_moby_animation(moby, 0, 0, 3);
        }
        {
            int initial_mode =
                1; /* loaded ahead of the other constants: it is the last one stored (retail keeps it in $a2 into the call's delay slot) */
            data->alert_timer = 0;
            data->init0 = 0;
            data->damping = 0.5f;
            data->health = 2.0f;
            data->blend = 8;
            data->count = 5;
            data->mode = initial_mode;
            data->fall_flags = 0x200;
            func_L01_0026E8E0_2E2EF0((char *)&data->effect_class);
            data->effect_class = 0x38D;
            data->effect_flags |= 0x10;
            data->effect_speed = D_0015ED6C * 5.5f;
            data->effect_size1 = 0.5f;
            data->effect_size0 = 0.5f;
            if (data->group_enabled) {
                timer_range_min_frames = scale_game_frames(20);
                data->group_timer =
                    random_int_between(timer_range_min_frames, scale_game_frames(170));
            }
            qcopy(data->idle_pos, moby->pos);
            timer_range_min_frames = scale_game_frames(120);
            data->idle_timer = random_int_between(timer_range_min_frames, scale_game_frames(700));
            qcopy(target_report_or_vector_scratch.v0, moby->pos);
            target_report_or_vector_scratch.v0[2] += 0.8f;
            moby->pos[2] = probe_ground_height(target_report_or_vector_scratch.v0, 0, 0.5f);
            if (rand() & 1)
                moby->flags |= 0x8000;
            data->tuning = &D_L12_001618E8;
        }
        break;
    case ANKLEBITER_IDLE: {
        float idle_distance, idle_angle, idle_difference;
        idle_distance = fast_vec_dist(target->pos, data->home);
        idle_angle = fast_arc_tan(target->pos[0] - moby->pos[0], target->pos[1] - moby->pos[1]);
        idle_difference = fast_difference_between_rotations(moby->rot[2], idle_angle);
        if (idle_distance < data->range && absolute_float(moby->pos[2] - target->pos[2]) < 4.0f) {
            moby->state = 3;
            if (idle_difference < 0.9599311f) {
                if (moby->anim != 2)
                    blend_moby_animation(moby, 2, 0, scale_game_frames(5));
                moby->anim_rate = 1.55f;
            } else if (moby->anim != 17)
                blend_moby_animation(moby, 17, 0, scale_game_frames(5));
        } else if (((moby->proximity & 1) && random_integer_below(19) == 0) ||
                   moby->proximity == 7) {
            if (random_integer_below(256) & 1)
                data->circle_rate = -data->circle_rate;
            data->circle_angle = data->circle_rate * 0.017453292f;
            data->circle_timer = scale_game_frames((int)random_float_between(30.0f, 90.0f));
            moby->state = 3;
            if (moby->anim != 2)
                blend_moby_animation(moby, 2, 0, scale_game_frames(7));
            moby->proximity = 0;
        } else {
            if (moby->anim != 15)
                blend_moby_animation(moby, 15, 0, scale_game_frames(14));
            count = data->idle_timer;
            if (count > scale_game_frames(20))
                approach_value(D_0015ED6C * 2.5f, D_0015ED70 * 7.0f, &data->idle_speed);
            else
                approach_value(0.0f, D_0015ED70 * 7.0f, &data->idle_speed);
            moby->anim_rate = 0.8f;
            update_object_wander_movement((char *)moby, (char *)data->idle_pos, 0.35f, 0.25f);
        }
        break;
    }
    case ANKLEBITER_DORMANT:
        if (data->group_enabled) {
            if (vector_distance_xy(moby->pos, target->pos) < 2.0f)
                break;
            if (data->group_check < D_L12_0015F5CC) {
                data->group_check = D_L12_0015F5CC;
                count = 0;
                func_L00_0025A208_2E2EF0((int *)&iterator, moby->group, 0, 0);
                while (iterator) {
                    if ((signed char)iterator->state >= 0 && iterator->state != 9)
                        count++;
                    func_L00_0025A2F0_2E2EF0((int *)&iterator, (int)iterator, 0, 0);
                }
                *data->group_count = count;
            }
            if (*data->group_count >= 6)
                break;
        }
        fast_vec_sub(target_report_or_vector_scratch.v0, target->pos, moby->pos);
        if (fast_vec_length(target_report_or_vector_scratch.v0) < data->range &&
            absolute_float(moby->pos[2] - target->pos[2]) < 8.0f && random_integer_below(19) == 0) {
            if (moby->group != 255)
                func_L01_0026F040_2E2EF0(moby->group, 1);
        } else if (!((moby->proximity & 1) && fast_dec_timer_s16(&data->group_timer)) &&
                   moby->proximity != 7)
            break;
        moby->state = 13;
        if (moby->anim != 7)
            blend_moby_animation(moby, 7, 0, 1);
        moby->flags |= 0x1000;
        *(u128 *)target_report_or_vector_scratch.v10 = *(u128 *)moby->pos;
        spawn_spark_scatter(target_report_or_vector_scratch.v10);
        break;
    case ANKLEBITER_ACTIVATE_FROM_DORMANCY: {
        int collision = *(int *)(moby->model + 0x10);
        moby->flags &= 0xFFBE;
        moby->collision = collision;
        angle = fast_arc_tan(target->pos[0] - moby->pos[0], target->pos[1] - moby->pos[1]);
        approach_w((char *)moby, &data->turn_velocity, angle, 0.03f, 0.3f, D_0015ED6C * 6.2831855f);
        if (moby->anim_flags & 2) {
            if (random_integer_below(256) & 1)
                data->circle_rate = -data->circle_rate;
            data->circle_angle = data->circle_rate * 0.017453292f;
            data->circle_timer = scale_game_frames((int)random_float_between(30.0f, 90.0f));
            moby->state = 4;
            data->blocked = 0;
            select_hoven_anklebiter_pursuit_animation((struct Moby *)moby);
            moby->proximity = 0;
        }
        break;
    }
    case ANKLEBITER_WAIT_FOR_LEAP_TRIGGER:
        angle = fast_arc_tan(data->home[0] - moby->pos[0], data->home[1] - moby->pos[1]);
        approach_w((char *)moby, &data->turn_velocity, angle, 0.05f, 0.3f, 0.2f);
        fast_vec_sub(target_report_or_vector_scratch.v0, target->pos, data->home);
        if ((fast_vec_length(target_report_or_vector_scratch.v0) < data->range &&
             absolute_float(data->home[2] - target->pos[2]) < 8.0f) ||
            moby->proximity == 2) {
            if (moby->group != 255)
                func_L01_0026F040_2E2EF0(moby->group, 2);
            moby->joints = 0x40;
            data->zero = 0.0f;
            data->gravity = hoven_anklebiter_leap_gravity_gp * D_0015ED70;
            data->launch_speed = hoven_anklebiter_leap_launch_speed_gp * D_0015ED6C;
            data->launch_time = func_L00_0025BC48_2E2EF0(moby->pos,
                                                         data->home,
                                                         &trajectory,
                                                         data->launch_speed,
                                                         -hoven_anklebiter_leap_gravity_gp);
            data->fall_mode = 5;
            data->falling = 0;
            setup_fall54(moby->rot[2], (char *)moby, data->fall, 8, 1, 0);
            data->fall_a = 5.0f;
            data->fall_b = 10.0f;
            moby->state = 12;
            moby->flags |= 0x1000;
            moby->collision = *(int *)(moby->model + 0x10);
        }
        break;
    case ANKLEBITER_LEAP_TO_HOME:
        if (step_moby_motion(moby, data->fall) & 1) {
            moby->state = 14;
            if (moby->anim != 9)
                blend_moby_animation(moby, 9, 8, scale_game_frames(7));
            return;
        }
        if (moby->pos[2] < 0.0f) {
            mark_moby_for_removal(moby);
            return;
        }
        break;
    case ANKLEBITER_LAND_AFTER_LEAP:
        if (moby->anim_flags & 2) {
            moby->state = 4;
            data->blocked = 0;
            if (random_integer_below(256) & 1)
                data->circle_rate = -data->circle_rate;
            data->circle_angle = data->circle_rate * 0.017453292f;
            data->circle_timer = scale_game_frames((int)random_float_between(30.0f, 90.0f));
            select_hoven_anklebiter_pursuit_animation((struct Moby *)moby);
        }
        break;
    case ANKLEBITER_FACE_TARGET_BEFORE_PURSUIT:
        angle = fast_arc_tan(target->pos[0] - moby->pos[0], target->pos[1] - moby->pos[1]);
        approach_w((char *)moby, &data->turn_velocity, angle, 0.03f, 0.3f, D_0015ED6C * 6.2831855f);
        if (moby->anim_flags & 2) {
            moby->state = 4;
            data->blocked = 0;
            select_hoven_anklebiter_pursuit_animation((struct Moby *)moby);
        }
        break;
    case ANKLEBITER_PURSUE_TARGET: {
        float chase_angle, look_difference, clamp;
        chase_angle = fast_arc_tan(target->pos[0] - moby->pos[0], target->pos[1] - moby->pos[1]);
        look_difference = fast_subtract_rotations(chase_angle, moby->rot[2]);
        clamp = 0.6981317f;
        if (look_difference > clamp || (clamp = -0.6981317f, look_difference < clamp))
            look_difference = clamp;
        data->look_angle = look_difference;
        if (move_hoven_anklebiter_toward_heading(
                (struct Moby *)moby, fast_add_rotations(chase_angle, data->circle_angle)) == 2) {
            moby->state = 6;
            data->return_timer = 0;
        } else {
            if (fast_dec_timer(&data->circle_timer)) {
                data->circle_timer = scale_game_frames((int)random_float_between(30.0f, 90.0f));
                data->circle_angle = -data->circle_angle;
            }
            fast_vec_sub(target_report_or_vector_scratch.v0, target->pos, data->home);
            if (vector_distance_xy(moby->pos, target->pos) < 1.5f) {
                moby->state = 5;
                if (moby->anim != 4)
                    blend_moby_animation(moby, 4, 0, scale_game_frames(5));
            } else if (fast_vec_length(target_report_or_vector_scratch.v0) < data->range + 5.5f &&
                       absolute_float(moby->pos[2] - target->pos[2]) < 4.4f) {
                if (moby->group != 255)
                    func_L01_0026F040_2E2EF0(moby->group, 1);
            } else if (moby->proximity != 1 && random_integer_below(9) == 0) {
                moby->state = 6;
                data->return_timer = 0;
            }
        }
        moby->proximity = 0;
        break;
    }
    case ANKLEBITER_MELEE_ATTACK:
        (*(AnklebiterMoby **)D_0013E633_2E2EF0)->anim_rate = 1.3f;
        angle = fast_arc_tan(target->pos[0] - moby->pos[0], target->pos[1] - moby->pos[1]);
        approach_w((char *)moby, &data->turn_velocity, angle, 0.05f, 0.3f, 0.2f);
        if (moby->anim_end == moby->anim &&
            is_value_within_interpolated_window((char *)moby, 13.0f) &&
            absolute_float(moby->pos[2] - target->pos[2]) < 0.5f &&
            vector_distance_xy(moby->pos, target->pos) < 2.0f &&
            fast_difference_between_rotations(
                fast_arc_tan(target->pos[0] - moby->pos[0], target->pos[1] - moby->pos[1]),
                moby->rot[2]) < 0.2617994f) {
            target_report_or_vector_scratch.v0[0] = fast_cos(moby->rot[2]) * 0.2f;
            target_report_or_vector_scratch.v0[1] = fast_sin(moby->rot[2]) * 0.2f;
            target_report_or_vector_scratch.v0[2] = 0.0f;
            qcopy(target_report_or_vector_scratch.v10, target->pos);
            target_report_or_vector_scratch.v10[2] += 0.75f;
            spawn_nearby_effect(target,
                                (int)moby,
                                1,
                                target_report_or_vector_scratch.v10,
                                target_report_or_vector_scratch.v0,
                                1.0f);
        }
        if ((moby->anim_flags & 2) && vector_distance_xy(moby->pos, target->pos) >= 2.0f) {
            moby->state = 4;
            data->blocked = 0;
            select_hoven_anklebiter_pursuit_animation((struct Moby *)moby);
        }
        break;
    case ANKLEBITER_RETURN_HOME: {
        float return_distance;
        move_hoven_anklebiter_toward_heading(
            (struct Moby *)moby,
            fast_arc_tan(data->home[0] - moby->pos[0], data->home[1] - moby->pos[1]));
        if (vector_distance_xy(moby->pos, data->home) < 2.0f) {
            moby->state = 1;
            if (moby->anim != 0)
                blend_moby_animation(moby, 0, 0, scale_game_frames(7));
        }
        return_distance = fast_vec_dist(target->pos, data->home);
        data->return_timer++;
        blocked = 0;
        if (data->path != -1) {
            char *path = ((char *)l12_paths[data->path]);
            blocked = point_in_path_xy(D_0013F3D0_r, path + 0x10, *(int *)path) == 0;
        }
        if (!blocked) {
            if (return_distance < data->range) {
                if (data->return_timer > scale_game_frames(70)) {
                    if (moby->group != 255)
                        func_L01_0026F040_2E2EF0(moby->group, 1);
                    moby->state = 4;
                    data->blocked = 0;
                    select_hoven_anklebiter_pursuit_animation((struct Moby *)moby);
                }
            } else if (moby->proximity == 1) {
                moby->state = 4;
                data->blocked = 0;
                select_hoven_anklebiter_pursuit_animation((struct Moby *)moby);
            }
        }
        moby->proximity = 0;
        break;
    }
    case ANKLEBITER_KNOCKBACK_FALL:
        if (moby->group != 255)
            func_L01_0026F040_2E2EF0(moby->group, 1);
        if (step_moby_motion(moby, data->fall) & 1) {
            moby->state = 10;
            if (moby->anim != 13)
                blend_moby_animation(moby, 13, 8, scale_game_frames(8));
            return;
        }
        if (moby->pos[2] < 27.0f) {
            mark_moby_for_removal(moby);
            return;
        }
        break;
    case ANKLEBITER_CONTROLLED_MOTION_UNTIL_RETURN_HOME:
        if (func_L00_002DDEA0_2E2EF0(moby, data->fall)) {
            moby->state = 6;
            data->return_timer = 0;
            data->stuck = 0;
        }
        break;
    case ANKLEBITER_RECOVER_AFTER_KNOCKBACK:
        if (compute_interpolated_record_value(moby) > 29.0f) {
            moby->state = 4;
            data->blocked = 0;
            select_hoven_anklebiter_pursuit_animation((struct Moby *)moby);
            return;
        }
        break;
    case ANKLEBITER_DEATH_FALL:
        hit = step_moby_motion(moby, data->fall);
        if ((hit & 0x40) ||
            (compute_interpolated_record_value(moby) > 37.0f && moby->anim_end == moby->anim)) {
            moby->anim_rate = 0.0f;
            mark_moby_for_removal_cf(moby, 0, -1);
            moby->state = 15;
            moby->flags |= 8;
        } else if (moby->pos[2] < 10.0f) {
            mark_moby_for_removal_cf(moby, 0, -1);
            {
                mark_moby_for_removal(moby);
                return;
            }
        } else if (hit & 1)
            spawn_type_52_particle_near_moby((char *)moby);
        break;
    case ANKLEBITER_DEATH_EFFECTS_AND_REMOVE:
        spawn_type_52_particle_near_moby((char *)moby);
        spawn_hoven_anklebiter_death_sparks((struct Moby *)moby);
        spawn_hoven_anklebiter_death_sparks((struct Moby *)moby);
        {
            mark_moby_for_removal(moby);
            return;
        }
    }
    if (moby->enabled && moby->state != 9 && moby->state != 8) {
        func_L00_00263950_2E2EF0(
            (char *)moby, data->collision_data, 0, D_0015ED64 * 0.018f, D_0015ED64 * 0.3f);
        if (moby->enabled && fast_vec_dist(moby->pos, D_L12_00167240) < 28.0f) {
            func_L00_0025B178_2E2EF0(moby);
            moby->active = 22;
        }
    }
    return;
}

extern int move_w(void *, void *, void *, void *, float) __asm__("FUN_L00_00258b50");
extern int path_w(int, void *, void *, void *) __asm__("FUN_L00_00261b48");

int move_hoven_anklebiter_toward_heading(struct Moby *moby,
                                         float        speed) __asm__("FUN_L12_002e2eb8");

int move_hoven_anklebiter_toward_heading(struct Moby *moby, float speed) {
    char *moby_vars = (char *)moby->pvars;
    float pre_move_position[4] __attribute__((aligned(16)));
    float forward_movement_vector[4];
    float hit[4];
    float path_corrected_position[4] __attribute__((aligned(16)));
    float path_test_position[4];
    float path_test_offset[4];
    int   move_result;
    *(OvlQuad *)pre_move_position = *(OvlQuad *)&moby->pos;
    ease_hoven_anklebiter_height_to_ground(moby);
    approach_w(moby, (float *)(moby_vars + 0x2A0), speed, 0.05f, 0.3f, 0.2f);
    forward_movement_vector[0] = fast_cos(moby->rot.z) * 2.0f;
    forward_movement_vector[1] = fast_sin(moby->rot.z) * 2.0f;
    forward_movement_vector[2] = 0.0f;
    move_result = move_w(moby, moby_vars + 0x180, forward_movement_vector, hit, 1.0f);
    if (*(int *)(moby_vars + 0x2D0) != -1) {
        fast_vec_sub(path_test_offset, pre_move_position, &moby->pos);
        fast_vec_normalize(path_test_offset, path_test_offset, 0.05f);
        fast_vec_add(path_test_position, pre_move_position, path_test_offset);
        if (path_w(*(int *)(moby_vars + 0x2D0),
                   &moby->pos,
                   path_test_position,
                   path_corrected_position) != 0) {
            char *path = ((char *)l12_paths[*(int *)(moby_vars + 0x2D0)]);
            if (point_in_path_xy(&moby->pos, path + 0x10, *(int *)path) == 0)
                *(OvlQuad *)&moby->pos = *(OvlQuad *)path_corrected_position;
            if (fast_vec_dist(&moby->pos, pre_move_position) < frame_time * 0.5f) {
                *(int *)(moby_vars + 0x2D4) += 1;
                if (scale_game_frames(10) < *(int *)(moby_vars + 0x2D4))
                    move_result = 2;
            } else {
                *(int *)(moby_vars + 0x2D4) = 0;
            }
        }
    }
    return move_result;
}

int          D_L12_0016183C = 0xa;
int          D_L12_00161840 = 8;
extern float D_L12_00161844 __attribute__((sda));
extern float D_L12_00161848 __attribute__((sda));
extern float D_L12_0016184C __attribute__((sda));
extern float D_L12_00161850 __attribute__((sda));
extern float D_L12_00161854 __attribute__((sda));
extern float D_L12_00161858 __attribute__((sda));
extern float D_L12_0016185C __attribute__((sda));
extern float D_L12_00161860 __attribute__((sda));
extern float D_L12_00161864 __attribute__((sda));
extern float D_L12_00161868 __attribute__((sda));
int          D_L12_0016186C = 0x8000eeee;
int          D_L12_00161870 = 0xffee;
int          D_L12_00161874 = 0x8000ff90;
int          D_L12_00161878 = 0xffee;
int          D_L12_0016187C = 0xa;
int          D_L12_00161880 = 0x1e;
extern int   D_L12_00161884 __attribute__((sda));
extern void  add_random_offset_to_vector_xyz(void *, float) __asm__("FUN_L00_0025f8e0");
extern float scale_time(float) __asm__("FUN_001f96b0");
extern int   fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");

/* Death effects: bursts of sparks around the moby, more and slower ones after the death fall (state 15). */
void spawn_hoven_anklebiter_death_sparks(struct Moby *moby) __asm__("FUN_L12_002e3098");

void spawn_hoven_anklebiter_death_sparks(struct Moby *moby) {
    float  pos[4];
    float  vel[4];
    float  accel[4];
    float  push[4];
    char  *vars;
    int    count;
    float *pos_ptr;
    void  *src;
    int    i;
    int    j;
    float  angle;
    float  speed;
    float  rise;
    float  size;
    float  time_base;
    int    start_color;
    int    end_color;
    int    life;
    int    fade;

    count = D_L12_0016183C;
    vars = (char *)moby->pvars;
    if (moby->state == 15)
        count = truncate_float_to_s32((float)count * 1.5f);
    for (i = 0; i < count; i++) {
        pos_ptr = pos;
        src = &moby->pos;
        qcopy_nc(pos, src);
        add_random_offset_to_vector_xyz(pos_ptr, D_L12_00161844);
        pos[2] += 0.55f;
        angle = random_angle_radians();
        speed = random_float_between(D_L12_00161850, D_L12_00161854);
        rise = random_float_between(D_L12_00161858, D_L12_0016185C);
        if (moby->state == 15) {
            speed *= 0.55f;
            rise *= 0.55f;
            pos[2] -= 0.2f;
        }
        vel[0] = fast_cos(angle) * (speed * D_0015ED6C);
        vel[1] = fast_sin(angle) * (speed * D_0015ED6C);
        vel[2] = 0.0f + rise * D_0015ED6C;
        fast_vec_scale(push, vars + 0x120, 0.5f);
        push[2] = 0.0f;
        fast_vec_add(vel, vel, push);
        qcopy(accel, vel);
        accel[2] -= D_L12_00161860 * D_0015ED70 * (float)scale_game_frames(D_L12_00161880);
        for (j = 0; j < D_L12_00161840; j++) {
            size = random_float_between(0.5f, 1.5f);
            vel[3] = size * D_L12_00161864;
            accel[3] = size * D_L12_00161868;
            add_random_offset_to_vector_xyz(pos, D_L12_00161848);
            add_random_offset_to_vector_xyz(accel, D_L12_0016184C * D_0015ED6C);
            start_color =
                fast_tween_color(D_L12_0016186C, D_L12_00161874, random_float_between(0.0f, 1.0f));
            end_color =
                fast_tween_color(D_L12_00161870, D_L12_00161878, random_float_between(0.0f, 1.0f));
            life = scale_game_frames(D_L12_0016187C);
            fade = scale_game_frames(D_L12_00161880);
            time_base = (float)D_L12_00161884;
            spawn_spark(pos,
                        vel,
                        accel,
                        start_color,
                        end_color,
                        life,
                        fade,
                        truncate_float_to_s32(
                            scale_time(random_float_between(time_base * 0.5f, time_base * 2.5f))),
                        -1);
        }
    }
}

/* Variables of the Hoven anklebiter (moby->pvars); the l00 ElectroidVars damage layout moved by 0xB0. */
typedef struct {
    u8  pad0[0x20];
    f32 health; /* 0x20 */
    u8  pad24[2];
    s16 stun_timer; /* 0x26 */
    u8  pad28[0x10];
    s32 reset_alert; /* 0x38 */
    u8  pad3C[0xD4];
    u8  anim[7]; /* 0x110: animation blend state */
    u8  unk117;  /* 0x117 */
    u8  pad118[8];
    u8  fall[0x10]; /* 0x120 */
    f32 unk130;     /* 0x130 */
    f32 unk134;     /* 0x134 */
    f32 unk138;     /* 0x138 */
    f32 unk13C;     /* 0x13C */
    u8  pad140[4];
    s32 unk144; /* 0x144 */
    u8  pad148[8];
    f32 unk150; /* 0x150 */
    f32 unk154; /* 0x154 */
    f32 unk158; /* 0x158 */
    u8  pad15C;
    u8  unk15D; /* 0x15D */
    u8  pad15E[0x12];
    f32 unk170; /* 0x170 */
    f32 unk174; /* 0x174 */
    u8  pad178[0x134];
    f32 speed; /* 0x2AC */
    u8  pad2B0[0x14];
    s32 alert_timer; /* 0x2C4 */
    u8  pad2C8[0x20];
    s16 call_enabled; /* 0x2E8 */
    s16 call_timer;   /* 0x2EA */
} AnklebiterVars;

extern s32  D_001413D0_lui __asm__("D_001413D0") NOT_SDA;
extern s32  count_active_moby_group_members(s32, s32) __asm__("FUN_L01_0026e008");
extern void alert_group_e3528(s32, s32) __asm__("FUN_L01_0026e090");
extern void
call_for_help_e3528(struct Moby *, s32, s32, Vec4f *, f32 *, f32) __asm__("FUN_L00_00259bc8");

/* What find_moby_hit reports: where the hit landed. */
typedef struct {
    u8   pad0[0x10];
    Vec4 pos;
} AnklebiterHit;

extern AnklebiterHit *find_hit_e3528(struct Moby *, s32, s32) __asm__("FUN_L00_0025a420");
extern void           resolve_moby_hit_response(
              struct Moby *, AnklebiterHit *, f32 *, s32, s32 *, f32 *, s32, s32) __asm__("FUN_00213928");
extern void trooper_knockback_arc(void *, f32 *, f32 *, f32 *) __asm__("FUN_L00_0025ab48");
extern void start_fall_e3528(struct Moby *, void *, s32, s32, s32, f32) __asm__("FUN_L00_0025c558");
extern void anim_reset_e3528(struct Moby *, void *) __asm__("FUN_L00_0025d458");
extern void anim_update_e3528(struct Moby *, void *) __asm__("FUN_L00_0025d538");

#include "rnc/overlay/l12/mobys/hoven_anklebiter_data/process_hoven_anklebiter_damage_and_alerts.h"

/* Damage and alert step of the Hoven anklebiter: refreshes its speed while alerted, now and then calls for help behind
   itself, and takes hits (dying, or staggering back). */
void process_hoven_anklebiter_damage_and_alerts(struct Moby *moby) __asm__("FUN_L12_002e3528");

void process_hoven_anklebiter_damage_and_alerts(struct Moby *moby) {
    AnklebiterVars *anklebiter_vars = (AnklebiterVars *)moby->pvars;
    Vec4            help_direction_or_hit_position;
    s32             hit;
    f32             dmg;
    f32             angle;
    f32             unused;
    AnklebiterHit  *coll;

    if (anklebiter_vars->reset_alert != 0) {
        anklebiter_vars->reset_alert = 0;
        anklebiter_vars->alert_timer = scale_game_frames(0xF0);
    }
    if (moby->state != 9 && moby->state != 0xB && moby->state != 0) {
        if (fast_dec_timer(&anklebiter_vars->alert_timer)) {
            anklebiter_vars->speed = 12.0f;
        } else {
            anklebiter_vars->speed = 20.0f;
        }
    }
    if (anklebiter_vars->call_enabled != 0 && moby->state != 9) {
        s16 updated_call_timer = ++anklebiter_vars->call_timer;
        if (scale_game_frames(0x3C) * 15 < updated_call_timer && moby->visible == 0 &&
            moby->group < 0xFF && count_active_moby_group_members(moby->group, -1) >= 4 &&
            random_integer_below(0x45) == 0) {
            help_direction_or_hit_position.f[0] =
                fast_cos(fast_add_rotations(moby->rot.z, 3.1415927f));
            help_direction_or_hit_position.f[1] =
                fast_sin(fast_add_rotations(moby->rot.z, 3.1415927f));
            help_direction_or_hit_position.f[2] = 0.0f;
            call_for_help_e3528(
                moby, D_001413D0_lui, 0x10000, &moby->pos, help_direction_or_hit_position.f, 1.0f);
        }
    }
    coll = find_hit_e3528(moby, 0x330000, 0);
    dmg = 0.0f;
    resolve_moby_hit_response(moby, coll, &anklebiter_vars->health, 0, &hit, &dmg, 0, 4);
    if (coll != 0 && moby->state != 0x63 && moby->state != 8) {
        if (moby->group != 0xFF) {
            alert_group_e3528(moby->group, 7);
        }
        if (dmg != 0.0f) {
            anklebiter_vars->health -= dmg;
            anklebiter_vars->unk130 = 0.008f;
            anklebiter_vars->unk134 = 0.0005f;
            anklebiter_vars->unk138 = hoven_anklebiter_nonlethal_knockback_scale_gp * frame_time;
            anklebiter_vars->unk13C = hoven_anklebiter_nonlethal_knockback_z_scale_gp * frame_time;
            anklebiter_vars->unk15D = 0;
            anklebiter_vars->unk144 = 9;
            anklebiter_vars->unk150 = 1.0f;
            anklebiter_vars->unk154 = 1.0f;
            anklebiter_vars->unk158 = 1.0f;
            if (anklebiter_vars->health <= 0.0f) {
                moby->flags &= ~0x1000;
                anklebiter_vars->unk138 = frame_time * 8.0f;
                anklebiter_vars->unk13C = frame_time * 10.0f;
                help_direction_or_hit_position.q = coll->pos.q;
                trooper_knockback_arc(&help_direction_or_hit_position,
                                      &angle,
                                      &anklebiter_vars->unk138,
                                      &anklebiter_vars->unk13C);
                start_fall_e3528(moby, anklebiter_vars->fall, 6, 1, 0, angle);
                anklebiter_vars->unk170 = 11.0f;
                anklebiter_vars->unk174 = 18.0f;
                moby->state = 0x63;
                anklebiter_vars->unk117 = 0x78;
                anim_reset_e3528(moby, anklebiter_vars->anim);
            } else {
                help_direction_or_hit_position.q = coll->pos.q;
                trooper_knockback_arc(&help_direction_or_hit_position,
                                      &unused,
                                      &anklebiter_vars->unk138,
                                      &anklebiter_vars->unk13C);
                start_fall_e3528(moby, anklebiter_vars->fall, 0xE, 1, 0, unused);
                anklebiter_vars->unk174 = anklebiter_vars->unk170 = -1.0f;
                moby->state = 7;
                anklebiter_vars->unk117 = 0xFA;
                anklebiter_vars->stun_timer = scale_game_frames(0x3C);
                anim_reset_e3528(moby, anklebiter_vars->anim);
            }
            spawn_hoven_anklebiter_death_sparks(moby);
        }
    }
    moby->hit_slot = 0xFF;
    anim_update_e3528(moby, anklebiter_vars->anim);
}
