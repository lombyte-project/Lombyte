/* Level 03 moby class taxi (0x330): the rail taxi that carries Ratchet along its routes, its rail path flight and its bobbing. */
#include "types.h"
#include "rnc/overlay/level_path.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/level_globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/cutscenes/update_npc_dialogue.h"
#include "rnc/ui/help/try_set_help_message.h"
#include "rnc/hero/hero.h"
#include "qcopy.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/overlay/level_save_state.h"
#include "rnc/overlay/l03/mobys/taxi.h"
#include "rnc/overlay/l03/mobys/taxi_globals.h"

/* Test the observed halfword and byte range without inferring object identity. */
s32 is_taxi_in_round_trip_phase(u8 *object) __asm__("FUN_L03_002d30d8");

s32 is_taxi_in_round_trip_phase(u8 *tested_moby) {
    if (*(s16 *)(tested_moby + 0xa6) != 0x330) {
        return 0;
    }
    return (u32)(tested_moby[0x20] - 2) < 5;
}
/* Ported from rac1-decomp (src/overlays/l03_kerwan/vendor_002CB280.c: func_L03_002D44C8), where it is exact; names translated to the US level program. */

void request_taxi_return_help_message(void *taxi_moby, int mode) __asm__("FUN_L03_002d3100");

void request_taxi_return_help_message(void *taxi_moby, int mode) {
    if (mode == 1) {
        try_set_help_message(8, 0xBC9);
    } else {
        try_set_help_message(8, 0xBC9);
    }
}

void       update_taxi_bobbing_and_rocking(struct Moby *taxi_moby) __asm__("FUN_L03_002d3c40");
extern f32 frame_time_sq_macro __asm__("D_0015ED70") MACRO_ADDR;

/* Pvars of a rail taxi: up to five routes, each called by a trigger volume and
   made of an outbound path, a return path and an optional onward path. */
struct RailTaxiVars {
    u8  pad0[0x20];
    u8  coll[0x40]; /* 0x20: collision record moved with the taxi */
    f32 speed;      /* 0x60: FUN_L03_002d3918 state (RailFlyerVars) */
    s32 idx;        /* 0x64 */
    f32 yaw_vel;    /* 0x68 */
    f32 pitch_vel;  /* 0x6C */
    f32 roll_vel;   /* 0x70 */
    s32 route;      /* 0x74: route kept while the hero stays aboard (-1: none) */
    s32 snd;        /* 0x78: travel sound voice */
    s32 active;     /* 0x7C */
    s32 volume[5];  /* 0x80: trigger clip volume of each route (-1: none) */
    s32 out[5];     /* 0x94: rail path index of each route's outbound leg (-1: none) */
    s32 back[5];    /* 0xA8: return leg */
    s32 onward[5];  /* 0xBC: optional leg taken when the hero walks away (-1: none) */
    s32 help[5];    /* 0xD0: help message mode at each route's stop (FUN_L03_002d3100) */
    s32 unkE4;      /* 0xE4: set to -1 on arrival */
};

s32        taxi_path_point_debug_draw_enabled_gp __asm__("D_L03_00161B04") = 0;
extern f32 rail_flyer_max_speed_gp __asm__("D_L03_00161B08") __attribute__((sda));
extern f32 rail_flyer_pitch_acceleration_degrees_gp __asm__("D_L03_00161B10") __attribute__((sda));
extern f32 rail_flyer_roll_acceleration_degrees_gp __asm__("D_L03_00161B18") __attribute__((sda));

extern s32 D_L03_0015F594 __attribute__((sda));
extern void
draw_number(void *, struct Moby *, s32, u32, s32, s32, s32, f32) __asm__("FUN_L01_00285768");
extern int  is_point_inside_clip_volume(void *, int) __asm__("FUN_00214720");
extern int  is_hero_on_moby(void *) __asm__("FUN_L01_00277fb8");
extern void request_hero_target_pose(void *, float, int) __asm__("FUN_L00_00233ee8");
extern s32  get_hero_target_pose_status(void) __asm__("FUN_L00_00233f38");
extern void update_entity_matrix(void *, void *, void *, void *) __asm__("FUN_L00_00260738");
extern f32  approach_angle(f32 *, f32, f32 *, f32, f32, f32) __asm__("FUN_L00_0025be00");

/* Rail taxi: waits at its stop until the hero enters a route's trigger volume,
   flies the outbound path, lets the hero off, flies back (or onward) and returns
   to waiting. With the debug flag set it draws its first route's paths. */
void rail_taxi_update(struct Moby *taxi_moby) __asm__("FUN_L03_002d3198");

/* Moby class 816 */
void rail_taxi_update(struct Moby *taxi_moby) {
    enum {
        TAXI_INITIALIZE = 0,
        TAXI_WAIT_FOR_ROUTE_TRIGGER = 1,
        TAXI_FOLLOW_OUTBOUND_PATH = 2,
        TAXI_WAIT_FOR_RETURN_REQUEST = 3,
        TAXI_WAIT_FOR_RETURN_DEPARTURE = 4,
        TAXI_FOLLOW_RETURN_PATH = 5,
        TAXI_WAIT_FOR_HERO_TO_MOVE_AWAY = 6,
        TAXI_FOLLOW_ONWARD_PATH = 7
    };
    struct RailTaxiVars *taxi_vars = (struct RailTaxiVars *)taxi_moby->pvars;
    Vec4                 oldPos, oldRot, move;
    LevelPath           *route_path;
    s32                  i;
    s32                  route_index;

    set_moby_dialogue_record_value(taxi_moby, is_taxi_in_round_trip_phase((u8 *)taxi_moby));
    if (taxi_path_point_debug_draw_enabled_gp != 0) {
        for (i = 0; i < (l03_paths[taxi_vars->out[0]])->count; i++) {
            draw_number(&(l03_paths[taxi_vars->out[0]])->points[i],
                        taxi_moby,
                        4,
                        0x800000FF,
                        0x7F,
                        2,
                        0xFF,
                        0.333f);
        }
        for (i = 0; i < (l03_paths[taxi_vars->back[0]])->count; i++) {
            draw_number(&(l03_paths[taxi_vars->back[0]])->points[i],
                        taxi_moby,
                        4,
                        0x8000FF00,
                        0x7F,
                        2,
                        0xFF,
                        0.333f);
        }
        for (i = 0; i < (l03_paths[taxi_vars->onward[0]])->count; i++) {
            draw_number(&(l03_paths[taxi_vars->onward[0]])->points[i],
                        taxi_moby,
                        4,
                        0x80FF0000,
                        0x7F,
                        2,
                        0xFF,
                        0.333f);
        }
    }
    qcopy(&oldPos, &taxi_moby->pos);
    qcopy(&oldRot, &taxi_moby->rot);
    switch (taxi_moby->state) {
    case TAXI_INITIALIZE:
        if (taxi_vars->out[0] != -1) {
            qcopy_nc(&taxi_moby->pos, &(l03_paths[taxi_vars->out[0]])->points[0]);
            taxi_moby->rot.z =
                fast_arc_tan((l03_paths[taxi_vars->out[0]])->points[1].f[0] - taxi_moby->pos.x,
                             (l03_paths[taxi_vars->out[0]])->points[1].f[1] - taxi_moby->pos.y);
        } else if (taxi_vars->back[0] != -1) {
            qcopy_nc(&taxi_moby->pos, &(l03_paths[taxi_vars->back[0]])->points[0]);
            taxi_moby->rot.z =
                fast_arc_tan((l03_paths[taxi_vars->back[0]])->points[1].f[0] - taxi_moby->pos.x,
                             (l03_paths[taxi_vars->back[0]])->points[1].f[1] - taxi_moby->pos.y);
        }
        taxi_moby->draw_dist = 0;
        taxi_moby->update_dist = 0xFF;
        taxi_vars->route = -1;
        taxi_moby->state = 1;
        taxi_moby->collision = 0;
        if (l03_save_state.cleared[(s16)taxi_moby->save_id] != 0 ||
            (D_0014C190_257218[current_level_index][(s16)taxi_moby->save_id >> 5] >>
             (taxi_moby->save_id & 0x1F)) &
                1) {
            mark_moby_for_removal(taxi_moby);
            return;
        }
        break;
    case TAXI_WAIT_FOR_ROUTE_TRIGGER:
        if (taxi_vars->active == 0) {
            break;
        }
        for (route_index = 0; route_index < 5; route_index++) {
            if ((taxi_vars->volume[route_index] != -1 || taxi_vars->route == route_index) &&
                (is_point_inside_clip_volume(&hero.motion.pos, taxi_vars->volume[route_index]) ||
                 taxi_vars->route == route_index)) {
                taxi_moby->collision = taxi_moby->pclass->collision;
                if (taxi_vars->out[route_index] != -1) {
                    route_path = (l03_paths[taxi_vars->out[route_index]]);
                    qcopy(&taxi_moby->pos, &route_path->points[0]);
                    taxi_moby->rot.z = fast_arc_tan(route_path->points[1].f[0] - taxi_moby->pos.x,
                                                    route_path->points[1].f[1] - taxi_moby->pos.y);
                } else if (taxi_vars->back[route_index] != -1) {
                    route_path = (l03_paths[taxi_vars->back[route_index]]);
                    qcopy(&taxi_moby->pos, &route_path->points[0]);
                    taxi_moby->rot.z = fast_arc_tan(route_path->points[1].f[0] - taxi_moby->pos.x,
                                                    route_path->points[1].f[1] - taxi_moby->pos.y);
                }
                taxi_moby->command = route_index;
                taxi_moby->state = 2;
                taxi_moby->draw_dist = 0x80;
                taxi_vars->idx = 1;
                taxi_vars->speed = rail_flyer_max_speed_gp * frame_time;
                taxi_vars->route = -1;
                break;
            }
        }
        break;
    case TAXI_FOLLOW_OUTBOUND_PATH:
        if (taxi_vars->out[taxi_moby->command] == -1 ||
            advance_moby_along_rail_path(
                taxi_moby, taxi_vars->out[taxi_moby->command], &taxi_vars->snd)) {
            taxi_moby->state = 3;
            taxi_vars->unkE4 = -1;
        }
        break;
    case TAXI_WAIT_FOR_RETURN_REQUEST:
        approach_angle(&taxi_moby->rot.y,
                       0.0f,
                       &taxi_vars->pitch_vel,
                       rail_flyer_pitch_acceleration_degrees_gp * DEG_TO_RAD * frame_time_sq_macro,
                       rail_flyer_pitch_acceleration_degrees_gp * DEG_TO_RAD * frame_time_sq_macro,
                       frame_time * 12.566371f);
        approach_angle(&taxi_moby->rot.x,
                       0.0f,
                       &taxi_vars->roll_vel,
                       rail_flyer_roll_acceleration_degrees_gp * DEG_TO_RAD * frame_time_sq_macro,
                       rail_flyer_roll_acceleration_degrees_gp * DEG_TO_RAD * frame_time_sq_macro,
                       frame_time * 12.566371f);
        if (hero.ground_moby == taxi_moby && hero.air_frames.s == 0) {
            request_taxi_return_help_message(taxi_moby, taxi_vars->help[taxi_moby->command]);
            if ((D_0013CAE4 & 0x10) && D_L03_0015F594 == 8) {
                request_hero_target_pose(&taxi_moby->pos, taxi_moby->rot.z, 0);
                taxi_moby->state = 4;
                taxi_vars->idx = 1;
            }
        }
        break;
    case TAXI_WAIT_FOR_RETURN_DEPARTURE:
        if (!get_hero_target_pose_status()) {
            taxi_moby->state = 5;
        }
        break;
    case TAXI_FOLLOW_RETURN_PATH:
        if (is_hero_on_moby(taxi_moby)) {
            hero.jump_lock_timer = 4;
            hero.unk1F4 = 4;
        }
        if (advance_moby_along_rail_path(
                taxi_moby, taxi_vars->back[taxi_moby->command], &taxi_vars->snd)) {
            taxi_moby->state = 6;
        }
        break;
    case TAXI_WAIT_FOR_HERO_TO_MOVE_AWAY:
        approach_angle(&taxi_moby->rot.y,
                       0.0f,
                       &taxi_vars->pitch_vel,
                       rail_flyer_pitch_acceleration_degrees_gp * DEG_TO_RAD * frame_time_sq_macro,
                       rail_flyer_pitch_acceleration_degrees_gp * DEG_TO_RAD * frame_time_sq_macro,
                       frame_time * 12.566371f);
        approach_angle(&taxi_moby->rot.x,
                       0.0f,
                       &taxi_vars->roll_vel,
                       rail_flyer_roll_acceleration_degrees_gp * DEG_TO_RAD * frame_time_sq_macro,
                       rail_flyer_roll_acceleration_degrees_gp * DEG_TO_RAD * frame_time_sq_macro,
                       frame_time * 12.566371f);
        if (taxi_vars->onward[taxi_moby->command] != -1 &&
            fast_vec_dist(&taxi_moby->pos, &hero.motion.pos) > 15.0f) {
            taxi_moby->state = 7;
            taxi_vars->idx = 1;
        } else if (taxi_vars->onward[taxi_moby->command] == -1 &&
                   vector_distance_xy(&taxi_moby->pos, &hero.motion.pos) > 5.0f) {
            taxi_moby->state = 1;
        }
        break;
    case TAXI_FOLLOW_ONWARD_PATH:
        if (advance_moby_along_rail_path(
                taxi_moby, taxi_vars->onward[taxi_moby->command], &taxi_vars->snd)) {
            taxi_moby->draw_dist = 0;
            taxi_moby->state = 1;
            taxi_moby->collision = 0;
        }
        break;
    }
    update_taxi_bobbing_and_rocking(taxi_moby);
    fast_vec_sub(&move, &taxi_moby->pos, &oldPos);
    update_entity_matrix(taxi_vars->coll, &move, &oldRot, &taxi_moby->rot);
}

/* Pvars of a moby flying along a rail path. */
typedef struct {
    u8  pad0[0x60];
    f32 speed;     /* 0x60 */
    s32 idx;       /* 0x64: next path point */
    f32 yaw_vel;   /* 0x68 */
    f32 pitch_vel; /* 0x6C */
    f32 roll_vel;  /* 0x70 */
} RailFlyerVars;

extern f32  rail_flyer_speed_acceleration_gp __asm__("D_L03_00161B0C") __attribute__((sda));
extern f32  rail_flyer_yaw_acceleration_degrees_gp __asm__("D_L03_00161B14") __attribute__((sda));
extern f32  D_L03_00161B1C __attribute__((sda));
extern f32  advance_accelerated_scalar(f32 *, f32, f32, f32, f32, f32 *) __asm__("FUN_00213f38");
extern s32  sound_is_playing(void *, s32) __asm__("FUN_L00_0028d8c0");
extern s32  play_moby_sound(s32, s32, void *, s32) __asm__("FUN_L00_0028dc90");
extern void release_voice_slot(s32) __asm__("FUN_0022d798");

/* Moves m along rail path `path`, easing speed toward the end and banking into turns;
   keeps the travel sound playing and releases it on arrival. Returns 1 at the last point. */
s32 advance_moby_along_rail_path(struct Moby *rail_moby,
                                 s32          path,
                                 s32         *snd) __asm__("FUN_L03_002d3918");

s32 advance_moby_along_rail_path(struct Moby *rail_moby, s32 path, s32 *snd) {
    RailFlyerVars *rail_flyer_vars = (RailFlyerVars *)rail_moby->pvars;
    LevelPath    **pp = &l03_paths[path];
    Vec4f          target_path_point, movement_delta;
    f32            acc;
    f32            target_pitch;
    f32            distance_to_path_end;
    s32            done;
    s32            sound_voice;

    done = 0;
    acc = 0.0f;
    qcopy(&target_path_point, &(*pp)->points[rail_flyer_vars->idx]);
    fast_vec_sub(&movement_delta, &target_path_point, &rail_moby->pos);
    if (fast_vec_length(&movement_delta) < rail_flyer_max_speed_gp * frame_time * 2.0f) {
        rail_flyer_vars->idx++;
        done = rail_flyer_vars->idx == (*pp)->count;
    }
    distance_to_path_end = fast_vec_dist(&rail_moby->pos, &(*pp)->points[(*pp)->count - 1]);
    advance_accelerated_scalar(&acc,
                               distance_to_path_end,
                               rail_flyer_speed_acceleration_gp * frame_time_sq_macro,
                               rail_flyer_speed_acceleration_gp * frame_time_sq_macro,
                               rail_flyer_max_speed_gp * frame_time,
                               &rail_flyer_vars->speed);
    fast_vec_normalize(&movement_delta, &movement_delta, rail_flyer_vars->speed);
    fast_vec_add(&rail_moby->pos, &movement_delta, &rail_moby->pos);
    target_pitch = -fast_arc_tan(vector_length_xy(&movement_delta), movement_delta.z);
    if (target_pitch > 0.2617994f)
        target_pitch = 0.2617994f;
    else if (target_pitch < -0.2617994f)
        target_pitch = -0.2617994f;
    approach_angle(&rail_moby->rot.y,
                   target_pitch,
                   &rail_flyer_vars->pitch_vel,
                   rail_flyer_pitch_acceleration_degrees_gp * DEG_TO_RAD * frame_time_sq_macro,
                   rail_flyer_pitch_acceleration_degrees_gp * DEG_TO_RAD * frame_time_sq_macro,
                   frame_time * 12.566371f);
    approach_angle(&rail_moby->rot.z,
                   fast_arc_tan(target_path_point.x - rail_moby->pos.x,
                                target_path_point.y - rail_moby->pos.y),
                   &rail_flyer_vars->yaw_vel,
                   rail_flyer_yaw_acceleration_degrees_gp * DEG_TO_RAD * frame_time_sq_macro,
                   rail_flyer_yaw_acceleration_degrees_gp * DEG_TO_RAD * frame_time_sq_macro,
                   frame_time * 12.566371f);
    approach_angle(&rail_moby->rot.x,
                   rail_flyer_vars->yaw_vel * rail_flyer_vars->speed * D_L03_00161B1C,
                   &rail_flyer_vars->roll_vel,
                   rail_flyer_roll_acceleration_degrees_gp * DEG_TO_RAD * frame_time_sq_macro,
                   rail_flyer_roll_acceleration_degrees_gp * DEG_TO_RAD * frame_time_sq_macro,
                   frame_time * 12.566371f);
    sound_voice = *snd;
    if (!done) {
        if (sound_is_playing(rail_moby, sound_voice) == 0)
            *snd = play_moby_sound(0, 4, rail_moby, 0x330);
    } else {
        if (sound_voice != -1) {
            VoicePoolWindow *voice_slot =
                (VoicePoolWindow *)((u8 *)&voice_pool + sound_voice * 0x70);
            if ((struct Moby *)voice_slot->voice.owner == rail_moby && voice_slot->voice.state)
                release_voice_slot(sound_voice);
        }
        *snd = -1;
    }
    return done;
}
/* Sets two speed pairs on a moby's data from level constants scaled by degrees-to-radians and the frame scale. */
/* Ported from rac1-decomp (src/overlays/l03_kerwan/vendor_002CB280.c: func_L03_002D5008), where it is exact; names translated to the US level program. */

extern float taxi_bobbing_amplitude_gp __asm__("D_L03_00161AF0") __attribute__((sda));
extern float taxi_bobbing_phase_rate_degrees_gp __asm__("D_L03_00161AF4") __attribute__((sda));
extern float taxi_rocking_amplitude_gp __asm__("D_L03_00161AF8") __attribute__((sda));
extern float taxi_rocking_first_phase_rate_degrees_gp __asm__("D_L03_00161AFC")
    __attribute__((sda));
extern float taxi_rocking_second_phase_rate_degrees_gp __asm__("D_L03_00161B00")
    __attribute__((sda));
extern void update_moby_vertical_oscillation(float, float, char *, float *, float *) __asm__("FUN_L00_00262b00");
extern void update_moby_rotation_oscillation(char *, char *, char *, float, float, float) __asm__("FUN_L00_00262b80");

void update_taxi_bobbing_and_rocking(struct Moby *taxi_moby) {
    char *moby_vars = (char *)taxi_moby->pvars;
    update_moby_vertical_oscillation(taxi_bobbing_amplitude_gp,
                                     taxi_bobbing_phase_rate_degrees_gp * DEG_TO_RAD * frame_time,
                                     taxi_moby,
                                     (float *)(moby_vars + 0xEC),
                                     (float *)(moby_vars + 0xE8));
    update_moby_rotation_oscillation(
        taxi_moby,
        moby_vars + 0xF0,
        moby_vars + 0xF4,
        taxi_rocking_amplitude_gp,
        taxi_rocking_first_phase_rate_degrees_gp * DEG_TO_RAD * frame_time,
        taxi_rocking_second_phase_rate_degrees_gp * DEG_TO_RAD * frame_time);
}
