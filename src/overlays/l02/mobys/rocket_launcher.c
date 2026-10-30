/* Level 02 moby class rocket_launcher. */
#include "types.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/math_consts.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/l02/mobys/rocket_launcher_globals.h"

extern float D_0015ED6C;
extern int   allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");

/* Ported from rac1-decomp (src/overlays/l02_aridia/vendor_002A59D8.c: func_L02_002D6B60), where it is exact; names translated to the US level program. */

extern float scale_time(float) __asm__("FUN_001f96b0");

#include "sda.h"

extern char D_L02_001673C0[];

extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");

extern int  voice_is_playing(void *, int) __asm__("FUN_L00_0028d8c0");
extern void release_voice_slot(s32) __asm__("FUN_0022d798");

#include "qcopy.h"

extern float compute_interpolated_record_value(void *) __asm__("FUN_0020c9e0");

/* Pvars of the l02 swivel turret: turns to a random heading, then fires one shot. */
typedef struct {
    f32 yaw_vel;    /* 0x00 */
    f32 target_yaw; /* 0x04 */
    s16 wait_timer; /* 0x08 */
    u8  padA[2];
    s16 voice; /* 0x0C: turning loop sound, -1 for none */
    u8  padE[2];
    f32 recoil; /* 0x10 */
} SwivelTurretVars;

typedef struct {
    u8           pad0[0x74];
    u8           active;
    u8           pad75[0x13];
    struct Moby *owner;
} TurretVoiceSlot;

extern float D_0015ED6C MACRO_ADDR;
extern f32 rocket_launcher_projectile_speed_gp __asm__("D_L02_00161C6C") __attribute__((sda));

#include "rnc/overlay/l02/mobys/rocket_launcher_data/rocket_launcher_update_2.h"

s32 D_L02_00161C78 = 0x800000ff;
s32 D_L02_00161C7C = 0x80329600;

#include "rnc/overlay/l02/mobys/rocket_launcher_data/rocket_launcher_update.h"

void play_rocket_launcher_delayed_shot_sounds(struct Moby *moby) __asm__("FUN_L02_002df730");

extern void play_rocket_launcher_delayed_shot_sounds(struct Moby *moby) __asm__("FUN_L02_002df730");
extern char *spawn_projectile_moby(char *src, void *pos, float *dir) __asm__("FUN_L02_002ec228");
extern f32   scale_time(f32) __asm__("FUN_001f96b0");
extern f32
turret_approach_angle(f32 *angle, f32 target, f32 *vel, f32, f32, f32) __asm__("FUN_L00_0025be00");
extern void turret_spawn_debris(struct Moby *,
                                void *,
                                void *,
                                f32,
                                f32,
                                s32,
                                s32,
                                s32,
                                f32,
                                f32,
                                f32,
                                f32,
                                s32,
                                f32,
                                s32,
                                s32,
                                s32,
                                s32) __asm__("FUN_L00_0025e450");
extern s32  turret_lerp_colour(f32, s32, s32) __asm__("FUN_001fa6e0");

extern f32 frame_time_sq_macro __asm__("D_0015ED70") MACRO_ADDR;
/* Swivel turret update: waits a random time, turns to a random heading with a loop sound, then fires one shot with debris and fades its colour while the recoil animation plays. */
void rocket_launcher_update(struct Moby *m) __asm__("FUN_L02_002df3d8");

void rocket_launcher_update(struct Moby *moby) {
    enum {
        ROCKET_LAUNCHER_WAIT_BETWEEN_SHOTS = 0,
        ROCKET_LAUNCHER_TURN_AND_FIRE = 1,
        ROCKET_LAUNCHER_WAIT_FOR_POST_SHOT_ANIMATION = 2
    };
    SwivelTurretVars *rocket_launcher_vars = (SwivelTurretVars *)moby->pvars;
    Vec4f             projectile_velocity, muzzle;
    f32               glow_blend_fraction;

    play_rocket_launcher_delayed_shot_sounds(moby);
    switch (moby->state) {
    case ROCKET_LAUNCHER_WAIT_BETWEEN_SHOTS:
        if (fast_dec_timer_s16(&rocket_launcher_vars->wait_timer)) {
            rocket_launcher_vars->target_yaw = random_float_between(0.0f, 3.14159f);
            moby->state = 1;
            allocate_voice_for_target_entry(4, 0, moby);
        }
        break;
    case ROCKET_LAUNCHER_TURN_AND_FIRE:
        turret_approach_angle(
            &moby->rot.z,
            rocket_launcher_vars->target_yaw,
            &rocket_launcher_vars->yaw_vel,
            rocket_launcher_yaw_acceleration_gp * DEG_TO_RAD * frame_time_sq_macro,
            rocket_launcher_yaw_acceleration_gp * DEG_TO_RAD * frame_time_sq_macro,
            rocket_launcher_max_yaw_speed_gp * DEG_TO_RAD * D_0015ED6C);
        if (!voice_is_playing(moby, rocket_launcher_vars->voice))
            rocket_launcher_vars->voice = allocate_voice_for_target_entry(2, 4, moby);
        if (fast_difference_between_rotations(moby->rot.z, rocket_launcher_vars->target_yaw) <
                DEG_TO_RAD &&
            rocket_launcher_vars->yaw_vel == 0.0f) {
            s32 turning_voice_index = rocket_launcher_vars->voice;
            if (turning_voice_index != -1) {
                TurretVoiceSlot *voice_slot =
                    (TurretVoiceSlot *)(((char *)&voice_pool) + turning_voice_index * 0x70);
                if (voice_slot->owner == moby && voice_slot->active)
                    release_voice_slot(turning_voice_index);
            }
            rocket_launcher_vars->voice = -1;
            allocate_voice_for_target_entry(3, 0, moby);
            rocket_launcher_vars->recoil = rocket_launcher_shot_sound_initial_distance_gp;
            moby->state = 2;
            if (moby->prev_seq != 1)
                blend_moby_animation(moby, 1, 0, 1);
            moby_point_position(moby, 1, &muzzle);
            moby_point_position(moby, 0, &projectile_velocity);
            fast_vec_sub(&projectile_velocity, &muzzle, &projectile_velocity);
            fast_vec_normalize(&projectile_velocity,
                               &projectile_velocity,
                               rocket_launcher_projectile_speed_gp * D_0015ED6C);
            turret_spawn_debris(moby,
                                &projectile_velocity,
                                &muzzle,
                                0.0f,
                                0.0f,
                                0,
                                6,
                                0x20,
                                7.0f,
                                4.0f,
                                1.0f,
                                3.0f,
                                -1,
                                30.0f,
                                0,
                                1,
                                -1,
                                0);
            spawn_projectile_moby((char *)moby, &muzzle, (float *)&projectile_velocity);
        }
        break;
    case ROCKET_LAUNCHER_WAIT_FOR_POST_SHOT_ANIMATION:
        moby->flags |= 0x10;
        glow_blend_fraction = compute_interpolated_record_value(moby) / 10.0f;
        if (glow_blend_fraction > 1.0f)
            glow_blend_fraction = 1.0f;
        moby->glow_color = turret_lerp_colour(glow_blend_fraction, D_L02_00161C78, D_L02_00161C7C);
        if (moby->anim_flags & 2) {
            if (moby->prev_seq != 0)
                blend_moby_animation(moby, 0, 0, 1);
            moby->state = 0;
            rocket_launcher_vars->wait_timer =
                truncate_float_to_s32(scale_time(random_float_between(60.0f, 180.0f)));
        }
        break;
    }
}

/* Ported from rac1-decomp (src/overlays/l02_aridia/vendor_002A59D8.c: func_L02_002E0B68), where it is exact; names translated to the US level program. */

extern int D_001413D4 NOT_SDA;
extern int            FUN_L02_0028d618(int, int);
extern short rocket_launcher_shot_sound_distance_threshold_rate_gp __asm__("D_L02_00161C80")
    __attribute__((sda));
extern short rocket_launcher_shot_sound_0_scalar_full_distance_gp __asm__("D_L02_00161C88")
    __attribute__((sda));
extern short rocket_launcher_shot_sound_0_scalar_zero_distance_gp __asm__("D_L02_00161C8C")
    __attribute__((sda));
extern short rocket_launcher_shot_sound_1_scalar_zero_distance_gp __asm__("D_L02_00161C90")
    __attribute__((sda));
extern short rocket_launcher_shot_sound_1_scalar_full_distance_gp __asm__("D_L02_00161C94")
    __attribute__((sda));
s32 allocate_voice_for_target_entry(s32   entry_index,
                                    s32   flags,
                                    void *target) __asm__("FUN_0022da68");

void play_rocket_launcher_delayed_shot_sounds(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    float distance;
    int   sound_a, sound_b, gain_a, gain_b;
    if (*(float *)(data + 0x10) >= 0.0f) {
        distance = fast_vec_dist(D_L02_001673C0, &moby->pos);
        *(float *)(data + 0x10) +=
            *(float *)&rocket_launcher_shot_sound_distance_threshold_rate_gp * D_0015ED6C;
        if (distance < *(float *)(data + 0x10) && D_001413D4 != 0x72) {
            sound_a = allocate_voice_for_target_entry(0, 0x10, (int)moby);
            sound_b = allocate_voice_for_target_entry(1, 0x10, (int)moby);
            gain_a = truncate_float_to_s32(
                (1.0f -
                 (distance - *(float *)&rocket_launcher_shot_sound_0_scalar_full_distance_gp) /
                     (*(float *)&rocket_launcher_shot_sound_0_scalar_zero_distance_gp -
                      *(float *)&rocket_launcher_shot_sound_0_scalar_full_distance_gp)) *
                1024.0f);
            gain_b = truncate_float_to_s32(
                ((distance - *(float *)&rocket_launcher_shot_sound_1_scalar_zero_distance_gp) /
                 (*(float *)&rocket_launcher_shot_sound_1_scalar_full_distance_gp -
                  *(float *)&rocket_launcher_shot_sound_1_scalar_zero_distance_gp)) *
                1024.0f);
            if (gain_a > 1024)
                gain_a = 1024;
            else if (gain_a < 0)
                gain_a = 0;
            if (gain_b > 1024)
                gain_b = 1024;
            else if (gain_b < 0)
                gain_b = 0;
            FUN_L02_0028d618(sound_a, gain_a);
            FUN_L02_0028d618(sound_b, gain_b);
            *(float *)(data + 0x10) = -1.0f;
        }
    }
}
