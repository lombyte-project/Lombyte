/* Blarg ship rocket (class 0x333, Wrench name rocket): the rocket the blarg ship fires, its spawner, and a moby sprite particle helper. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/fast_math.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"

typedef struct {
    float pad0[4];
    float a[4];
    float b[4];
    int   c[4];
    float d[4];
} L10SpriteData;

extern int            fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern unsigned char *func_L00_00273E08_298940(
    void *, int, unsigned char, int, int, int, int, float) __asm__("FUN_L00_00272f68");
void emit_moby_sprite_particles(char *moby) __asm__("FUN_L10_00298940");

void emit_moby_sprite_particles(char *moby) {
    L10SpriteData *sprite_data = ((L10SpriteData *)MOBY(moby)->pvars);
    float          d[4] __attribute__((aligned(16)));
    float          particle_position_step[4] __attribute__((aligned(16)));
    int            i, col;
    float          color_blend_offset;

    MOBY(moby)->pos.z += 0.5f;
    fast_vec_sub(d, D_L10_00167240, moby + 0x10);
    fast_vec_normalize(d, d, -0.3f);
    fast_vec_normalize(particle_position_step, d, 0.1f);
    fast_vec_add(d, d, moby + 0x10);
    MOBY(moby)->pos.z -= 0.5f;
    for (i = 0; i < 4; i++) {
        float phase_sum = sprite_data->a[i] + sprite_data->b[i];
        sprite_data->a[i] = phase_sum;
        if (255.0f <= phase_sum) {
            sprite_data->a[i] = phase_sum - 255.0f;
        } else if (phase_sum <= 0.0f) {
            sprite_data->a[i] = phase_sum + 255.0f;
        }
        if (fast_dec_timer(&sprite_data->c[i])) {
            sprite_data->c[i] = scale_game_frames(0xFF);
        }
        color_blend_offset = convert_integer_to_float(scale_game_frames(0xFF) - sprite_data->c[i]);
        color_blend_offset = color_blend_offset / (float)scale_game_frames(0xFF);
        col = fast_tween_color(0x4040FFFF, 0x1040FFFF, absolute_float(0.5f - color_blend_offset));
        func_L00_00273E08_298940(d,
                                 col,
                                 truncate_float_to_s32(sprite_data->a[i]) & 0xFF,
                                 0x35,
                                 1,
                                 2,
                                 0,
                                 sprite_data->d[i]);
        fast_vec_add(d, d, particle_position_step);
    }
}

#include "sda.h"
#include "qcopy.h"

char *spawn_blarg_ship_rocket(float scale,
                              void *initial_velocity,
                              void *pos,
                              int   owner,
                              int   target,
                              int   timer_duration) __asm__("FUN_L10_002cc1b8");

char *spawn_blarg_ship_rocket(

    float scale,
    void *initial_velocity,
    void *pos,
    int   firing_moby,
    int   target,

    int timer_duration) {
    char *rocket = (char *)create_moby(0x333);
    char *shot_data;
    if (rocket) {
        *(float *)(rocket + 0x2C) = *(float *)(rocket + 0x2C) * scale;
        *(unsigned char *)(rocket + 0x30) = 0xFF;
        *(short *)(rocket + 0x32) = 0x7E;
        shot_data = *(char **)(rocket + 0x78);
        rocket[0x31] = 1;
        rocket[0x20] = 1;
        qcopy(rocket + 0x10, pos);
        qcopy(shot_data, initial_velocity);
        *(int *)(shot_data + 0x18) = scale_game_frames(timer_duration);
        *(int *)(shot_data + 0x10) = firing_moby;
        *(int *)(shot_data + 0x14) = target;
        *(int *)(shot_data + 0x28) = 0;
        *(float *)(shot_data + 0x38) = fast_vec_dist(rocket + 0x10, &hero.motion.pos);
        *(int *)(shot_data + 0x30) = 0;
        *(int *)(shot_data + 0x34) = 0;
        *(float *)(rocket + 0x48) = fast_arc_tan(*(float *)shot_data, *(float *)(shot_data + 4));
        *(float *)(rocket + 0x44) =
            -fast_arc_tan(vector_length_xy(shot_data), *(float *)(shot_data + 8));
        update_entity(rocket);
    }
    return rocket;
}
/* Homing shot: steers toward the hero along an intercept course, trails sparks, then bursts. */

typedef int Q_cc2e8 __attribute__((mode(TI), aligned(16)));

typedef struct {
    float vel[4];
    int   owner;
    char *target;
    int   timer;
    int   pad1C[3];
    float yawSpeed;
    float pitchSpeed;
    int   passed;
    int   warned;
    float dist;
} Shot;

typedef struct {
    char  pad00[0x40];
    float rx;
    float ry;
    float rz;
} Rot;

extern unsigned char D_0013F350_cc2e8[] __asm__("D_0013F350");
extern float         lensq_cc2e8(void *) __asm__("FUN_L00_001ff430");
extern float spring_turn(float, float, float, float, float, float *) __asm__("FUN_L00_00258110");
extern void  build_spherical_offset(float, float, float, void *) __asm__("FUN_00214db0");
extern void  spawn_effect(void *, void *, int, int, int, int, int, int) __asm__("FUN_L00_00269958");
extern int   collision_line(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern void
spawn_nearby_effect(float, int, void *, int, void *, void *) __asm__("FUN_L00_00259bc8");
extern void trooper_play_sound(int, int, void *, int) __asm__("FUN_L00_0028dc90");

void blarg_ship_rocket_update(char *m) __asm__("FUN_L10_002cc2e8");

/* Moby class 819 */
void blarg_ship_rocket_update(char *rocket) {
    enum {
        ROCKET_STATE_FLYING = 1,
        ROCKET_STATE_BURST = 2
    };
    Shot *shot_data = *(Shot **)(rocket + 0x78);
    float previous_rocket_position[4] __attribute__((aligned(16)));
    float spark_zero_vector[4] __attribute__((aligned(16)));
    float target_displacement_or_damage_vector[4] __attribute__((aligned(16)));
    float hv[4] __attribute__((aligned(16)));
    float tp[4] __attribute__((aligned(16)));
    float aim[4] __attribute__((aligned(16)));
    float speed, intercept_time_squared_coefficient, intercept_time_linear_coefficient,
        discriminant_sqrt, intercept_time_plus_root, intercept_time_minus_root, intercept_time, yaw,
        pitch, dist;
    char          *rocket_position;
    unsigned char *homing_target_state;
    char          *tg;
    float          ground_aim_height;
    char          *tb;

    switch ((unsigned char)rocket[0x20]) {
    case ROCKET_STATE_FLYING:
        qcopy(previous_rocket_position, rocket + 0x10);
        if (shot_data->target == 0 || (unsigned char)shot_data->target[0x20] == 0xFE ||
            (unsigned char)shot_data->target[0x20] == 0xFD)
            rocket[0x20] = 2;
        rocket_position = rocket + 0x10;
        homing_target_state = D_0013F350_cc2e8;
        if ((unsigned char)(homing_target_state[0x20A4] - 1) < 2 &&
            (tg = shot_data->target) == *(char **)(homing_target_state + 0x2080)) {
            speed = fast_vec_length(shot_data);
            qcopy(tp, tg + 0x10);
            ground_aim_height = probe_ground_height(tp, 0, 0.5f) + 0.2f;
            *(Q_cc2e8 *)hv = *(Q_cc2e8 *)(homing_target_state + 0x100);
            hv[2] = 0.0f;
            tp[2] = ground_aim_height;
            if (homing_target_state[0x20A4] == 2)
                tp[2] = ground_aim_height + 2.0f;
            fast_vec_sub(target_displacement_or_damage_vector, tp, rocket_position);
            intercept_time_squared_coefficient = speed * speed - lensq_cc2e8(hv);
            intercept_time_linear_coefficient = (hv[0] * target_displacement_or_damage_vector[0] +
                                                 hv[1] * target_displacement_or_damage_vector[1] +
                                                 hv[2] * target_displacement_or_damage_vector[2]) *
                                                -2.0f;
            discriminant_sqrt =
                square_root(intercept_time_linear_coefficient * intercept_time_linear_coefficient -
                            intercept_time_squared_coefficient * 4.0f *
                                -lensq_cc2e8(target_displacement_or_damage_vector));
            intercept_time_plus_root =
                (-intercept_time_linear_coefficient + discriminant_sqrt) /
                (intercept_time_squared_coefficient + intercept_time_squared_coefficient);
            intercept_time_minus_root =
                (-intercept_time_linear_coefficient - discriminant_sqrt) /
                (intercept_time_squared_coefficient + intercept_time_squared_coefficient);
            if (intercept_time_plus_root > 0.0f && intercept_time_minus_root > 0.0f) {
                if (intercept_time_plus_root > intercept_time_minus_root)
                    intercept_time = intercept_time_plus_root;
                else
                    intercept_time = intercept_time_minus_root;
            } else if (intercept_time_plus_root > 0.0f) {
                intercept_time = intercept_time_plus_root;
            } else {
                intercept_time = -1.0f;
                if (intercept_time_minus_root > 0.0f)
                    intercept_time = intercept_time_minus_root;
            }
            if (intercept_time > 0.0f) {
                fast_vec_scale(aim, hv, intercept_time);
                fast_vec_add(aim, aim, target_displacement_or_damage_vector);
                yaw = fast_arc_tan(aim[0], aim[1]);
                pitch = -fast_arc_tan(vector_length_xy(aim), aim[2]);
            } else {
                yaw = fast_arc_tan(target_displacement_or_damage_vector[0],
                                   target_displacement_or_damage_vector[1]);
                pitch = -fast_arc_tan(vector_length_xy(target_displacement_or_damage_vector),
                                      target_displacement_or_damage_vector[2]);
            }
            ((Rot *)rocket)->rz = spring_turn(((Rot *)rocket)->rz,
                                              yaw,
                                              frame_time_sq * 6.2831855f,
                                              frame_time_sq * 3.1415927f,
                                              frame_time * 6.2831855f,
                                              &shot_data->yawSpeed);
            ((Rot *)rocket)->ry = spring_turn(((Rot *)rocket)->ry,
                                              pitch,
                                              frame_time_sq * 6.2831855f,
                                              frame_time_sq * 3.1415927f,
                                              frame_time * 6.2831855f,
                                              &shot_data->pitchSpeed);
            ((Rot *)rocket)->rx = fast_add_rotations(((Rot *)rocket)->rx, frame_time * 6.2831855f);
            build_spherical_offset(speed, ((Rot *)rocket)->rz, -((Rot *)rocket)->ry, shot_data);
        }
        clear_u64_value(spark_zero_vector);
        spawn_effect(rocket_position,
                     spark_zero_vector,
                     0x6F00AFFF,
                     0xFF,
                     scale_game_frames(random_int_between(15, 22)),
                     0x28,
                     random_int_between(20, 35),
                     1);
        spawn_effect(rocket_position,
                     spark_zero_vector,
                     0x1FFFFFFF,
                     0x4F4F4F,
                     scale_game_frames(random_int_between(30, 60)),
                     0x28,
                     random_int_between(50, 75),
                     0);
        dist = fast_vec_dist(rocket_position, &hero.motion.pos);
        fast_vec_add(rocket_position, rocket_position, shot_data);
        if (shot_data->warned == 0 && shot_data->passed != 0 && shot_data->dist < dist &&
            dist < 5.0f) {
            allocate_voice_for_target_entry(0, 0, rocket);
            shot_data->warned = 1;
        }
        if (dist < shot_data->dist)
            shot_data->passed = 1;
        shot_data->dist = dist;
        if (collision_line(rocket_position, previous_rocket_position, 0x10, shot_data->owner, 0)) {
            tb = ground_hit;
            if (*(int *)(tb + 0x18) != shot_data->owner) {
                if (*(int *)(tb + 0x18) != 0) {
                    set_vector_length_xy(target_displacement_or_damage_vector, shot_data, 1.0f);
                    shot_data->vel[2] = 1.0f;
                    spawn_nearby_effect(1.0f,
                                        *(int *)(tb + 0x18),
                                        rocket,
                                        0x10003,
                                        tb + 0x20,
                                        target_displacement_or_damage_vector);
                }
                rocket[0x20] = 2;
            }
        }
        if (fast_dec_timer(&shot_data->timer))
            rocket[0x20] = 2;
        break;
    case ROCKET_STATE_BURST:
        if (current_level_index != 10)
            cart_burst(rocket, rocket + 0x10, -1, 0.25f, 13.0f);
        else
            cart_burst(rocket, rocket + 0x10, -1, 0.25f, 0.0f);
        trooper_play_sound(0, 0, rocket, 0x99);
        mark_moby_for_removal(rocket);
        break;
    }
}
