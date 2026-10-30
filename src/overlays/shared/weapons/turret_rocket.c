/* Turret rocket (class 0x1CA, Wrench name rocket): the homing projectile the Batalia cannon and the Hoven turret fire, and the function that spawns it. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/rendering/set_moby_color.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/world/collision/collision_line.h"
#include "rnc/overlay/shared/weapons/turret_rocket_data/spawn_turret_rocket.h"
#include "rnc/overlay/shared/weapons/turret_rocket_data/update_turret_rocket.h"

typedef union {
    float float_value;
    int   int_value;
} Data;

typedef struct {
    unsigned int words[4];
} Vec_002e7eb0 __attribute__((aligned(16)));

typedef struct {
    unsigned int words[4];
} Vec_002e8210 __attribute__((aligned(16)));

extern void *spawn_burst(void *, void *, int, int, int, int, float) __asm__("FUN_L00_0026cbb0");
extern float FUN_L08_00211a20(float);
extern void
spawn_spark(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");
extern int D_L08_0015F5CC __asm__("D_L08_0015F5CC");

extern int D_L08_00161D08_gp __asm__("D_L08_00161D08") __attribute__((sda));
extern int D_L08_00161D0C_gp __asm__("D_L08_00161D0C") __attribute__((sda));
extern int D_L08_00161D10_gp __asm__("D_L08_00161D10") __attribute__((sda));

extern float moby_creation_default_timer_frames_gp __asm__("D_L08_00161CD8") __attribute__((sda));
extern float moby_creation_particle_count_limit_gp __asm__("D_L08_00161CE0") __attribute__((sda));
extern int   moby_creation_initial_spark_word_gp __asm__("D_L08_00161CF0") __attribute__((sda));
extern int   moby_creation_second_spark_word_gp __asm__("D_L08_00161CF4") __attribute__((sda));
extern int   moby_creation_initial_particle_word_gp __asm__("D_L08_00161CF8") __attribute__((sda));
extern int   moby_creation_second_particle_word_gp __asm__("D_L08_00161CFC") __attribute__((sda));
extern float moby_creation_particle_spread_speed_base_gp __asm__("D_L08_00161D04")
    __attribute__((sda));

extern float scale_game_frames_002e8210(float) __asm__("FUN_L08_00211a20");
extern void  scale_vector_in_place_002e8210(float, void *) __asm__("FUN_L00_0025f730");
extern void  FUN_L00_0025e450_002e8210(void *,
                                       void *,
                                       void *,
                                       float,
                                       float,
                                       int,
                                       int,
                                       int,
                                       float,
                                       float,
                                       float,
                                       int,
                                       float,
                                       float,
                                       int,
                                       int,
                                       int,
                                       int) __asm__("FUN_L00_0025e450");
extern float homing_projectile_trail_particle_count_limit_gp __asm__("D_L08_00161CDC")
    __attribute__((sda));
extern float homing_projectile_spin_rate_degrees_gp __asm__("D_L08_00161CE4") __attribute__((sda));
extern int   homing_projectile_trail_initial_spark_word_gp __asm__("D_L08_00161CE8")
    __attribute__((sda));
extern int homing_projectile_trail_second_spark_word_gp __asm__("D_L08_00161CEC")
    __attribute__((sda));
extern float homing_projectile_trail_spread_speed_limit_gp __asm__("D_L08_00161D00")
    __attribute__((sda));

extern float D_L08_00174660[4];

/* Retail inherits GP 0x166c00: these scalar labels preserve its GP-relative loads.
   Keep the existing timer name while binding it to the checked level-08 address. */

char *spawn_turret_rocket(int    owner,
                          float *position,
                          float *rotation,
                          int    value) __asm__("FUN_L08_002e7eb0");

char *spawn_turret_rocket(int owner, float *position, float *rotation, int value) {
    float position_copy[4] __attribute__((aligned(16)));
    float rotation_copy[4] __attribute__((aligned(16)));
    float matrix[4][4] __attribute__((aligned(16)));
    float velocity[4] __attribute__((aligned(16)));
    float offset[4] __attribute__((aligned(16)));
    float angle, magnitude;
    int   i, particle_frame_count, particle_second_frame_count, particle_third_frame_count;
    char *moby;
    char *data;
    float *volatile particle_position_ptr;
    float *velocity_ptr, *offset_ptr, *matrix_row_ptr;

    *(u128 *)position_copy = *(u128 *)position;
    *(u128 *)rotation_copy = *(u128 *)rotation;
    position = position_copy;
    rotation = rotation_copy;
    moby = create_moby(0x1ca);
    if (moby != 0) {
        data = ((char *)MOBY(moby)->pvars);
        *(int *)(data + 0x14) = owner;
        *(int *)(data + 0x10) = value;
        set_moby_color(moby, 0xa0, 0xa0, 0xa0);
        *(volatile int *)(moby + 0x40) = 0;
        MOBY(moby)->update_dist = 0xff;
        MOBY(moby)->draw_dist = 0xff;
        MOBY(moby)->visible = 1;
        MOBY(moby)->rot.y = -fast_arc_tan(vector_length_xy(rotation_copy), rotation[2]);
        MOBY(moby)->rot.z = fast_arc_tan(rotation[0], rotation[1]);
        qcopy(moby + 0x10, position_copy);
        qcopy(data, rotation_copy);
        if (D_L08_0015F5CC & 1)
            *(volatile int *)(data + 0x1c) = 1;
        else
            *(volatile int *)(data + 0x1c) = -1;
        euler_rotation_matrix(matrix, moby + 0x40);
        i = 0;
        if ((float)i < moby_creation_particle_count_limit_gp) {
            /* Retail intentionally uses the matrix third row as the particle input. */
            particle_position_ptr = matrix[2];
            velocity_ptr = velocity;
            offset_ptr = offset;
            matrix_row_ptr = matrix[1];
            do {
                angle = random_angle_radians();
                {
                    float spread = moby_creation_particle_spread_speed_base_gp;
                    magnitude = random_float_between(spread, spread + spread) * frame_time;
                }
                fast_vec_normalize(
                    velocity_ptr, particle_position_ptr, magnitude * fast_cos(angle));
                fast_vec_normalize(offset_ptr, matrix_row_ptr, magnitude * fast_sin(angle));
                fast_vec_add(velocity_ptr, velocity_ptr, offset_ptr);
                qcopy(offset_ptr, velocity_ptr);
                velocity[3] = moby_creation_particle_primary_motion_w_gp;
                offset[3] = moby_creation_particle_secondary_motion_w_gp;
                i++;
                spawn_burst(moby + 0x10,
                            velocity_ptr,
                            moby_creation_initial_particle_word_gp,
                            moby_creation_second_particle_word_gp,
                            random_int_between(scale_game_frames(10), scale_game_frames(20)),
                            1,
                            20000.0f);
                particle_frame_count = truncate_float_to_s32(FUN_L08_00211a20(random_float_between(
                    (float)D_L08_00161D08_gp, (float)D_L08_00161D08_gp * 1.2f)));
                particle_second_frame_count =
                    truncate_float_to_s32(FUN_L08_00211a20(random_float_between(
                        (float)D_L08_00161D0C_gp, (float)D_L08_00161D0C_gp * 1.2f)));
                particle_third_frame_count =
                    truncate_float_to_s32(FUN_L08_00211a20(random_float_between(
                        (float)D_L08_00161D10_gp, (float)D_L08_00161D10_gp * 1.5f)));
                spawn_spark(moby + 0x10,
                            velocity_ptr,
                            offset_ptr,
                            moby_creation_initial_spark_word_gp,
                            moby_creation_second_spark_word_gp,
                            particle_frame_count,
                            particle_second_frame_count,
                            particle_third_frame_count,
                            -1);
            } while ((float)i < moby_creation_particle_count_limit_gp);
        }
        if (current_level_index == 12)
            *(int *)(data + 0x18) = scale_game_frames(120);
        else
            *(int *)(data + 0x18) =
                truncate_float_to_s32(FUN_L08_00211a20(moby_creation_default_timer_frames_gp));
        update_entity(moby);
    }
    return moby;
}

void update_turret_rocket(char *moby) __asm__("FUN_L08_002e8210");

void update_turret_rocket(char *moby) {
    char *position_ptr = moby + 0x10;
    char *data = ((char *)MOBY(moby)->pvars);
    float position_copy[4] __attribute__((aligned(16)));
    float position[4] __attribute__((aligned(16)));
    float direction[4] __attribute__((aligned(16)));
    float velocity[4] __attribute__((aligned(16)));
    float offset[4] __attribute__((aligned(16)));
    float angle, magnitude, length, radius, spread;
    int   i, particle_frame_count, particle_second_frame_count, particle_third_frame_count;
    int  *timer_ptr;
    char *target;

    qcopy(position_copy, position_ptr);
    fast_vec_add(position_ptr, position_ptr, data);
    if (MOBY(moby)->pos.x < 10.0f || MOBY(moby)->pos.y < 10.0f || MOBY(moby)->pos.z < 10.0f ||
        500.0f < MOBY(moby)->pos.x || 500.0f < MOBY(moby)->pos.y || 500.0f < MOBY(moby)->pos.z) {
        mark_moby_for_removal(moby);
        return;
    }
    fast_vec_scale(direction, data, -1.0f / homing_projectile_trail_particle_count_limit_gp);
    qcopy(position, position_ptr);
    i = 0;
    timer_ptr = (int *)(data + 0x18);
    while ((float)i < homing_projectile_trail_particle_count_limit_gp) {
        fast_vec_add(position, position, direction);
        angle = random_angle_radians();
        spread = homing_projectile_trail_spread_speed_limit_gp;
        magnitude = random_float_between(0.0f, spread) * frame_time;
        fast_vec_normalize(velocity, moby + 0xe0, magnitude * fast_cos(angle));
        fast_vec_normalize(offset, moby + 0xd0, magnitude * fast_sin(angle));
        fast_vec_add(velocity, velocity, offset);
        qcopy(offset, offset);
        velocity[3] = homing_projectile_trail_primary_motion_w_gp;
        offset[3] = homing_projectile_trail_secondary_motion_w_gp;
        i++;
        particle_frame_count = truncate_float_to_s32(scale_game_frames_002e8210(
            random_float_between((float)D_L08_00161D08_gp, (float)D_L08_00161D08_gp * 1.2f)));
        particle_second_frame_count = truncate_float_to_s32(scale_game_frames_002e8210(
            random_float_between((float)D_L08_00161D0C_gp, (float)D_L08_00161D0C_gp * 1.2f)));
        particle_third_frame_count = truncate_float_to_s32(scale_game_frames_002e8210(
            random_float_between((float)D_L08_00161D10_gp, (float)D_L08_00161D10_gp * 1.5f)));
        spawn_spark(position,
                    velocity,
                    offset,
                    homing_projectile_trail_initial_spark_word_gp,
                    homing_projectile_trail_second_spark_word_gp,
                    particle_frame_count,
                    particle_second_frame_count,
                    particle_third_frame_count,
                    -1);
    }
    MOBY(moby)->rot.x = fast_add_rotations(MOBY(moby)->rot.x,
                                           homing_projectile_spin_rate_degrees_gp * 0.017453292f *
                                               frame_time * (float)*(int *)(data + 0x1c));
    target = *(char **)(data + 0x10);
    if (target != 0) {
        if ((unsigned char)target[0x20] == 0xfe)
            goto clear_target;
        if ((unsigned char)target[0x20] == 0xfd)
            goto clear_target;
        if (*(short *)(target + 0xa6) != 0x1b3)
            goto clear_target;
        {
            char *target_data = *(char **)(target + 0x78);
            length = fast_vec_length(data);
            spread = fast_vec_dist(moby + 0x10, *(char **)(data + 0x10) + 0x10);
            fast_vec_scale(position, target_data + 0x20, spread / length);
            fast_vec_add(position, position, *(char **)(data + 0x10) + 0x10);
            fast_vec_sub(direction, position, moby + 0x10);
            fast_vec_normalize(direction, direction, length);
            fast_vec_sub(velocity, direction, data);
            scale_vector_in_place_002e8210(frame_time * 3.7f, velocity);
            fast_vec_add(data, data, velocity);
            fast_vec_normalize(data, data, length);
        }
        goto target_done;
    clear_target:
        *(int *)(data + 0x10) = 0;
    }
target_done:
    ((int *)direction)[1] = 0x50001;
    ((int *)direction)[0] = (int)moby;
    direction[3] = 2.0f;
    ((int *)velocity)[0] = 1;
    qcopy(position, data);
    if (fast_dec_timer(timer_ptr) == 0)
        goto collision;
remove:
    mark_moby_for_removal(moby);
    return;
collision:
    if (collision_line(position_copy, moby + 0x10, 0, moby, position) == 0)
        return;
    qcopy(moby + 0x10, D_L08_00174660);
    if (15.0f < MOBY(moby)->pos.x && 15.0f < MOBY(moby)->pos.y && 15.0f < MOBY(moby)->pos.z) {
        radius = 24.0f;
        if (MOBY(moby)->pos.x - 2.0f < radius)
            radius = MOBY(moby)->pos.x - 2.0f;
        if (MOBY(moby)->pos.y - 2.0f < radius)
            radius = MOBY(moby)->pos.y - 2.0f;
        if (MOBY(moby)->pos.z - 2.0f < radius)
            radius = MOBY(moby)->pos.z - 2.0f;
        FUN_L00_0025e450_002e8210(moby,
                                  data,
                                  moby + 0x10,
                                  3.0f,
                                  3.0f,
                                  0,
                                  10,
                                  20,
                                  5.0f,
                                  3.0f,
                                  9.0f,
                                  -1,
                                  1.0f,
                                  radius,
                                  1,
                                  1,
                                  -1,
                                  0);
    }
    mark_moby_for_removal(moby);
}

int   D_L12_00161A88 = 7;
int   D_L12_00161A8C = 8;
int   D_L12_00161A90 = 0xf;
float D_L12_00161A58 = 2.4e+02f;
float D_L12_00161A60 = 1e+01f;
int   D_L12_00161A70 = 0x10b0c0c0;
int   D_L12_00161A74 = 0x8708080;
int   D_L12_00161A78 = 0x4f0090f0;
int   D_L12_00161A7C = 0x4f0000f0;
float D_L12_00161A84 = 2.0f;
float D_L12_00161A5C = 2.0f;
float D_L12_00161A64 = 3.2e+02f;
int   D_L12_00161A68 = 0x30e0ffff;
int   D_L12_00161A6C = 0x1080c0c0;
float D_L12_00161A80 = 1.0f;
