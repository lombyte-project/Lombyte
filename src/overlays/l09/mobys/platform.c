/* Level 09 moby class 0x298: platform. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/overlay/shared/math/rotations/reorient_position_about_pivot.h"
#include "qcopy.h"
#include "sda.h"

/* ticks a falling rock: moves it toward its target, sheds sparks and debris on impact, shakes the camera and removes it */
/* Ported from rac1-decomp (src/overlays/l09_gaspar/vendor_002C2B08.c: func_L09_002EC188), where it is exact; names translated to the US level program. */
typedef struct Moby Moby;

#include "rnc/hero/hero.h"
#include "qzero.h"

/* Ported from rac1-decomp (src/overlays/l09_gaspar/vendor_002C2B08.c: func_L09_002F9A50), where it is exact; names translated to the US level program. */
typedef struct Hero Hero;

typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) Vec_2F9A50;

typedef struct {
    char       pad00[0x20];
    char       f20[0x40]; /* 0x20: passed to func_L00_002617B0 */
    Vec_2F9A50 ang;       /* 0x60: tilt x, y */
    Vec_2F9A50 angvel;    /* 0x70 */
    int        init;      /* 0x80 */
    float      baseZ;     /* 0x84 */
    int        timer;     /* 0x88 */
    int        pad8C;
    float      f90; /* 0x90 */
    float      f94; /* 0x94 */
    float      f98; /* 0x98 */
    float      f9C; /* 0x9C */
} PlatVars;

extern float D_0015ED70;
float        platform_hero_offset_tilt_gain_gp __asm__("D_L09_001619F4") = 0.1f;
float        platform_sink_speed_gp __asm__("D_L09_001619F8") = 0.35f;
float        platform_height_return_gain_gp __asm__("D_L09_001619FC") = 0.3f;

#include "rnc/overlay/l09/mobys/platform_data/platform_update.h"

extern void update_moby_vertical_oscillation(float, float, char *, float *, float *) __asm__("FUN_L00_00262b00");
extern void update_moby_rotation_oscillation(void *, char *, char *, float, float, float) __asm__("FUN_L00_00262b80");
extern int   func_001F9850_2F9A50(int) __asm__("FUN_001f96f8");
extern float func_001F9C78_2F9A50(void *a, void *b) __asm__("FUN_001f9ab0");
extern float func_L00_0025CCF0_2F9A50(void *, float, void *, int, float, float, float) __asm__("FUN_L00_0025bc98");
void platform_update(Moby *moby) __asm__("FUN_L09_002f86a0");

/* Moby class 664 */
void platform_update(Moby *moby) {
    PlatVars  *platform_vars = (*(PlatVars **)&moby->pvars);
    Vec_2F9A50 movement;
    Vec_2F9A50 old_rot;
    Vec_2F9A50 hero_offset;
    Vec_2F9A50 old_angles;
    Vec_2F9A50 old_pos;
    Vec_2F9A50 carried_pos;
    float      sink_level;
    float      old_z;
    float      tilt;

    fast_vec_scale(&movement, &(*(Vec_2F9A50 *)&moby->pos), -1.0f);
    qcopy(&old_rot, &(*(Vec_2F9A50 *)&moby->rot));
    sink_level = platform_vars->baseZ - 0.25f;
    old_z = (*(Vec_2F9A50 *)&moby->pos).z;
    if (platform_vars->init == 0) {
        qcopy(&platform_vars->ang, &(*(Vec_2F9A50 *)&moby->rot));
        qzero(&platform_vars->angvel);
        (*(Vec_2F9A50 *)&moby->pos).z = (*(Vec_2F9A50 *)&moby->pos).z - 0.3f;
        platform_vars->init = 1;
        platform_vars->baseZ = (*(Vec_2F9A50 *)&moby->pos).z;
    }
    if (hero.air_frames.s == 0 && ((int)hero.ground_moby) == (int)moby) {
        if (platform_vars->timer == 0) {
            allocate_voice_for_target_entry(0, 0, moby);
        }
        if (platform_vars->timer < func_001F9850_2F9A50(30)) {
            platform_vars->timer = func_001F9850_2F9A50(30);
        }
    }
    if (platform_vars->timer != 0) {
        float side_dot, front_dot, pitch_target;
        float deg_to_rad;
        fast_dec_timer(&platform_vars->timer);
        fast_vec_sub(&hero_offset, ((unsigned char *)&hero + 0x80), &(*(Vec_2F9A50 *)&moby->pos));
        deg_to_rad = 0.01745329238474369f;
        side_dot = func_001F9C78_2F9A50(&hero_offset, &(*(Vec_2F9A50 *)&moby->rot_row0));
        front_dot = func_001F9C78_2F9A50(&hero_offset, &(*(Vec_2F9A50 *)&moby->rot_row1));
        tilt = platform_hero_offset_tilt_gain_gp;
        pitch_target = tilt * side_dot;
        *(u128 *)&old_angles = *(u128 *)&platform_vars->ang;
        *(u128 *)&old_pos = *(u128 *)&moby->pos;
        func_L00_0025CCF0_2F9A50(&platform_vars->ang.x,
                                 -tilt * front_dot,
                                 &platform_vars->angvel.x,
                                 0,
                                 platform_active_tilt_error_gain_gp * deg_to_rad * D_0015ED70,
                                 platform_active_tilt_velocity_damping_gp * frame_scale_sq,
                                 platform_active_tilt_velocity_limit_gp * deg_to_rad * frame_time);
        func_L00_0025CCF0_2F9A50(&platform_vars->ang.y,
                                 pitch_target,
                                 &platform_vars->angvel.y,
                                 0,
                                 platform_active_tilt_error_gain_gp * deg_to_rad * D_0015ED70,
                                 platform_active_tilt_velocity_damping_gp * frame_scale_sq,
                                 platform_active_tilt_velocity_limit_gp * deg_to_rad * frame_time);
        (*(Vec_2F9A50 *)&moby->pos).z =
            (*(Vec_2F9A50 *)&moby->pos).z - platform_sink_speed_gp * frame_time;
        if (sink_level < old_z && (*(Vec_2F9A50 *)&moby->pos).z <= sink_level) {
            allocate_voice_for_target_entry(1, 0, moby);
        }
        reorient_position_about_pivot(((unsigned char *)&hero + 0x80),
                                      &carried_pos,
                                      &(*(Vec_2F9A50 *)&moby->pos),
                                      &old_angles,
                                      &platform_vars->ang);
        fast_vec_sub((unsigned char *)&hero + 0xF0, &carried_pos, ((unsigned char *)&hero + 0x80));
        hero.motion.platform_delta.f[2] =
            hero.motion.platform_delta.f[2] + ((*(Vec_2F9A50 *)&moby->pos).z - old_pos.z);
    } else {
        float deg_to_rad = 0.01745329238474369f;
        func_L00_0025CCF0_2F9A50(&platform_vars->ang.x,
                                 0.0f,
                                 &platform_vars->angvel.x,
                                 0,
                                 platform_return_tilt_error_gain_gp * deg_to_rad * D_0015ED70,
                                 platform_return_tilt_velocity_damping_gp * frame_scale_sq,
                                 platform_return_tilt_velocity_limit_gp * deg_to_rad * frame_time);
        func_L00_0025CCF0_2F9A50(&platform_vars->ang.y,
                                 0.0f,
                                 &platform_vars->angvel.y,
                                 0,
                                 platform_return_tilt_error_gain_gp * deg_to_rad * D_0015ED70,
                                 platform_return_tilt_velocity_damping_gp * frame_scale_sq,
                                 platform_return_tilt_velocity_limit_gp * deg_to_rad * frame_time);
        (*(Vec_2F9A50 *)&moby->pos).z = (*(Vec_2F9A50 *)&moby->pos).z +
                                        platform_height_return_gain_gp * frame_time *
                                            (platform_vars->baseZ - (*(Vec_2F9A50 *)&moby->pos).z);
        if (platform_vars->baseZ < (*(Vec_2F9A50 *)&moby->pos).z) {
            (*(Vec_2F9A50 *)&moby->pos).z = platform_vars->baseZ;
        }
        if (old_z < sink_level && sink_level <= (*(Vec_2F9A50 *)&moby->pos).z) {
            allocate_voice_for_target_entry(2, 0, moby);
        }
    }
    update_moby_vertical_oscillation(
        0.14f, frame_time * 0.5235987901687622f, moby, &platform_vars->f98, &platform_vars->f9C);
    update_moby_rotation_oscillation(moby,
                                     &platform_vars->f90,
                                     &platform_vars->f94,
                                     0.0872664600610733f,
                                     frame_time * 0.3141592741012573f,
                                     frame_time * 0.48869219422340393f);
    (*(Vec_2F9A50 *)&moby->rot).x =
        fast_add_rotations(platform_vars->ang.x, (*(Vec_2F9A50 *)&moby->rot).x);
    (*(Vec_2F9A50 *)&moby->rot).y =
        fast_add_rotations(platform_vars->ang.y, (*(Vec_2F9A50 *)&moby->rot).y);
    fast_vec_add(&movement, &movement, &(*(Vec_2F9A50 *)&moby->pos));
    update_entity_matrix(platform_vars->f20, &movement, &old_rot, &(*(Vec_2F9A50 *)&moby->rot));
}
