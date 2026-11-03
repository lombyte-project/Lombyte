/* Level 07 moby class 0x42d: command triggered falling platform. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "rnc/data_labels.h"
#include "rnc/game/game_globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"
#include "qcopy.h"
#include "rnc/overlay/shared/world/level_labels.h"
#include "rnc/math/random.h"
#include "rnc/overlay/shared/math/rotations/reorient_position_about_pivot.h"
#include "rnc/overlay/shared/mobys/count_active_moby_group_members.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"

/* steps a hit-effect emitter: orient, spawn a puff, and reseed the timer */
/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_00313690), where it is exact; names translated to the US level program. */
extern int   FUN_0022da68(int, int, int);
extern void  update_entity_matrix(void *, void *, void *, void *) __asm__("FUN_L00_00260738");
extern void  mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern f32   random_float_between(f32, f32) __asm__("FUN_002132a8");
void         mark_moby_for_removal(void *obj) __asm__("FUN_0020c828");
extern int   rand(void) __asm__("FUN_001160d8");
extern s32   rand(void);
extern float step_angle_toward_target(float *, float, int, float) __asm__("FUN_L00_0025bc00");
extern float random_float_between(float, float) __asm__("FUN_002132a8");
void         command_falling_platform_update(char *moby) __asm__("FUN_L07_003112c8");

typedef struct CommandFallingPlatformVars {
    u8  pad_0[0x20];
    s32 health; /* 0x20: MobyTargetInfo.health, cleared as an int */
    s16 max_health;
    u8  pad_26[0x2];
    u8  hit_weight; /* 0x28: enemy target info at pvars+0x20, see moby_target_info.h */
    u8  pad_29[0x15];
    s16 target_flags; /* 0x3E: MobyTargetInfo.flags; target info at pvars+0x20, see moby_target_info.h */
    u8  pad_40[0x20];
    f32 transform[16];
    s32 parent_index;
    f32 saved_rot_x;
    f32 saved_rot_y;
    s32 sway_timer;
    f32 fall_speed;
    f32 gravity_scale;
    f32 float_height; /* water line the platform bobs at */
    u8  recovers;     /* nonzero: floats back up after the fall instead of being removed */
    u8  pre_fall_delay;
    u8  fall_signaling_enabled;
    u8  parent_fall_signal;
    f32 sway_phase;
    f32 roll_phase;
    s32 crossing_sound_timer; /* a sound plays when it crosses float_height */
    f32 last_z;
} CommandFallingPlatformVars;

/* Moby class 1069 */
void command_falling_platform_update(char *moby) {
    enum {
        PLATFORM_INITIALIZE = 0,
        PLATFORM_COMMAND_GATED_SWAY = 1,
        PLATFORM_TIMED_SWAY_BEFORE_FALL = 2,
        PLATFORM_TILT_AND_FALL = 3,
        PLATFORM_PARENT_GATED_RECOVERY_SETUP = 4,
        PLATFORM_HEIGHT_RECOVERY_AND_SWAY = 5
    };
    CommandFallingPlatformVars *falling_platform_vars =
        (CommandFallingPlatformVars *)MOBY(moby)->pvars;
    char *parent = 0;
    char *parent_data = 0;
    float previous[4];
    float delta[4];
    float rotation[4];
    float platform_to_hero_offset[4];
    float current_rotation[4];
    float current_position[4];
    float transformed[4];
    float target;
    float height;
    float sway;
    float roll;
    float speed;
    int   random_bits_or_sink_hold_frames;

    qcopy(previous, moby + 0x10);
    fast_vec_scale(delta, moby + 0x10, -1.0f);
    qcopy(rotation, moby + 0x40);
    if (falling_platform_vars->parent_index != -1) {
        parent = level_mobys + falling_platform_vars->parent_index * 0x100;
        parent_data = *(char **)(parent + 0x78);
    }
    if (((unsigned char *)moby)[0xBC] == 2) {
        MOBY(moby)->state = PLATFORM_TIMED_SWAY_BEFORE_FALL;
        MOBY(moby)->command = 3;
        falling_platform_vars->sway_timer = falling_platform_vars->pre_fall_delay;
    }
    switch (((unsigned char *)moby)[0x20]) {
    case PLATFORM_INITIALIZE:
        falling_platform_vars->health = 0;
        falling_platform_vars->max_health = 0;
        falling_platform_vars->hit_weight = 4;
        falling_platform_vars->target_flags = 13;
        timer = rand();
        if (timer & 1)
            MOBY(moby)->rot.z = fast_add_rotations(MOBY(moby)->rot.z, 3.1415927f);
        falling_platform_vars->saved_rot_x = MOBY(moby)->rot.x;
        falling_platform_vars->saved_rot_y = MOBY(moby)->rot.y;
        falling_platform_vars->crossing_sound_timer = -1;
        MOBY(moby)->state = PLATFORM_COMMAND_GATED_SWAY;
        MOBY(moby)->command = 0;
        if (((unsigned char *)moby)[0xB0] != 0xFF) {
            if (visited52[((unsigned char *)moby)[0xB0] + current_level_index * 16] != 0xFF)
                break;
            MOBY(moby)->pos.z -= 5.0f;
            if (falling_platform_vars->recovers == 0) {
                mark_moby_for_removal(moby);
                return;
            }
            MOBY(moby)->state = PLATFORM_HEIGHT_RECOVERY_AND_SWAY;
        }
        break;
    case PLATFORM_COMMAND_GATED_SWAY:
        if (((unsigned char *)moby)[0xBC] != 1)
            break;
        if (falling_platform_vars->sway_timer == 0)
            falling_platform_vars->sway_timer = rand() % 10;
        if (falling_platform_vars->fall_signaling_enabled != 0)
            FUN_L01_0026e090(((unsigned char *)moby)[0x21], 2);
        MOBY(moby)->rot.x = fast_add_rotations(
            falling_platform_vars->saved_rot_x,
            fast_sin(normalize_angle(convert_integer_to_float(falling_platform_vars->sway_timer) *
                                     0.75f)) *
                0.017453292f);
        MOBY(moby)->rot.y = fast_add_rotations(
            falling_platform_vars->saved_rot_y,
            fast_cos(normalize_angle(convert_integer_to_float(falling_platform_vars->sway_timer) *
                                     0.89f)) *
                0.017453292f);
        break;
    case PLATFORM_TIMED_SWAY_BEFORE_FALL:
        MOBY(moby)->rot.x = fast_add_rotations(
            falling_platform_vars->saved_rot_x,
            fast_sin(normalize_angle(convert_integer_to_float(falling_platform_vars->sway_timer) *
                                     0.75f)) *
                0.017453292f);
        MOBY(moby)->rot.y = fast_add_rotations(
            falling_platform_vars->saved_rot_y,
            fast_cos(normalize_angle(convert_integer_to_float(falling_platform_vars->sway_timer) *
                                     0.89f)) *
                0.017453292f);
        if (!fast_dec_timer(&falling_platform_vars->sway_timer))
            break;
        if (falling_platform_vars->fall_signaling_enabled != 0) {
            parent_data[0x19D] = falling_platform_vars->parent_fall_signal;
            parent[0x20] = 12;
        }
        MOBY(moby)->state = PLATFORM_TILT_AND_FALL;
        break;
    case PLATFORM_TILT_AND_FALL:
        speed = 1.3962634f;
        speed -= MOBY(moby)->rot.x;
        speed *= 0.05f;
        MOBY(moby)->rot.x += speed;
        {
            /* Retail takes the sine of the rotation step. */
            float descent = fast_sin(speed) * 3.1f;
            float gravity = frame_time_sq;
            float drop = 15.0f;
            MOBY(moby)->pos.z -= descent;
            falling_platform_vars->fall_speed -= falling_platform_vars->gravity_scale * gravity;
            MOBY(moby)->pos.z += falling_platform_vars->fall_speed;
            if (!(MOBY(moby)->pos.z < falling_platform_vars->float_height - drop))
                break;
            if (falling_platform_vars->recovers != 0) {
                falling_platform_vars->fall_speed = 0.0f;
                MOBY(moby)->state = PLATFORM_PARENT_GATED_RECOVERY_SETUP;
            } else {
                mark_moby_for_removal(moby);
                return;
            }
        }
        break;
    case PLATFORM_PARENT_GATED_RECOVERY_SETUP:
        if (((unsigned char *)parent_data)[0xEF] < 2 && ((unsigned char *)parent)[0x20] > 11)
            break;
        falling_platform_vars->fall_speed = random_float_between(frame_time, frame_time * 4.0f);
        falling_platform_vars->sway_phase = random_angle_radians();
        falling_platform_vars->roll_phase = random_angle_radians();
        MOBY(moby)->state = PLATFORM_HEIGHT_RECOVERY_AND_SWAY;
        MOBY(moby)->rot.x = 0.0f;
        MOBY(moby)->rot.y = 0.0f;
        falling_platform_vars->sway_timer = 0;
        break;
    case PLATFORM_HEIGHT_RECOVERY_AND_SWAY:
        target = MOBY(moby)->pos.z + falling_platform_vars->fall_speed;
        height = falling_platform_vars->float_height;
        if (hero.air_frames.s == 0 && (char *)hero.ground_moby == moby) {
            if (falling_platform_vars->sway_timer == 0)
                FUN_0022da68(0, 0, (int)moby);
            timer = scale_game_frames(90);
            if (falling_platform_vars->sway_timer < timer)
                falling_platform_vars->sway_timer = scale_game_frames(90);
        }
        if (falling_platform_vars->sway_timer != 0) {
            if (MOBY(moby)->pos.z > falling_platform_vars->float_height - 2.6f) {
                falling_platform_vars->fall_speed = -(frame_time * 0.25f);
            } else {
                falling_platform_vars->fall_speed = 0.0f;
            }
        } else {
            if (target < height) {
                float correction = (height - target) * 0.2f;
                if (correction > 0.9f)
                    correction = 0.9f;
                else if (correction < -0.9f)
                    correction = -0.9f;
                falling_platform_vars->fall_speed += correction;
            }
            {
                float velocity = falling_platform_vars->fall_speed - frame_time_sq * 9.8f;
                float cap = frame_time * 5.0f;
                falling_platform_vars->fall_speed = velocity;
                if (cap < velocity)
                    falling_platform_vars->fall_speed = cap;
            }
        }
        MOBY(moby)->pos.z += falling_platform_vars->fall_speed;
        falling_platform_vars->sway_phase =
            fast_add_rotations(falling_platform_vars->sway_phase, 0.01f);
        falling_platform_vars->roll_phase =
            fast_add_rotations(falling_platform_vars->sway_phase, 0.017f);
        sway = fast_sin(falling_platform_vars->sway_phase) * 0.02f;
        roll = fast_sin(falling_platform_vars->roll_phase) * 0.02f;
        if (falling_platform_vars->sway_timer != 0) {
            float forward, side;
            fast_dec_timer(&falling_platform_vars->sway_timer);
            fast_vec_sub(platform_to_hero_offset, &hero.motion.pos, moby + 0x10);
            forward = fast_vec_dot(platform_to_hero_offset, moby + 0xC0);
            side = fast_vec_dot(platform_to_hero_offset, moby + 0xD0) * -0.03f;
            forward *= 0.03f;
            sway = fast_add_rotations(sway, side);
            sway = fast_add_rotations(sway, forward);
            *(u128 *)current_rotation = *(u128 *)(moby + 0x40);
            *(u128 *)current_position = *(u128 *)(moby + 0x10);
            reorient_position_about_pivot(
                &hero.motion.pos, transformed, moby + 0x10, current_rotation, moby + 0x40);
            fast_vec_sub(&hero.motion.platform_delta, transformed, &hero.motion.pos);
            hero.motion.platform_delta.f[2] += MOBY(moby)->pos.z - current_position[2];
        }
        step_angle_toward_target((float *)&MOBY(moby)->rot, sway, 0, frame_time * 0.17453292f);
        step_angle_toward_target((float *)&MOBY(moby)->rot.y, roll, 0, frame_time * 0.17453292f);
        if (MOBY(moby)->rot.x > 0.5235988f)
            MOBY(moby)->rot.x = 0.5235988f;
        if (MOBY(moby)->rot.x < -0.5235988f)
            MOBY(moby)->rot.x = -0.5235988f;
        if (MOBY(moby)->rot.y > 0.5235988f)
            MOBY(moby)->rot.y = 0.5235988f;
        if (MOBY(moby)->rot.y < -0.5235988f)
            MOBY(moby)->rot.y = -0.5235988f;
        break;
    }
    if (((unsigned char *)moby)[0x20] == 5 &&
        fast_dec_timer(&falling_platform_vars->crossing_sound_timer)) {
        float last = falling_platform_vars->last_z;
        float height = falling_platform_vars->float_height;
        if ((height <= last && MOBY(moby)->pos.z < height) ||
            (last <= height && height < MOBY(moby)->pos.z)) {
            FUN_0022da68(0, 0, (int)moby);
            falling_platform_vars->crossing_sound_timer =
                rand() % scale_game_frames(20) + scale_game_frames(20);
        }
    }
    falling_platform_vars->last_z = MOBY(moby)->pos.z;
    fast_vec_add(delta, delta, moby + 0x10);
    update_entity_matrix(falling_platform_vars->transform, delta, rotation, moby + 0x40);
}
