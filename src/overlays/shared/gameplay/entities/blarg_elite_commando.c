/* Shared moby classes: rocket, blarg_elite_commando. */
#include "types.h"
#include "rnc/gameplay/hero.h"
#include "rnc/gameplay/entities/moby.h"
#include "rnc/math_consts.h"
#include "rnc/globals.h"
#include "asm.h"

#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"
#include "rnc/overlay/moby_anim.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_002A1B58.c: func_L16_002A1B58), where it is exact; names translated to the US level program. */

extern char *func_0020D348_m(int);
extern float vector_length_xy(void *) __asm__("FUN_001f9b20");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern int scale_game_frames(int) __asm__("FUN_001f96f8");

char *FUN_L16_002a09a0(char *owner, float *initial_movement_vector, char *spawn_position) {
    char *moby = func_0020D348_m(0x31);
    if (moby != 0) {
        char *data;
        float xy_length;
        float elevation_angle;
        ((unsigned char *)moby)[0x30] = 0xFF;
        *(short *)(moby + 0x32) = 0xFF;
        moby[0x31] = 1;
        moby[0x20] = 0;
        *(unsigned long *)(moby + 0x38) = *(unsigned long *)(owner + 0x38);
        *(float *)(moby + 0x2C) = *(float *)(moby + 0x2C) * 3.0f;
        qcopy(moby + 0x10, spawn_position);
        data = *(char **)(moby + 0x78);
        qcopy(data, initial_movement_vector);
        *(int *)(data + 0x14) = scale_game_frames(0xF0);
        *(char **)(data + 0x18) = owner;
        *(int *)(data + 0x10) = 0;
        *(float *)(moby + 0x48) = fast_arc_tan(initial_movement_vector[0], initial_movement_vector[1]);
        xy_length = vector_length_xy(initial_movement_vector);
        elevation_angle = fast_arc_tan(xy_length, initial_movement_vector[2]);
        *(float *)(moby + 0x44) = -elevation_angle;
        moby[0x31] = 0;
        *(unsigned short *)(moby + 0x34) |= 1;
    }
    return moby;
}
/* Steer a projectile toward its target and emit its impact effects. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A1B58.c: func_L16_002A1C50), where it is exact; names translated to the US level program. */

extern char D_L16_001671C0[];
extern char D_0013E633[];
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern f32 fast_add_rotations(f32 a, f32 b) __asm__("FUN_001fa580");
extern f32 vector_length_xy(void *) __asm__("FUN_001f9b20");
extern f32 vector_length_xyz(void *a) __asm__("FUN_001f9af0");
extern float fast_vec_dist(void *) __asm__("FUN_001f9b48");
extern int collision_line(void *, void *, int, void *, void *) __asm__("FUN_001efa68");
extern int trooper_play_sound(int, int, int, int) __asm__("FUN_L00_0028dc90");
extern int tick_countdown_32_alt(int *) __asm__("FUN_001f9740");
extern void lerp_vector(void *, void *, void *, float) __asm__("FUN_001f9a40");
extern void play_entity_sound(int, float, void *, int, float, float, int, int, int) __asm__("FUN_L00_002598b0");
extern void spawn_explosion(void *, void *, void *, float, float, int, int, int, float, float,
                             float, int, float, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern void FUN_L15_0029ae18(char *);
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void subtract_vector_xyz(void *out, void *a, void *b) __asm__("FUN_001f9a28");
void build_spherical_offset(f32 *out, f32 scale, f32 a, f32 b) __asm__("FUN_00214db0");
void mark_moby_for_removal(void *obj) __asm__("FUN_0020c828");
extern float func_001F9D10_2E7198(void *, void *) __asm__("FUN_001f9b48");

void rocket_update(char *m) __asm__("FUN_L16_002a0a98");
void rocket_update(char *rocket_moby) {
    float desired[4], next[4], delta[4], blend[4];
    char *rocket_data = *(char **)(rocket_moby + 0x78);
    void *remaining;
    float yaw, pitch;
    if (*(int *)(rocket_data + 0x34) == 0 && func_001F9D10_2E7198(rocket_moby + 0x10, D_L16_001671C0) < 15.0f) {
        *(int *)(rocket_data + 0x34) = 1;
        trooper_play_sound(5, 0, (int)rocket_moby, 0x54C);
    }
    if (*(unsigned char *)(rocket_moby + 0x20) != 0)
        return;
    FUN_L15_0029ae18(rocket_moby);
    subtract_vector_xyz(delta, rocket_data + 0x10, rocket_moby + 0x10);
    yaw = fast_arc_tan(delta[0], delta[1]);
    pitch = fast_arc_tan(vector_length_xy(delta), delta[2]);
    yaw = fast_add_rotations(yaw, *(float *)(rocket_data + 0x28));
    pitch = fast_add_rotations(pitch, *(float *)(rocket_data + 0x2C));
    if (pitch > 1.2217305f)
        pitch = 1.2217305f;
    else if (pitch < -1.2217305f)
        pitch = -1.2217305f;
    build_spherical_offset(desired, *(float *)(rocket_data + 0x24), yaw, pitch);
    if (tick_countdown_32_alt((int *)(rocket_data + 0x20)) == 0) {
        float blend_ticks_remaining = convert_integer_to_float(*(int *)(rocket_data + 0x20));
        float blend_duration_ticks = convert_integer_to_float(*(int *)(rocket_data + 0x30));
        lerp_vector(blend, desired, rocket_data, blend_ticks_remaining / blend_duration_ticks);
        add_vector_xyz(next, rocket_moby + 0x10, blend);
        *(float *)(rocket_moby + 0x48) = fast_arc_tan(blend[0], blend[1]);
        *(float *)(rocket_moby + 0x44) = -fast_arc_tan(vector_length_xy(blend), blend[2]);
    } else {
        add_vector_xyz(next, rocket_moby + 0x10, desired);
        *(float *)(rocket_moby + 0x48) = yaw;
        *(float *)(rocket_moby + 0x44) = pitch;
    }
    remaining = delta;
    if (collision_line((rocket_moby + 0x10), next, 0, rocket_moby, 0) != 0 ||
        vector_length_xyz(remaining) < *(float *)(rocket_data + 0x24)) {
        if (hero.vehicle_mode == 2)
            play_entity_sound((int)rocket_moby, 2.0f, (rocket_moby + 0x10), 3, 2.0f, 1.0f, 0, 1, 0);
        trooper_play_sound(4, 0, (int)rocket_moby, 0x54C);
        spawn_explosion(rocket_moby, rocket_data, (rocket_moby + 0x10), 4.0f, 4.0f, 10, 3, 16, 4.0f, 2.0f, 9.0f, -1, 1.0f, 15.0f,
                         1, 1, -1, 0);
        mark_moby_for_removal(rocket_moby);
    } else
        qcopy((rocket_moby + 0x10), next);
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A1B58.c: func_L16_002A1F78), where it is exact; names translated to the US level program. */

extern float fast_vec_length(void *) __asm__("FUN_001f9af0");
extern float random_float_between_alt(float, float) __asm__("FUN_002132a8");
extern float D_L16_001613E0 __attribute__((sda));
extern float D_L16_001613E4 __attribute__((sda));

char *FUN_L16_002a0dc0(char *owner, char *initial_movement_vector, void *pos, void *vel, int blend_duration_ticks, float unused) {
    char *spawned_moby = (char *)func_0020D348_m(0x32);
    char *moby_data;
    float initial_movement_length, x, random_angle_radians, y;
    if (spawned_moby) {
        *(unsigned char *)(spawned_moby + 0x30) = 0xFF;
        *(short *)(spawned_moby + 0x32) = 0xFF;
        spawned_moby[0x31] = 1;
        spawned_moby[0x20] = 0;
        *(unsigned long *)(spawned_moby + 0x38) = *(unsigned long *)(owner + 0x38);
        *(float *)(spawned_moby + 0x2C) = *(float *)(spawned_moby + 0x2C) * 3.0f;
        qcopy(spawned_moby + 0x10, pos);
        moby_data = *(char **)(spawned_moby + 0x78);
        qcopy(moby_data, initial_movement_vector);
        qcopy(moby_data + 0x10, vel);
        *(int *)(moby_data + 0x30) = blend_duration_ticks;
        *(int *)(moby_data + 0x20) = blend_duration_ticks;
        initial_movement_length = fast_vec_length(initial_movement_vector);
        x = D_L16_001613E0;
        *(float *)(moby_data + 0x24) = initial_movement_length;
        random_angle_radians = random_float_between_alt(-x, x) * DEG_TO_RAD;
        y = D_L16_001613E4;
        *(float *)(moby_data + 0x28) = random_angle_radians;
        *(float *)(moby_data + 0x2C) = random_float_between_alt(0.0f, y) * DEG_TO_RAD;
        *(int *)(moby_data + 0x34) = 0;
        *(float *)(spawned_moby + 0x48) = fast_arc_tan(*(float *)initial_movement_vector, *(float *)(initial_movement_vector + 4));
        *(float *)(spawned_moby + 0x44) = -fast_arc_tan(vector_length_xy(initial_movement_vector), *(float *)(initial_movement_vector + 8));
        spawned_moby[0x31] = 0;
        *(unsigned short *)(spawned_moby + 0x34) |= 1;
    }
    return spawned_moby;
}

#include "qcopy.h"
#include "rnc/overlay/quad.h"

/* Update the floating enemy's patrol, attack and death states. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A1B58.c: func_L16_002D40B8), where it is exact; names translated to the US level program. */

extern char *D_L16_001B0930[];
extern char *FUN_L16_002a09a0(char *, float *, char *);
extern char D_L16_001671C0[], D_0013E633[];
extern char D_L16_00167240[], D_0013E533[];
extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern float fast_vec_dist(void *) __asm__("FUN_001f9b48");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float approach_angle(float *, float *, float, float, float, float) __asm__("FUN_L00_0025be00");
extern float approach_value(float *, float, float) __asm__("FUN_00213ed8");
extern float fast_add_rotations(float, float);
extern float fast_sin(float);
extern float random_angle_radians(void) __asm__("FUN_00213308");
extern float random_float_between(float, float) __asm__("FUN_002132a8");
#include "rnc/gameplay/state/options.h"
extern int D_L16_00161A74 __attribute__((sda));
extern int D_L16_00161A78 __attribute__((sda));
extern int D_L16_00161A7C __attribute__((sda));
extern int D_L16_0015F5C4;
extern int D_L16_0016CC90 __attribute__((section(".data")));
extern int fast_dec_timer_s16(void *) __asm__("FUN_001f9770");
extern int mine_fall(void *, void *) __asm__("FUN_L00_0025c698");
extern int voice_is_playing(void *, int) __asm__("FUN_L00_0028d8c0");
extern int is_value_within_interpolated_window(char *, float) __asm__("FUN_00214cc8");
extern int random_integer_below(int) __asm__("FUN_00213260");
extern int scale_game_frames(int);
extern int truncate_float_to_s32(float);
extern int D_001413DC __attribute__((section(".data")));
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void FUN_L00_00263b38(void *, float);
extern void FUN_L16_002d38b0(struct Moby *);
extern void FUN_L16_002d4070(char *);
extern void add_vector_xyz(void *, void *, void *);
extern void allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");
extern void blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern void normalize_vector_xyz(void *, void *, float);
extern void subtract_vector_xyz(void *, void *, void *);
extern void transform_vector_by_basis(void *, void *, void *);
float FUN_L16_002d3eb8(char *moby, float *target);
extern void FUN_L16_002d41f8_2(void *, void *) __asm__("FUN_L16_002d41f8");
void FUN_L16_002d4590(char *m);
void FUN_L16_002d4908(char *m);
extern float func_L16_002D5280_update(char *, void *, int) __asm__("FUN_L16_002d3eb8");
extern int func_0022ED80_update(int, int, void *) __asm__("FUN_0022da68");
extern float func_001F9D10_update(void *, void *) __asm__("FUN_001f9b48");

void blarg_elite_commando_update(char *m) __asm__("FUN_L16_002d2cf0");
void blarg_elite_commando_update(char *commando_moby) {
    enum {
        COMMANDO_STATE_INITIALIZE = 0,
        COMMANDO_STATE_RETURN_TO_INITIAL_POSITION = 1,
        COMMANDO_STATE_IDLE = 2,
        COMMANDO_STATE_MOVE_TO_PATROL_NODE = 3,
        COMMANDO_STATE_PREPARE_ROCKET_FIRE = 4,
        COMMANDO_STATE_FIRE_ROCKETS = 5,
        COMMANDO_STATE_MOVE_TO_ATTACK_NODE = 7,
        COMMANDO_STATE_KNOCKBACK_RECOVERY = 8,
        COMMANDO_STATE_FATAL_FALL = 9,
        COMMANDO_STATE_FOLLOW_PARENT_TRANSFORM = 10
    };
    float target[4], scratch_vector[4], delta[4];
    char *commando_data = *(char **)(commando_moby + 0x78);
    char *source, *owner;
    float distance, angle;
    float *heading, *movement;
    int ticks;
    int cls;
    FUN_L16_002d38b0(commando_moby);
    owner = *(char **)(commando_data + 0x110);
    cls = *(short *)(owner + 0xA6);
    if (cls == 0 || cls == 0x1A3) {
        qcopy(target, &hero.motion.unkC0);
    } else {
        qcopy(target, owner + 0x10);
    }
    if ((&big_head_cheat_enabled)[7])
        FUN_L00_00263b38(commando_data + 0x130, 2.5f);
    switch (((unsigned char *)commando_moby)[0x20]) {
    case COMMANDO_STATE_INITIALIZE:
        *(int *)(commando_data + 0x2EC) = 0;
        commando_data[0x29] = 1;
        *(short *)(commando_data + 0x24) = 3;
        commando_data[0x28] = 2;
        *(float *)(commando_data + 0x30) = 1.5f;
        *(float *)(commando_data + 0x20) = 3.0f;
        *(float *)(commando_data + 0x2F4) = random_angle_radians();
        *(float *)(commando_data + 0x2FC) = random_angle_radians();
        *(float *)(commando_data + 0x2E8) = random_angle_radians();
        *(float *)(commando_data + 0x2F0) = random_float_between(90.0f, 180.0f) * DEG_TO_RAD * frame_time;
        *(float *)(commando_data + 0x2F8) = random_float_between(90.0f, 180.0f) * DEG_TO_RAD * frame_time;
        *(float *)(commando_data + 0x2E4) = random_float_between(90.0f, 180.0f) * DEG_TO_RAD * frame_time;
        *(short *)(commando_data + 0x302) =
            truncate_float_to_s32(multiply_global_factor_ed64(random_float_between(180.0f, 360.0f)));
        if (random_integer_below(2))
            *(unsigned short *)(commando_moby + 0x34) |= 0x8000;
        else
            *(unsigned short *)(commando_moby + 0x34) &= 0x7FFF;
        {
            char *position = commando_moby + 0x10;
            if (*(int *)(commando_data + 0x120) >= 0) {
                int node = truncate_float_to_s32(random_float_between(
                    0.0f, (float)(*(int *)D_L16_001B0930[*(int *)(commando_data + 0x120)] - 1)));
                *(short *)(commando_data + 0x300) = node;
                qcopy(position, D_L16_001B0930[*(int *)(commando_data + 0x120)] + (short)node * 16 + 0x10);
            }
            *(int *)(commando_data + 0x308) = 1;
            qcopy(commando_data + 0x2C0, position);
            qcopy(commando_data + 0x2B0, position);
        }
        if (*(int *)(commando_data + 0x128)) {
            commando_moby[0x20] = 10;
            commando_moby[0x31] = 0;
            *(unsigned short *)(commando_moby + 0x34) = (*(unsigned short *)(commando_moby + 0x34) | 1) & 0xEFFF;
            *(int *)(commando_moby + 0x94) = 0;
            if (((unsigned char *)commando_moby)[0x53] != 1) {
                blend_moby_animation(commando_moby, 1, 0, scale_game_frames(20));
            }
        } else
            goto idle;
        break;
    case COMMANDO_STATE_FOLLOW_PARENT_TRANSFORM:
        if (*(char **)(commando_data + 0x2D0)) {
            commando_moby[0x31] = 1;
            *(unsigned short *)(commando_moby + 0x34) &= 0xFFFE;
            movement = scratch_vector;
            transform_vector_by_basis(movement, commando_data + 0x2C0, *(char **)(commando_data + 0x2D0) + 0xC0);
            add_vector_xyz(commando_moby + 0x10, movement, *(char **)(commando_data + 0x2D0) + 0x10);
            *(float *)(commando_moby + 0x48) =
                fast_add_rotations(*(float *)(*(char **)(commando_data + 0x2D0) + 0x48), *(float *)(commando_data + 0x2CC));
        }
        break;
    case COMMANDO_STATE_RETURN_TO_INITIAL_POSITION:
        if (!fast_dec_timer_s16(commando_data + 0x2DC)) {
            normalize_vector_xyz(scratch_vector, commando_moby + 0xC0, *(short *)(commando_data + 0x2DC) * 0.1f);
            add_vector_xyz(scratch_vector, scratch_vector, commando_moby + 0x10);
            func_L16_002D5280_update(commando_moby, scratch_vector, 1);
        } else {
            heading = (float *)(commando_moby + 0x48);
            angle = fast_arc_tan(*(float *)(commando_data + 0x2B0) - *(float *)(commando_moby + 0x10),
                                 *(float *)(commando_data + 0x2B4) - *(float *)(commando_moby + 0x14));
            approach_angle(heading, (float *)(commando_data + 0x2D8), angle, frame_time_sq * 12.566371f,
                             frame_time_sq * 12.566371f, frame_time * 12.566371f);
            distance = func_L16_002D5280_update(commando_moby, commando_data + 0x2B0, 1);
            if (((unsigned char *)commando_moby)[0x53] != 0)
                blend_moby_animation(commando_moby, 0, 0, scale_game_frames(10));
            if (distance < 1.0f)
                commando_moby[0x20] = 2;
        }
        break;
    case COMMANDO_STATE_IDLE:
        fast_dec_timer_s16(commando_data + 0x2DC);
        if (*(int *)(commando_data + 0x114) != 2) {
            if (*(short *)(commando_data + 0x2DC) == 0) {
                if (*(int *)(commando_data + 0x120) >= 0) {
                    commando_moby[0x20] = 7;
                    *(short *)(commando_data + 0x2DC) =
                        truncate_float_to_s32(multiply_global_factor_ed64(random_float_between(30.0f, 90.0f)));
                    *(short *)(commando_data + 0x300) = (*(short *)(commando_data + 0x300) + 1) % 2;
                    FUN_L16_002d4070(commando_moby);
                } else if (!random_integer_below(20)) {
                    commando_moby[0x20] = 4;
                    if (((unsigned char *)commando_moby)[0x53] != 2) {
                        blend_moby_animation(commando_moby, 2, 0, scale_game_frames(10));
                    }
                }
            }
        } else if (fast_dec_timer_s16(commando_data + 0x302)) {
            if (*(int *)(commando_data + 0x120) >= 0) {
                *(short *)(commando_data + 0x300) = (*(short *)(commando_data + 0x300) + 1) % 2;
                commando_moby[0x20] = 3;
                *(short *)(commando_data + 0x302) = scale_game_frames(180);
                FUN_L16_002d4070(commando_moby);
            }
        }
        break;
    case COMMANDO_STATE_MOVE_TO_PATROL_NODE: {
        float path_distance = func_L16_002D5280_update(
            commando_moby, D_L16_001B0930[*(int *)(commando_data + 0x120)] + (*(short *)(commando_data + 0x300) * 16 + 0x10), 1);
        if (path_distance < 0.01f) {
            commando_moby[0x20] = 2;
            if (((unsigned char *)commando_moby)[0x53] != 0)
                blend_moby_animation(commando_moby, 0, 0, scale_game_frames(10));
            *(short *)(commando_data + 0x302) =
                truncate_float_to_s32(multiply_global_factor_ed64(random_float_between(180.0f, 360.0f)));
        } else if (fast_dec_timer_s16(commando_data + 0x302)) {
            commando_moby[0x20] = 2;
            if (((unsigned char *)commando_moby)[0x53] != 0)
                blend_moby_animation(commando_moby, 0, 0, scale_game_frames(10));
            *(short *)(commando_data + 0x302) =
                truncate_float_to_s32(multiply_global_factor_ed64(random_float_between(180.0f, 360.0f)));
        }
        if (*(float *)(commando_data + 0x2D4) < 2.0f * frame_time && path_distance < 0.5f) {
            if (((unsigned char *)commando_moby)[0x53] != 0) {
                ticks = 30;
                goto animate_idle;
            }
        }
        break;
    }
    case COMMANDO_STATE_PREPARE_ROCKET_FIRE:
        heading = (float *)(commando_moby + 0x48);
        angle = fast_arc_tan(target[0] - *(float *)(commando_moby + 0x10), target[1] - *(float *)(commando_moby + 0x14));
        approach_angle(heading, (float *)(commando_data + 0x2D8), angle, frame_time_sq * 12.566371f,
                         frame_time_sq * 12.566371f, frame_time * 12.566371f);
        if (((unsigned char *)commando_moby)[0x70] & 2) {
            commando_moby[0x20] = 5;
            if (((unsigned char *)commando_moby)[0x53] != 3) {
                blend_moby_animation(commando_moby, 3, 0, scale_game_frames(10));
            }
        }
        break;
    case COMMANDO_STATE_FIRE_ROCKETS:
        heading = (float *)(commando_moby + 0x48);
        angle = fast_arc_tan(target[0] - *(float *)(commando_moby + 0x10), target[1] - *(float *)(commando_moby + 0x14));
        approach_angle(heading, (float *)(commando_data + 0x2D8), angle, frame_time_sq * 12.566371f,
                         frame_time_sq * 12.566371f, frame_time * 12.566371f);
        if (is_value_within_interpolated_window(commando_moby, 2.0f) ||
            is_value_within_interpolated_window(commando_moby, 17.0f) ||
            is_value_within_interpolated_window(commando_moby, 32.0f)) {
            moby_point_position(commando_moby, 0, scratch_vector);
            subtract_vector_xyz(delta, target, scratch_vector);
            normalize_vector_xyz(delta, delta, frame_time * 20.0f);
            FUN_L16_002a09a0(commando_moby, delta, (char *)scratch_vector);
        } else if (is_value_within_interpolated_window(commando_moby, 10.0f)) {
            int count = *(unsigned short *)(commando_data + 0x2DE) + 1;
            *(unsigned short *)(commando_data + 0x2DE) = count;
            if ((short)count >= 3) {
                commando_moby[0x20] = 2;
                if (((unsigned char *)commando_moby)[0x53] != 0)
                    blend_moby_animation(commando_moby, 0, 0, scale_game_frames(10));
                *(short *)(commando_data + 0x2DC) = scale_game_frames(60);
                *(unsigned short *)(commando_data + 0x2DE) = 0;
            }
        }
        break;
    case COMMANDO_STATE_MOVE_TO_ATTACK_NODE:
        distance = func_L16_002D5280_update(
            commando_moby, D_L16_001B0930[*(int *)(commando_data + 0x120)] + (*(short *)(commando_data + 0x300) * 16 + 0x10), 1);
        fast_dec_timer_s16(commando_data + 0x2DC);
        if (distance < 0.01f || *(short *)(commando_data + 0x2DC) == 0) {
            commando_moby[0x20] = 4;
            if (((unsigned char *)commando_moby)[0x53] != 2)
                blend_moby_animation(commando_moby, 2, 0, scale_game_frames(30));
        }
        if (distance < 0.5f && *(float *)(commando_data + 0x2D4) < 2.0f * frame_time) {
            if (((unsigned char *)commando_moby)[0x53] != 2) {
                blend_moby_animation(commando_moby, 2, 0, scale_game_frames(30));
            }
        }
        break;
    case COMMANDO_STATE_KNOCKBACK_RECOVERY:
        if (((unsigned char *)commando_moby)[0x70] & 2) {
        idle:
            commando_moby[0x20] = 2;
            if (((unsigned char *)commando_moby)[0x53] != 0) {
                ticks = 10;
            animate_idle:
                blend_moby_animation(commando_moby, 0, 0, scale_game_frames(ticks));
            }
        } else {
            normalize_vector_xyz(scratch_vector, commando_moby + 0xC0, -frame_time);
            add_vector_xyz(commando_moby + 0x10, commando_moby + 0x10, scratch_vector);
        }
        break;
    case COMMANDO_STATE_FATAL_FALL: {
        char *position;
        movement = scratch_vector;
        position = commando_moby + 0x10;
        qcopy(movement, position);
        if (mine_fall(commando_moby, commando_data + 0x70) & 0x160) {
            FUN_L16_002d41f8_2(commando_moby, position);
            func_0022ED80_update(0, 0, commando_moby);
            mark_moby_for_removal(commando_moby);
            return;
        }
        subtract_vector_xyz(movement, position, movement);
        add_vector_xyz(commando_data + 0x2C0, commando_data + 0x2C0, movement);
    } break;
    case 6:
    default:
        break;
    }
    if (D_L16_00161A74) {
        *(float *)(commando_data + 0x2BC) = *(float *)(commando_moby + 0x18);
        if (((unsigned char *)commando_moby)[0x20] != 0 && ((unsigned char *)commando_moby)[0x20] != 10) {
            approach_value((float *)(commando_data + 0x2EC), 0.25f, frame_time * 0.25f);
            *(float *)(commando_data + 0x2F4) =
                fast_add_rotations(*(float *)(commando_data + 0x2F4), *(float *)(commando_data + 0x2F0));
            *(float *)(commando_data + 0x2FC) =
                fast_add_rotations(*(float *)(commando_data + 0x2FC), *(float *)(commando_data + 0x2F8));
            *(float *)(commando_data + 0x2E8) =
                fast_add_rotations(*(float *)(commando_data + 0x2E8), *(float *)(commando_data + 0x2E4));
            *(float *)(commando_moby + 0x10) =
                *(float *)(commando_data + 0x2C0) + *(float *)(commando_data + 0x2EC) * fast_sin(*(float *)(commando_data + 0x2F4));
            *(float *)(commando_moby + 0x14) =
                *(float *)(commando_data + 0x2C4) + *(float *)(commando_data + 0x2EC) * fast_sin(*(float *)(commando_data + 0x2FC));
            *(float *)(commando_moby + 0x18) =
                *(float *)(commando_data + 0x2C8) + *(float *)(commando_data + 0x2EC) * fast_sin(*(float *)(commando_data + 0x2E8));
        }
        if (func_001F9D10_update(commando_moby + 0x10, D_L16_001671C0) < 32.0f &&
            !voice_is_playing(commando_moby, *(int *)(commando_data + 0x304)))
            *(int *)(commando_data + 0x304) = func_0022ED80_update(7, 4, commando_moby);
    }
    if (D_L16_00161A78)
        FUN_L16_002d4590(commando_moby);
    if (D_L16_00161A7C && D_001413DC != 22)
        FUN_L16_002d4908(commando_moby);
    if (current_level_index == 18 && D_L16_0015F5C4 == 2 && (unsigned int)(D_L16_0016CC90 - 3) < 3U)
        mark_moby_for_removal(commando_moby);
}
/* Handle damage and knockback, then update the pursuit target and timer. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A1B58.c: func_L16_002D4C78), where it is exact; names translated to the US level program. */

typedef struct {
    char pad00[0x20];
    float health;
    char pad24[0x14];
    int reset_timer;
    char pad3C[0x24];
    short damage[3];
    unsigned char pad66;
    unsigned char animation;
    char pad68[8];
    float knockback[4];
    float impulse_time;
    float vertical_impulse;
    float forward_impulse;
    float lateral_impulse;
    int sound_timer;
    int active;
    float damping;
    char pad9C[0x11];
    unsigned char pending;
    char padAE[0x12];
    float blend_start;
    float blend_end;
    char padC8[8];
    float target[4];
    char padE0[0x30];
    int owner;
    int pursuit_mode;
    char pad118[0xC];
    float pursuit_range;
    char pad128[4];
    int path_index;
    char pad130[0x1B0];
    int pursuit_timer;
    char pad2E4[0x24];
    int reward_hits;
} L16DamageData;


typedef struct {
    OvlQuad vector;
    int status;
    float amount;
    float angle;
    float fatal_angle;
} L16DamageScratch;

typedef struct {
    char pad00[0x2080];
    int owner;
    char pad2084[8];
    int mode;
    char pad2090[0x14];
    unsigned char difficulty;
} L16DamageResident;

typedef struct {
    unsigned char pad00[0x1D];
    unsigned char awarded;
} L16DamageReward;

extern char *D_L16_001B0930[];
extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern unsigned char D_0013D408[];
extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern float vector_distance_xy(void *, void *) __asm__("FUN_001f9b80");
extern int D_0015EE10 __attribute__((sda));
extern int hit_reaction(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_L00_0025a478");
extern int FUN_L00_0025fcb8(float, char *, void *);
extern int FUN_L00_0025ff38(float, char *, void *, int, int, void *, int);
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern void FUN_L00_00257470(void *, int, int);
extern void trooper_knockback_arc(void *, float *, void *, void *) __asm__("FUN_L00_0025ab48");
extern void trooper_start_knockback(float, void *, void *, int, int, int) __asm__("FUN_L00_0025c558");
extern void trooper_anim_update(void *, void *) __asm__("FUN_L00_0025d458");
extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
extern void queue_dialogue_message(int, int) __asm__("FUN_L00_00263d40");
extern void FUN_L16_002d3f78_u(void *, int, int) __asm__("FUN_L16_002d3f78");
extern s32 tick_countdown_32(void *) __asm__("func_001F9740");
s32 allocate_voice_for_bank_entry(s32 arg1, s32 arg0, s32 arg2) __asm__("FUN_0022db10");

void FUN_L16_002d38b0(struct Moby *commando_moby) {
    enum {
        HIT_RESPONSE_UNAVAILABLE = 0,
        HIT_RESPONSE_FATAL = 1,
        HIT_RESPONSE_FATAL_FALL = 2,
        HIT_RESPONSE_KNOCKBACK_RECOVERY = 3,
        HIT_RESPONSE_RECOVERABLE_KNOCKBACK = 4,
        HIT_RESPONSE_NONFATAL_KNOCKBACK = 5,
        HIT_RESPONSE_KNOCKBACK_TO_IDLE = 6,
        HIT_RESPONSE_ENTER_KNOCKBACK_RECOVERY = 7,
        HIT_RESPONSE_KNOCKBACK_AND_RECOVER = 8,
        HIT_RESPONSE_ANIMATION_ONLY = 9,
        HIT_RESPONSE_DAMAGE_ANIMATION_ONLY = 10,
        HIT_RESPONSE_NO_REACTION = 11
    };
    L16DamageScratch scratch;
    L16DamageData *commando_data = (L16DamageData *)commando_moby->pvars;
    char *hit;
    unsigned int response;
    float range;
    float time_scale;
    commando_moby->scale = commando_moby->pclass->scale;
    scratch.amount = 0.0f;
    hit = find_moby_hit(commando_moby, 0x330000, 0);
    if (hit) {
        if (*(char **)(hit + 0x20)) {
            char *source = *(char **)(hit + 0x20);
            hit = *(short *)(source + 0xA6) != 0x31 ? hit : 0;
        }
    }
    response = hit_reaction(commando_moby, hit, &commando_data->health, 0, &scratch.status, &scratch.amount, 0, 4);
    if (scratch.status != 1 && commando_moby->state != 9) {
        FUN_L16_002d3f78_u(commando_moby, 0x12C, 0x168);
        if (hit && *(char **)(hit + 0x20) && *(short *)(*(char **)(hit + 0x20) + 0xA6) != 0x47) {
            commando_data->reward_hits = 0;
        }
        commando_data->health -= scratch.amount;
        if (commando_data->health <= 0.0f)
            response = 1;
        commando_data->sound_timer = truncate_float_to_s32(512.0f);
        commando_data->impulse_time = frame_time_sq * 20.0f;
        commando_data->damping = 0.5f;
        commando_data->vertical_impulse = 0.0f;
        commando_data->active = 1;
        commando_data->pending = 0;
        switch (response) {
        case HIT_RESPONSE_ANIMATION_ONLY:
        case HIT_RESPONSE_DAMAGE_ANIMATION_ONLY:
            commando_data->animation = 0xFA;
            break;
        case HIT_RESPONSE_KNOCKBACK_RECOVERY:
        case HIT_RESPONSE_RECOVERABLE_KNOCKBACK:
        case HIT_RESPONSE_NONFATAL_KNOCKBACK:
        case HIT_RESPONSE_KNOCKBACK_TO_IDLE:
        case HIT_RESPONSE_ENTER_KNOCKBACK_RECOVERY:
        case HIT_RESPONSE_KNOCKBACK_AND_RECOVER: {
            float impulse = frame_time * 6.0f;
            char *source;
            commando_data->blend_start = -1.0f;
            commando_data->blend_end = -1.0f;
            commando_data->forward_impulse = impulse;
            commando_data->lateral_impulse = impulse;
            source = *(char **)(hit + 0x20);
            {
                float heading = fast_arc_tan(commando_moby->pos.x - *(float *)(source + 0x10),
                                             commando_moby->pos.y - *(float *)(source + 0x14));
                scratch.vector = *(OvlQuad *)(hit + 0x10);
                scratch.angle = heading;
            }
            trooper_knockback_arc(&scratch.vector, &scratch.angle, &commando_data->forward_impulse,
                             &commando_data->lateral_impulse);
            trooper_start_knockback(scratch.angle, commando_moby, commando_data->knockback, 6, 1, 0);
            commando_moby->state = 8;
            commando_data->animation = 0x78;
            break;
        }
        case HIT_RESPONSE_FATAL:
        case HIT_RESPONSE_FATAL_FALL: {
            char *source;
            if (current_level_index == 18 && commando_data->reward_hits) {
                ++*(int *)&D_0015EE10;
                if (*(int *)&D_0015EE10 >= 10) {
                    if (D_0013D408[0x1D] == 0) {
                        D_0013D408[0x1D] = 1;
                        allocate_voice_for_bank_entry(1, 0, 0);
                        queue_dialogue_message(0x53D6, -1);
                    }
                }
            }
            time_scale = frame_time_sq * 20.0f;
            commando_moby->flags &= 0xEFFF;
            commando_data->impulse_time = time_scale;
            if (((L16DamageResident *)((char *)&hero))->difficulty == 2) {
                float impulse = frame_time;
                commando_moby->anim_speed = 0.5f;
                commando_data->forward_impulse = impulse * 24.0f;
                commando_data->lateral_impulse = impulse * 8.0f;
            } else {
                float impulse = frame_time * 8.0f;
                commando_data->forward_impulse = impulse;
                commando_data->lateral_impulse = impulse;
            }
            commando_data->blend_start = -1.0f;
            commando_data->blend_end = -1.0f;
            source = *(char **)(hit + 0x20);
            {
                float heading = fast_arc_tan(commando_moby->pos.x - *(float *)(source + 0x10),
                                             commando_moby->pos.y - *(float *)(source + 0x14));
                scratch.vector = *(OvlQuad *)(hit + 0x10);
                scratch.fatal_angle = heading;
            }
            trooper_knockback_arc(&scratch.vector, &scratch.fatal_angle, &commando_data->forward_impulse,
                             &commando_data->lateral_impulse);
            trooper_start_knockback(scratch.fatal_angle, commando_moby, commando_data->knockback, 7, 1, 0);
            commando_moby->state = 9;
            commando_data->animation = 0xF0;
            FUN_L00_00257470(commando_moby, 0, -1);
            break;
        }
        case HIT_RESPONSE_UNAVAILABLE:
        case HIT_RESPONSE_NO_REACTION:
        default:
            break;
        }
        trooper_anim_update(commando_moby, commando_data->damage);
    }
    commando_moby->hit_slot = 0xFF;
    trooper_anim_finish(commando_moby, commando_data->damage);
    tick_countdown_32(&commando_data->pursuit_timer);
    if (commando_data->reset_timer) {
        commando_data->pursuit_timer =
            truncate_float_to_s32(multiply_global_factor_ed64(random_float_between_alt(180.0f, 240.0f)));
    }
    commando_data->reset_timer = 0;
    range = 255.0f;
    if (commando_data->pursuit_timer == 0)
        range = commando_data->pursuit_range;
    if (((L16DamageResident *)((char *)&hero))->mode == 22) {
        commando_data->pursuit_mode = 2;
    } else if (commando_data->path_index >= 0 && commando_data->pursuit_timer == 0) {
        char *path = D_L16_001B0930[commando_data->path_index];
        FUN_L00_0025ff38(24.0f, commando_moby, commando_data->target, 0, 0, path + 0x10, *(int *)path);
    } else {
        FUN_L00_0025fcb8(24.0f, commando_moby, commando_data->target);
    }
    if (range < vector_distance_xy(&commando_moby->pos, commando_data->target))
        commando_data->pursuit_mode = 2;
    if (commando_data->owner == 0)
        commando_data->owner = ((L16DamageResident *)((char *)&hero))->owner;
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A1B58.c: func_L16_002D5188), where it is exact; names translated to the US level program. */

void FUN_L16_002d3dc0(char *moby, void *owner, void *position, char *vector) {
    char *data = *(char **)(moby + 0x78);
    *(void **)(data + 0x2D0) = owner;
    qcopy(data + 0x2C0, position);
    *(float *)(data + 0x2CC) = *(float *)(vector + 8);
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A1B58.c: func_L16_002D51A8), where it is exact; names translated to the US level program. */

void blend_moby_animation_u(MobyAnim *arg0, int arg1, int arg2, int arg3) __asm__("FUN_00212f90");

void FUN_L16_002d3de0(struct Moby *moby, int return_countdown_ticks) {
    char *commando_data = (char *)moby->pvars;
    if (moby->state == 10) {
        moby->state = 1;
        if (moby->prev_seq != 1) {
            blend_moby_animation_u(moby, 1, 0, scale_game_frames(0x14));
        }
        moby->visible = 1;
        moby->flags &= 0xFFFE;
        moby->collision = moby->pclass->collision;
        moby->flags |= 0x1000;
        *(short *)(commando_data + 0x2DC) = return_countdown_ticks;
        *(float *)(commando_data + 0x2BC) = moby->pos.z;
        qcopy(commando_data + 0x2C0, &moby->pos);
        *(float *)(commando_data + 0x2D4) = frame_time * 5.0f;
    }
}
/* steers the moby's vector toward the target and returns the distance */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A1B58.c: func_L16_002D5280), where it is exact; names translated to the US level program. */

extern float fast_vec_dist(void *) __asm__("FUN_001f9b48");
extern float advance_accelerated_scalar(float *, float *, float, float, float,
                                        float) __asm__("FUN_00213f38");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");

float FUN_L16_002d3eb8(char *moby, float *target) {
    char *data = *(char **)(moby + 0x78);
    char *base_position = data + 0x2C0;
    float distance = fast_vec_dist(base_position);
    float acceleration_braking_step = frame_time_sq * 4.0f;
    int zero[1];
    float movement_step[4];
    zero[0] = 0;
    advance_accelerated_scalar((float *)zero, (float *)(data + 0x2D4), distance, acceleration_braking_step, acceleration_braking_step, frame_time * 5.0f);
    fast_vec_sub(movement_step, target, base_position);
    fast_vec_normalize(movement_step, movement_step, *(float *)(data + 0x2D4));
    fast_vec_add(base_position, base_position, movement_step);
    return distance;
}
/* Randomize the timers of active mobys in the selected linked group. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A1B58.c: func_L16_002D5340), where it is exact; names translated to the US level program. */

typedef struct {
    char pad00[0x2E0];
    int timer;
} L16GroupTimer;

typedef struct {
    char pad00[0x20];
    signed char state;
    unsigned char group;
    char pad22[0x56];
    L16GroupTimer *data;
} L16GroupMoby;

extern char *D_L16_0015FFD8;
extern short *D_L16_001ABCC0[];
extern float func_001F9878_timer(float) __asm__("FUN_001f96b0");
extern int func_001FA898_timer(float) __asm__("FUN_001fa6d0");

void FUN_L16_002d3f78(void *group_source_moby, int lower, int upper) {
    L16GroupMoby *source_moby = group_source_moby;
    L16GroupTimer *timer_data;
    short *group_entry;
    if (source_moby->group == 255) {
        timer_data = source_moby->data;
        timer_data->timer = func_001FA898_timer(
            func_001F9878_timer(random_float_between_alt((float)lower, (float)upper)));
    } else {
        L16GroupMoby *other;
        group_entry = D_L16_001ABCC0[source_moby->group];
    next:
        other = (L16GroupMoby *)(((*(unsigned short *)group_entry & 0x7FFF) << 8) + (int)D_L16_0015FFD8);
        if (other->state >= 0) {
            timer_data = other->data;
            timer_data->timer = func_001FA898_timer(
                func_001F9878_timer(random_float_between_alt((float)lower, (float)upper)));
        }
        if (*group_entry++ >= 0)
            goto next;
    }
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A1B58.c: func_L16_002D5438), where it is exact; names translated to the US level program. */

extern char *D_L16_001B0930[];
extern float fast_subtract_rotations(float, float) __asm__("FUN_001fa5c8");
void blend_moby_animation_u(MobyAnim *arg0, int arg1, int arg2, int arg3) __asm__("FUN_00212f90");

void FUN_L16_002d4070(char *moby) {
    char *data = *(char **)(moby + 0x78);
    char *target = D_L16_001B0930[*(int *)(data + 0x120)] + (*(short *)(data + 0x300) << 4);
    float angle = fast_arc_tan(*(float *)(target + 0x10) - *(float *)(moby + 0x10),
                               *(float *)(target + 0x14) - *(float *)(moby + 0x14));
    float delta = fast_subtract_rotations(angle, *(float *)(moby + 0x48));
    if (delta < 0.78539819f && -0.78539819f < delta) {
        if ((unsigned char)moby[0x53] != 1) {
            blend_moby_animation_u(moby, 1, 0, scale_game_frames(0x14));
        }
    } else if (delta < 0.0f) {
        if (*(unsigned short *)(moby + 0x34) & 0x8000) {
            if ((unsigned char)moby[0x53] != 4)
                blend_moby_animation_u(moby, 4, 0, scale_game_frames(0x14));
        } else {
            if ((unsigned char)moby[0x53] != 5)
                blend_moby_animation_u(moby, 5, 0, scale_game_frames(0x14));
        }
    } else if (*(unsigned short *)(moby + 0x34) & 0x8000) {
        if ((unsigned char)moby[0x53] != 5)
            blend_moby_animation_u(moby, 5, 0, scale_game_frames(0x14));
    } else {
        if ((unsigned char)moby[0x53] != 4)
            blend_moby_animation_u(moby, 4, 0, scale_game_frames(0x14));
    }
}
/* Spawn randomized hit particles at joint two, with extra bursts for distant hits. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A1B58.c: func_L16_002D55C0), where it is exact; names translated to the US level program. */

typedef struct {
    int values[6];
} L16ParticleChoices;

extern L16ParticleChoices D_L16_001E8300, D_L16_001E8618;
extern L16ParticleChoices D_L16_001E8600, D_L16_001E8318;
extern char D_L16_001671C0[], D_0013E633[];
extern f32 vector_length_xyz(void *a) __asm__("FUN_001f9af0");
extern float random_float_between_alt(float, float) __asm__("FUN_002132a8");
extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern int random_integer_below(int) __asm__("FUN_00213260");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern float D_L16_0015F5D0;
extern void clear_u64_value(void *) __asm__("FUN_001f99f8");
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void FUN_L00_0026a9f0(void *, void *, int, int, int, int, int, int, float, float);
extern void spawn_glow_particle(void *, void *, void *, float, int, int, int, int, int) __asm__("FUN_L00_002ac910");
extern void subtract_vector_xyz(void *out, void *a, void *b) __asm__("FUN_001f9a28");

void FUN_L16_002d41f8(void *moby) {
    float delta[4], velocity[4], position[4];
    L16ParticleChoices first, second;
    float distance, range, delay;
    int count;
    void *origin;
    float *joint = position;
    void *movement;
    moby_point_position(moby, 2, joint);
    subtract_vector_xyz(delta, D_L16_001671C0, joint);
    clear_u64_value(velocity);
    distance = vector_length_xyz(delta);
    if (distance < 8.0f)
        count = truncate_float_to_s32(distance) + 2;
    else
        count = 10;
    range = 0.0f;
    if (distance < 7.0f)
        range = 7.0f - distance;
    origin = position;
    movement = velocity;
    if (count > 0) {
        int remaining = count;
        do {
            float sample;
            int *first_choice, *second_choice, duration1, duration2;
            remaining--;
            sample = random_float_between_alt(8.0f, 10.0f);
            first = D_L16_001E8300;
            second = D_L16_001E8318;
            delay = sample * frame_time - range * frame_time;
            first_choice = &first.values[random_integer_below(6)];
            second_choice = &second.values[random_integer_below(6)];
            {
                int lo = scale_game_frames(15), hi = scale_game_frames(20);
                duration1 = random_int_between(lo, hi);
            }
            {
                int lo = scale_game_frames(25), hi = scale_game_frames(30);
                duration2 = random_int_between(lo, hi);
            }
            FUN_L00_0026a9f0(origin, movement, *first_choice, *second_choice, duration1, duration2, 0, 0, 400000.0f,
                             delay);
        } while (remaining != 0);
    }
    if (D_L16_0015F5D0 < 0.95f && distance > 9.0f) {
        spawn_glow_particle(moby, origin, movement, 4.0f, scale_game_frames(15), 127, 127, 127, 32);
        spawn_glow_particle(moby, origin, movement, 4.0f, scale_game_frames(24), 127, 32, 0, 32);
    }
    spawn_glow_particle(moby, origin, movement, 4.0f, scale_game_frames(20), 127, 64, 0, 48);
    spawn_glow_particle(moby, origin, movement, 3.5f, scale_game_frames(27), 96, 16, 0, 64);
    spawn_glow_particle(moby, origin, movement, 3.0f, scale_game_frames(29), 32, 0, 0, 32);
}
/* Aim paired joints toward the owner or player, then smooth their rotations. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A1B58.c: func_L16_002D5958), where it is exact; names translated to the US level program. */

extern char D_L16_00167240[], D_0013F420[];
extern f32 vector_length_xy(void *) __asm__("FUN_001f9b20");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("FUN_001f9cf8");
extern void transpose_rotation_matrix(void *, void *) __asm__("FUN_001fa2d8");
extern void update_joint_animation(char *, char *, int, float, float) __asm__("FUN_L00_002628d8");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void normalize_vector_xyz(void *out, void *a, f32 len) __asm__("FUN_001f9bf8");

void FUN_L16_002d4590(char *commando_moby) {
    float position[4], target[4], matrix[16], delta[4], aim[4], aimDelta[4], velocity[4];
    char *commando_data = *(char **)(commando_moby + 0x78);
    if (*(int *)(commando_data + 0x114) != 2) {
        float *savedTarget, *savedMatrix;
        float yaw, pitch;
        qcopy(position, commando_moby + 0x10);
        position[2] += 2.0f;
        if (*(short *)(*(char **)(commando_data + 0x110) + 0xA6) == 0)
            qcopy(target, D_0013F420);
        else
            qcopy(target, commando_data + 0xD0);
        savedTarget = target;
        transpose_rotation_matrix(matrix, commando_moby + 0xC0);
        subtract_vector_xyz(delta, savedTarget, position);
        transform_vector_by_basis(delta, delta, matrix);
        yaw = fast_arc_tan(delta[0], delta[1]);
        pitch = -fast_arc_tan(vector_length_xy(delta), delta[2]);
        if (yaw > 0.7853982f)
            yaw = 0.7853982f;
        else if (yaw < -0.7853982f)
            yaw = -0.7853982f;
        if (pitch > 0.7853982f)
            pitch = 0.7853982f;
        else if (pitch < -0.7853982f)
            pitch = -0.7853982f;
        {
            float halfYaw = yaw * 0.5f, halfPitch = pitch * 0.5f;
            *(float *)(commando_data + 0x198) = halfYaw;
            *(float *)(commando_data + 0x194) = halfPitch;
            *(float *)(commando_data + 0x218) = halfYaw;
            *(float *)(commando_data + 0x214) = halfPitch;
        }
        if ((unsigned int)(*(unsigned char *)(commando_moby + 0x20) - 4) < 2) {
            float yaw2, pitch2, bound;
            normalize_vector_xyz(aim, commando_moby + 0xC0, 1.0f);
            normalize_vector_xyz(velocity, commando_moby + 0xD0, 0.5f);
            add_vector_xyz(aim, aim, position);
            add_vector_xyz(aim, aim, velocity);
            subtract_vector_xyz(aimDelta, savedTarget, aim);
            savedMatrix = matrix;
            transform_vector_by_basis(aimDelta, aimDelta, savedMatrix);
            pitch2 = -fast_arc_tan(vector_length_xy(aimDelta), aimDelta[2]);
            yaw2 = -fast_arc_tan(aimDelta[0], aimDelta[1]);
            bound = 0.7853982f;
            if (pitch2 > bound || pitch2 < (bound = -0.7853982f))
                pitch2 = bound;
            if (yaw2 > 0.34906584f)
                yaw2 = 0.34906584f;
            else if (yaw2 < -0.34906584f)
                yaw2 = -0.34906584f;
            *(float *)(commando_data + 0x294) = pitch2;
            *(float *)(commando_data + 0x298) = yaw2;
        }
    }
    update_joint_animation(commando_moby, commando_data + 0x130, 1, frame_scale_sq * 0.03f, frame_scale_sq * 0.3f);
    update_joint_animation(commando_moby, commando_data + 0x1B0, 2, frame_scale_sq * 0.03f, frame_scale_sq * 0.3f);
    update_joint_animation(commando_moby, commando_data + 0x230, 3, frame_scale_sq * 0.03f, frame_scale_sq * 0.3f);
}
/* Emit randomized joint particles while the moby is active. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A1B58.c: func_L16_002D5CD0), where it is exact; names translated to the US level program. */

extern char *spawn_spark(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");
extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern int fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern void build_moby_bone_transform(void *, int, void *) __asm__("FUN_0020cca8");
extern void random_vector(float *, float, float) __asm__("FUN_L00_00257d78");

void FUN_L16_002d4908(char *commando_moby) {
    float matrix[16], velocity[4], acceleration[4], scatter[4];
    int i;
    if (*(unsigned char *)(commando_moby + 0x31) == 0 || *(unsigned char *)(commando_moby + 0x20) == 10)
        return;
    for (i = 0; i < 2; i++) {
        float speed, z;
        int color1, color2, time1, time2, time3;
        if (random_float_between_alt(0.0f, 1.0f) > 0.75f)
            continue;
        build_moby_bone_transform(commando_moby, i + 4, matrix);
        speed = (random_float_between_alt(-0.2f, 0.2f) + 1.0f) * -3.0f * frame_time;
        normalize_vector_xyz(velocity, matrix + 8, speed);
        {
            float component = (random_float_between_alt(-0.2f, 0.2f) + 1.0f) * -2.0f;
            z = component * frame_time;
        }
        clear_u64_value(acceleration);
        acceleration[2] = z;
        z = random_float_between_alt(-1.5f, 1.5f) * frame_time;
        random_vector(scatter, z, z);
        add_vector_xyz(acceleration, acceleration, scatter);
        velocity[3] = random_float_between_alt(0.2f, 0.2f);
        acceleration[3] = random_float_between_alt(0.4f, 0.4f);
        color1 = fast_tween_color(0x801010FF, 0x801080FF, random_float_between_alt(0.0f, 1.0f));
        color2 = fast_tween_color(0x400000C0, 0x4030FFFF, random_float_between_alt(0.0f, 1.0f));
        time1 = truncate_float_to_s32(
            multiply_global_factor_ed64((random_float_between_alt(-0.1f, 0.1f) + 1.0f) * 5.0f));
        time2 = truncate_float_to_s32(
            multiply_global_factor_ed64((random_float_between_alt(-0.1f, 0.1f) + 1.0f) * 20.0f));
        time3 = truncate_float_to_s32(
            multiply_global_factor_ed64((random_float_between_alt(-0.1f, 0.1f) + 1.0f) * 20.0f));
        spawn_spark(matrix + 12, velocity, acceleration, color1, color2, time1, time2, time3, -1);
    }
}
