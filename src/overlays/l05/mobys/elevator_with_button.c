/* Level 05 moby class elevator_with_button: its camera view and marker ring helpers. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "qcopy.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/audio/voice_pool.h"
#include "sda.h"
#include "rnc/math/random.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/overlay/shared/camera/scripted_camera.h"
#include "rnc/overlay/shared/camera/follow_camera_set_up_spring.h"
#include "rnc/overlay/level_path.h"
#include "rnc/overlay/cuboid.h"

extern int allocate_voice_for_target_entry() __asm__("FUN_0022da68");
extern int voice_is_playing(void *, int) __asm__("FUN_L00_0028d8c0");

#include "rnc/overlay/l05/mobys/elevator_with_button_globals.h"

/* pvars of the lift: it rides between two heights with the hero standing on it. */
typedef struct {
    u8   pad0[0x20];
    char rider[0x40]; /* 0x20: carried-object state for FUN_L00_00260738 */
    f32  top;         /* 0x60 */
    f32  bottom;      /* 0x64 */
    s32  marker;      /* 0x68: marker ring in D_L05_001B0930 and camera id, -1: none */
    s32  pose;        /* 0x6C: index into l05_cuboids */
    f32  progress;    /* 0x70: 0 at the bottom, 1 at the top */
    s32  sound;       /* 0x74: voice slot, -1: none */
    f32  fade;        /* 0x78 */
    s32  timer;       /* 0x7C */
    f32  phase;       /* 0x80: glow pulse angle */
    f32  base_yaw;    /* 0x84 */
    f32  speed;       /* 0x88 */
} LiftVars;

typedef struct {
    u32          handle;
    u8           state;
    u8           pad5[0x13];
    struct Moby *owner; /* 0x18 */
    u8           pad1C[0x54];
} LiftVoice;

/* The pool seen from slot n: its voice field is voices[n]. */
typedef struct {
    u8        header[0x70];
    LiftVoice voice;
} LiftVoiceWindow;

extern u16 D_0014162A NOT_SDA;

extern f32  approach_value(f32 *, f32, f32) __asm__("FUN_00213ed8");
extern f32  advance_accelerated_scalar(f32 *, f32 *, f32, f32, f32, f32) __asm__("FUN_00213f38");
extern void FUN_L00_00260738(char *a, void *b, void *c, void *d);
extern void lift_camera_view(void *, void *, void *) __asm__("FUN_L05_00315ee0");
extern void place_marker_ring(void *) __asm__("FUN_L05_00315f70");

void         elevator_with_button_update(struct Moby *moby) __asm__("FUN_L05_003156d0");
extern float fast_add_rotations(float, float);
extern float fast_subtract_rotations(float, float);
extern int   scale_game_frames(int);

void elevator_with_button_update(struct Moby *moby) {
    enum {
        ELEVATOR_INITIALIZE = 0,
        ELEVATOR_FADE_BEFORE_CAMERA_MOUNT = 1,
        ELEVATOR_MOVE_TO_TOP = 2,
        ELEVATOR_MOVE_TO_BOTTOM = 3,
        ELEVATOR_WAIT_AT_TOP = 4,
        ELEVATOR_WAIT_AT_BOTTOM = 5,
        ELEVATOR_WAIT_TO_RETURN_FROM_TOP = 6,
        ELEVATOR_WAIT_TO_RETURN_FROM_BOTTOM = 7
    };
    LiftVars *vars = (LiftVars *)moby->pvars;
    Vec4      delta;
    Vec4      rot;
    Vec4      eye;
    Vec4      look;
    f32       scale;
    f32       progress_speed_limit;
    f32       max_speed;
    f32       target;
    s32       level;

    fast_vec_scale(&delta, &moby->pos, -1.0f);
    qcopy(&rot, &moby->rot);
    switch (moby->state) {
    case ELEVATOR_INITIALIZE:
        vars->phase = random_angle_radians();
        vars->sound = -1;
        vars->base_yaw = moby->rot.z;
        moby->state = 3;
        break;
    case ELEVATOR_FADE_BEFORE_CAMERA_MOUNT:
        D_0014162A = vars->marker;
        approach_value(&vars->fade, 1.0f, 0.133f);
        elevator_camera_transition_fade = vars->fade;
        if (vars->fade == 1.0f) {
            lift_camera_view(moby, &eye, &look);
            jet_mount(&eye, &look, 1, 0, 0);
            moby->state = moby->command;
            moby->command = 1;
        }
        break;
    case ELEVATOR_MOVE_TO_TOP:
    case ELEVATOR_MOVE_TO_BOTTOM:
        if (!voice_is_playing(moby, vars->sound)) {
            vars->sound = allocate_voice_for_target_entry(0, 4, moby);
        }
        if (moby->command) {
            D_0014162A = vars->marker;
            approach_value(&vars->fade, 0.0f, 0.133f);
            elevator_camera_transition_fade = vars->fade;
            lift_camera_view(moby, &eye, &look);
            set_camera_position(&eye);
            set_camera_rotation(&look);
            scale = 1.0f / (vars->top - vars->bottom);
            progress_speed_limit = frame_time * (moby->command ? 6.0f : 16.0f) * scale;
            max_speed = frame_time_sq * (moby->command ? 6.0f : 16.0f) * scale;
        } else {
            progress_speed_limit = frame_time;
            max_speed = frame_time_sq;
        }
        target = moby->state == 2 ? 1.0f : 0.0f;
        advance_accelerated_scalar(
            &vars->progress, &vars->speed, target, max_speed, max_speed, progress_speed_limit);
        moby->pos.z = (vars->top - vars->bottom) * vars->progress + vars->bottom;
        moby->rot.z =
            fast_subtract_rotations(l05_cuboids[vars->pose].rot.z, vars->base_yaw) * vars->progress;
        moby->rot.z = fast_add_rotations(moby->rot.z, vars->base_yaw);
        if (absolute_float(vars->progress - target) < 0.01f) {
            vars->timer = scale_game_frames(120);
            if (vars->sound != -1) {
                LiftVoiceWindow *window =
                    (LiftVoiceWindow *)((u8 *)&voice_pool + vars->sound * sizeof(LiftVoice));
                if (window->voice.owner == moby && window->voice.state != 0) {
                    release_voice_slot(vars->sound);
                }
            }
            vars->sound = -1;
            if (vars->speed > 0.00125f) {
                vars->speed = 0.00125f;
            } else if (vars->speed < -0.00125f) {
                vars->speed = -0.00125f;
            }
            if (moby->command) {
                if (moby->state == 2) {
                    moby->state = 6;
                } else {
                    moby->state = 7;
                }
            } else {
                if (moby->state == 2) {
                    moby->state = 4;
                } else {
                    moby->state = 5;
                }
            }
        }
        break;
    case ELEVATOR_WAIT_TO_RETURN_FROM_TOP:
    case ELEVATOR_WAIT_TO_RETURN_FROM_BOTTOM:
        if (voice_is_playing(moby, vars->sound)) {
            release_voice_slot(vars->sound);
            vars->sound = -1;
        }
        if (vars->timer > scale_game_frames(30) ||
            vector_distance_xy(&hero.motion.pos, &moby->pos) > 0.5f) {
            fast_dec_timer(&vars->timer);
        }
        if (hero.ground_moby != moby) {
            jet_end_mount(1);
            if (moby->state == 7) {
                moby->state = 5;
            } else {
                moby->state = 4;
            }
            break;
        }
        if (vars->timer != 0) {
            break;
        }
        vars->phase = fast_add_rotations(vars->phase, frame_time * 6.2831855f);
        level = truncate_float_to_s32((fast_sin(vars->phase) * 4.0f - 3.0f) * 128.0f);
        if (level > 0x80) {
            level = 0x80;
        } else if (level < 0x20) {
            level = 0x20;
        }
        moby->glow_color = 0x80000000 | (level << 16) | (level << 8) | level;
        if (hero.ground_moby == moby && hero.air_frames.s == 0 &&
            vector_distance_xy(&hero.motion.pos, &moby->pos) < 0.5f) {
            allocate_voice_for_target_entry(1, 0, moby);
            moby->glow_color = 0x80208020;
            if (moby->state == 7) {
                moby->state = 2;
            } else {
                moby->state = 3;
            }
        }
        break;
    case ELEVATOR_WAIT_AT_TOP:
    case ELEVATOR_WAIT_AT_BOTTOM:
        vars->phase = fast_add_rotations(vars->phase, frame_time * 6.2831855f);
        level = truncate_float_to_s32((fast_sin(vars->phase) * 4.0f - 3.0f) * 128.0f);
        if (level > 0x80) {
            level = 0x80;
        } else if (level < 0x20) {
            level = 0x20;
        }
        moby->glow_color = 0x80000000 | (level << 16) | (level << 8) | level;
        if (hero.ground_moby == moby && hero.air_frames.s == 0 &&
            vector_distance_xy(&hero.motion.pos, &moby->pos) < 0.5f) {
            allocate_voice_for_target_entry(1, 0, moby);
            if (vars->marker != -1) {
                place_marker_ring(moby);
            }
            moby->glow_color = 0x80208020;
            if (moby->state == 5) {
                moby->command = 2;
            } else {
                moby->command = 3;
            }
            moby->state = 1;
            vars->fade = 0.0f;
        } else if (moby->state == 4) {
            f32 dist = vector_distance_xy(&moby->pos, &hero.motion.pos);
            if (absolute_float(hero.ground_z.f - vars->bottom) < 2.0f && dist < 32.0f &&
                dist > 5.0f) {
                moby->command = 0;
                moby->state = 3;
            }
        } else if (moby->state == 5) {
            f32 dist = vector_distance_xy(&moby->pos, &hero.motion.pos);
            if (absolute_float(hero.ground_z.f - vars->top) < 2.0f && dist < 32.0f && dist > 5.0f) {
                moby->command = 0;
                moby->state = 2;
            }
        }
        break;
    }
    fast_vec_add(&delta, &delta, &moby->pos);
    FUN_L00_00260738(vars->rider, &delta, &rot, &moby->rot);
}

/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_0030EB68.c: func_L05_003173A8), where it is exact; names translated to the US level program. */

extern short lift_camera_height_offset_gp __asm__("D_L05_00161EC0") __attribute__((sda));
extern short lift_camera_pitch_degrees_gp __asm__("D_L05_00161EC4") __attribute__((sda));
extern short lift_camera_distance_along_facing_gp __asm__("D_L05_00161EC8") __attribute__((sda));

void FUN_L05_00315ee0(char *lift_moby, char *camera_position, char *camera_rotation) {
    float pitch_degrees;
    fast_vec_normalize(
        camera_position, lift_moby + 0xC0, *(float *)&lift_camera_distance_along_facing_gp);
    fast_vec_add(camera_position, camera_position, lift_moby + 0x10);
    *(float *)(camera_position + 8) =
        *(float *)(camera_position + 8) + *(float *)&lift_camera_height_offset_gp;
    clear_u64_value(camera_rotation);
    pitch_degrees = *(float *)&lift_camera_pitch_degrees_gp;
    *(float *)(camera_rotation + 8) = MOBY(lift_moby)->rot.z;
    *(float *)(camera_rotation + 4) = pitch_degrees * DEG_TO_RAD;
}
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_0030EB68.c: func_L05_00317438), where it is exact; names translated to the US level program. */

void FUN_L05_00315f70(char *moby) {
    int   i = 0;
    char *data = ((char *)MOBY(moby)->pvars);
    char *tab = ((char *)l05_paths[*(int *)(data + 0x68)]);
    float ring_point_angle = 0.0f;
    float step = 6.28318f / (float)(*(int *)tab - 1);
    for (; i < *(int *)tab; i++) {
        char *ring_point_position = tab + 0x10 + i * 16;
        *(float *)ring_point_position = fast_cos(ring_point_angle) * 3.5f;
        *(float *)(ring_point_position + 4) = fast_sin(ring_point_angle) * 3.5f;
        *(int *)(ring_point_position + 8) = 0;
        fast_vec_add(ring_point_position, ring_point_position, moby + 0x10);
        ring_point_angle = fast_add_rotations(ring_point_angle, step);
    }
}
