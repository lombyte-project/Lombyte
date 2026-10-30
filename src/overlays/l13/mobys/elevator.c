/* Level 13 elevator moby class and its camera pose. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/hero/hero_set_state.h"

#ifndef NOT_SDA
#define NOT_SDA __attribute__((section(".data")))
#endif
#ifndef MACRO_ADDR
#define MACRO_ADDR __attribute__((section(".sdata")))
#endif
#include "qcopy.h"

typedef struct {
    u8  pad0[0x20];
    u8  path[0x40];  /* 0x20: passed to update_entity_matrix with the moved delta */
    f32 top;         /* 0x60 */
    f32 bottom;      /* 0x64 */
    f32 speed;       /* 0x68 */
    s32 voice;       /* 0x6C */
    f32 blend;       /* 0x70: camera blend, 0 to 1 */
    f32 blend_speed; /* 0x74 */
    f32 pulse;       /* 0x78: colour pulse angle */
    u8  armed;       /* 0x7C: the hero stepped off since the lift stopped */
} LiftVars;

extern char D_L13_001670C0_2b39e0[] __asm__("D_L13_001670C0");

extern float D_0015ED70_2b39e0 __asm__("D_0015ED70");

extern f32 advance_accelerated_scalar(f32 *, f32 *, f32, f32, f32, f32) __asm__("FUN_00213f38");

extern void update_entity_matrix(void *, void *, void *, void *) __asm__("FUN_L00_00260738");

extern void set_camera_position(void *) __asm__("FUN_L00_002ea9d8");

extern void set_camera_rotation(void *) __asm__("FUN_L00_002eaa30");

extern void jet_mount(void *, void *, int, int, int) __asm__("FUN_L00_002eaaa0");

extern void jet_end_mount(int) __asm__("FUN_L00_002eac18");

void compute_elevator_camera_pose(struct Moby *moby,
                                  float       *camera_position,
                                  float       *camera_rotation) __asm__("FUN_L13_002b4060");

extern void
compute_elevator_camera_pose(struct Moby *, float *, float *) __asm__("FUN_L13_002b4060");

void elevator_update(struct Moby *moby) __asm__("FUN_L13_002b39e0");

void elevator_update(struct Moby *moby) {
    enum {
        ELEVATOR_STATE_INITIALIZE = 0,
        ELEVATOR_STATE_BLEND_CAMERA = 1,
        ELEVATOR_STATE_MOVE_TO_TOP = 2,
        ELEVATOR_STATE_MOVE_TO_BOTTOM = 3,
        ELEVATOR_STATE_WAIT_AT_TOP = 4,
        ELEVATOR_STATE_WAIT_AT_BOTTOM = 5,
        ELEVATOR_STATE_WAIT_FOR_HERO_EXIT_AT_TOP = 6,
        ELEVATOR_STATE_WAIT_FOR_HERO_EXIT_AT_BOTTOM = 7
    };
    LiftVars *vars = (LiftVars *)moby->pvars;
    float     delta[4];
    float     rot[4];
    float     camera_position_scratch[4];
    float     camera_pose_scratch[4];
    float     blended_camera_rotation[4];

    fast_vec_scale(delta, &moby->pos, -1.0f);
    qcopy(rot, &moby->rot);
    switch (moby->state) {
    case ELEVATOR_STATE_INITIALIZE:
        vars->pulse = random_angle_radians();
        vars->voice = -1;
        moby->state = 3;
        break;
    case ELEVATOR_STATE_BLEND_CAMERA: {
        float old = vars->blend;
        advance_accelerated_scalar(&vars->blend,
                                   &vars->blend_speed,
                                   1.0f,
                                   D_0015ED70_2b39e0 * 4.0f,
                                   D_0015ED70_2b39e0 * 4.0f,
                                   frame_time * 4.0f);
        if (vars->blend < 1.0f) {
            float t = (1.0f - vars->blend) / (1.0f - old);
            char *base;
            compute_elevator_camera_pose(moby, camera_pose_scratch, blended_camera_rotation);
            fast_vec_sub(camera_position_scratch, D_L13_001670C0_2b39e0, camera_pose_scratch);
            base = D_L13_001670C0_2b39e0 - 0x140;
            fast_vec_scale(camera_position_scratch, camera_position_scratch, t);
            fast_vec_add(camera_pose_scratch, camera_pose_scratch, camera_position_scratch);
            blended_camera_rotation[1] = fast_add_rotations(
                fast_subtract_rotations(*(float *)(base + 0x154), blended_camera_rotation[1]) * t,
                blended_camera_rotation[1]);
            blended_camera_rotation[2] = fast_add_rotations(
                fast_subtract_rotations(*(float *)(base + 0x158), blended_camera_rotation[2]) * t,
                blended_camera_rotation[2]);
            set_camera_position(camera_pose_scratch);
            set_camera_rotation(blended_camera_rotation);
        } else {
            u8 s = moby->command;
            moby->command = 1;
            moby->state = s;
        }
        break;
    }
    case ELEVATOR_STATE_MOVE_TO_TOP:
    case ELEVATOR_STATE_MOVE_TO_BOTTOM: {
        float target;
        if (!voice_is_playing(moby, vars->voice)) {
            vars->voice = allocate_voice_for_target_entry(0, 4, moby);
        }
        if (moby->command != 0) {
            compute_elevator_camera_pose(moby, camera_position_scratch, camera_pose_scratch);
            set_camera_position(camera_position_scratch);
            set_camera_rotation(camera_pose_scratch);
        }
        if (moby->state == 2) {
            target = vars->top;
        } else {
            target = vars->bottom;
        }
        advance_accelerated_scalar(&moby->pos.z,
                                   &vars->speed,
                                   target,
                                   D_0015ED70_2b39e0 * 8.0f,
                                   D_0015ED70_2b39e0 * 8.0f,
                                   frame_time * 40.0f);
        if (absolute_float(moby->pos.z - target) == 0.0f) {
            if (moby->state == 2) {
                moby->state = 6;
            } else {
                moby->state = 7;
            }
            if (moby->command != 0) {
                hero_set_state(HERO_STATE_IDLE, 0);
                jet_end_mount(1);
            }
        }
        break;
    }
    case ELEVATOR_STATE_WAIT_FOR_HERO_EXIT_AT_TOP:
    case ELEVATOR_STATE_WAIT_FOR_HERO_EXIT_AT_BOTTOM:
        if (voice_is_playing(moby, vars->voice)) {
            release_voice_slot(vars->voice);
            vars->voice = -1;
        }
        if (hero.ground_moby != moby || hero.air_frames.s != 0 ||
            vector_distance_xy(&hero.motion.pos, &moby->pos) > 0.5f) {
            u8 s = moby->state;
            vars->armed = 0;
            if (s == 7) {
                moby->state = 5;
            } else {
                moby->state = 4;
            }
        }
        break;
    case ELEVATOR_STATE_WAIT_AT_TOP:
    case ELEVATOR_STATE_WAIT_AT_BOTTOM: {
        int i;
        vars->pulse = fast_add_rotations(vars->pulse, frame_time * 6.2831855f);
        i = truncate_float_to_s32((fast_sin(vars->pulse) * 4.0f - 3.0f) * 128.0f);
        if (i > 0x80) {
            i = 0x80;
        } else if (i < 0x20) {
            i = 0x20;
        }
        {
            int t = (i << 8) | 0x80000000;
            moby->glow_color = (i << 16) | t | i;
        }
        if (hero.ground_moby != moby || hero.air_frames.s != 0 ||
            vector_distance_xy(&hero.motion.pos, &moby->pos) > 1.1f) {
            vars->armed = 1;
        }
        if (vars->armed != 0 && hero.ground_moby == moby && hero.air_frames.s == 0 &&
            vector_distance_xy(&hero.motion.pos, &moby->pos) < 0.5f) {
            moby->glow_color = 0x80208020;
            if (moby->state == 5) {
                moby->command = 2;
            } else {
                moby->command = 3;
            }
            moby->state = 1;
            hero_set_state(0x72, 1);
            jet_mount(D_L13_001670C0_2b39e0, D_L13_001670C0_2b39e0 + 0x10, 1, 0, 0);
            vars->blend = 0.0f;
        } else if (moby->state == 4) {
            char *p = (char *)&hero.motion.pos;
            float d = vector_distance_xy(&moby->pos, p);
            if (absolute_float(*(float *)(p + 0x258) - vars->bottom) < 2.0f && d < 32.0f &&
                d > 5.0f) {
                moby->command = 0;
                moby->state = 3;
                vars->speed = -(frame_time * 5.0f);
            }
        } else if (moby->state == 5) {
            char *p = (char *)&hero.motion.pos;
            float d = vector_distance_xy(&moby->pos, p);
            if (absolute_float(*(float *)(p + 0x258) - vars->top) < 2.0f && d < 32.0f && d > 5.0f) {
                moby->command = 0;
                moby->state = 2;
                vars->speed = frame_time * 5.0f;
            }
        }
        break;
    }
    }
    fast_vec_add(delta, delta, &moby->pos);
    update_entity_matrix(vars->path, delta, rot, &moby->rot);
}

extern unsigned char D_0013E533[];

void compute_elevator_camera_pose(struct Moby *moby,
                                  float       *v,
                                  float       *w) __asm__("FUN_L13_002b4060");

void compute_elevator_camera_pose(struct Moby *moby,
                                  float       *camera_position,

                                  float *camera_rotation) {
    float          camera_offset_azimuth = moby->rot.z;
    float          horizontal_offset_radius = 2.5f;
    float          z = 1.5f;
    unsigned char *x;
    if ((moby->command >= 2 && (moby->command & 1)) || (moby->state >= 2 && (moby->state & 1))) {
        camera_offset_azimuth = fast_subtract_rotations(camera_offset_azimuth, 1.5707964f);
    } else {
        camera_offset_azimuth = fast_add_rotations(camera_offset_azimuth, 1.5707964f);
    }
    if (moby->oclass == 0x15) {
        horizontal_offset_radius = 2.0f;
        z = 1.2f;
    }
    camera_position[0] = fast_cos(camera_offset_azimuth) * horizontal_offset_radius;
    camera_position[1] = fast_sin(camera_offset_azimuth) * horizontal_offset_radius;
    camera_position[2] = z;
    fast_vec_add(camera_position, camera_position, &moby->pos);
    clear_u64_value(camera_rotation);
    x = D_0013E533 + 0xE1D;
    camera_rotation[2] = fast_arc_tan(*(float *)(x + 0xD0) - camera_position[0],
                                      *(float *)(x + 0xD4) - camera_position[1]);
    *(int *)(camera_rotation + 1) = 0;
}

extern void jet_mount(void *, void *, int, int, int) __asm__("FUN_L00_002eaaa0");
