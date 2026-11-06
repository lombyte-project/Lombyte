/* Level 07 moby class 0x5c2: mission gated taxi platform. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "rnc/data_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"

/* Taxi command byte (+0xBC); distinct from Moby.state (+0x20). */
enum TaxiCommand {
    TAXI_SHRINK_AND_RESET_TO_PATH_START = 0x0,
    TAXI_WAIT_FOR_HERO_BOARDING = 0x1,
    TAXI_TRAVERSE_PATH = 0x2,
    TAXI_WAIT_FOR_HERO_TO_LEAVE = 0x3
};

#include "qcopy.h"

/* Moves a moby along a path, easing between nodes and tracking the hero. */
extern char *D_L07_001B0530[];
extern float scale_time(float) __asm__("FUN_001f96b0");
extern int   voice_is_playing(void *, int) __asm__("FUN_L00_0028d8c0");
extern int   is_hero_on_moby(void *) __asm__("FUN_L01_00277fb8");
extern int   allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");
extern int   hero_set_state(int, int) __asm__("FUN_L00_002223f8");
extern void  set_hero_idle_state_for_vehicle_mode(void) __asm__("FUN_L00_00227638");
extern void  update_entity_matrix(void *, void *, void *, void *) __asm__("FUN_L00_00260738");
extern void  release_voice_slot(s32) __asm__("FUN_0022d798");
extern char  D_0013E550[];

/* Moves a moby along a path between its two ends, hidden while its gate byte is set. */
extern unsigned char D_L07_0015FC88[] MACRO_ADDR;
void       mission_gated_taxi_platform_update(struct Moby *moby) __asm__("FUN_L07_002cdb28");
extern int allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");
s32        allocate_voice_for_target_entry(s32   entry_index,
                                           s32   flags,
                                           void *target) __asm__("FUN_0022da68");

typedef struct MissionGatedTaxiPlatformVars {
    u8  pad_0[0x20];
    s32 health; /* 0x20: MobyTargetInfo.health, cleared as an int */
    s16 max_health;
    u8  pad_26[0x2];
    u8  hit_weight;
    u8  pad_29[0x15];
    s16 target_flags; /* 0x3E: MobyTargetInfo.flags; target info at pvars+0x20, see moby_target_info.h */
    u8  pad_40[0x20];
    f32 transform[16];
    u8  command;
    u8  pad_A1[0x3];
    f32 t; /* position along the path, 0 to 1 */
    f32 speed;
    f32 target_speed; /* +-1 / travel frames */
    f32 travel_time;  /* seconds per trip */
    s32 path;
    s16 retrigger; /* set when the hero steps off while stopped */
    s16 timer;
    f32 rise; /* height gained this frame */
    u8  pad_C0[0x4];
    s32 voice;
    s32 free_ride; /* nonzero: boarding does not put the hero in the ride state */
} MissionGatedTaxiPlatformVars;

/* Moby class 1474 */
void mission_gated_taxi_platform_update(struct Moby *moby) {
    enum {
        TAXI_PLATFORM_STATE_UNINITIALIZED = 0,
        TAXI_PLATFORM_STATE_INITIALIZED = 1
    };
    MissionGatedTaxiPlatformVars *mission_gated_taxi_vars =
        (MissionGatedTaxiPlatformVars *)moby->pvars;
    char *path;
    float orig[4], delta[4], path_segment_offset[4];
    if (D_L07_0015FC88[moby->mission] != 0xFF) {
        moby->collision = 0;
        moby->flags |= 0x41;
        return;
    }
    if (mission_gated_taxi_vars->path == -1) {
        return;
    }
    path = D_L07_001B0530[mission_gated_taxi_vars->path];
    qcopy(orig, &moby->pos);
    fast_dec_timer_s16(&mission_gated_taxi_vars->timer);
    if (moby->state == TAXI_PLATFORM_STATE_UNINITIALIZED) {
        float f;
        char *pts;
        mission_gated_taxi_vars->hit_weight = 4;
        mission_gated_taxi_vars->target_flags = 5;
        mission_gated_taxi_vars->health = 0;
        mission_gated_taxi_vars->max_health = 0;
        f = 0.0f;
        if ((moby->command = mission_gated_taxi_vars->command) == 0) {
            f = 1.0f;
        }
        mission_gated_taxi_vars->t = f;
        pts = path + 0x10;
        qcopy(&moby->pos, truncate_float_to_s32(f) ? pts + (*(int *)path - 1) * 16 : pts);
        mission_gated_taxi_vars->timer = 0;
        *(int *)&mission_gated_taxi_vars->speed = 0;
        *(int *)&mission_gated_taxi_vars->rise = 0;
        moby->state = TAXI_PLATFORM_STATE_INITIALIZED;
        moby->update_dist = 0xFF;
        moby->draw_dist = 0xFF;
        mission_gated_taxi_vars->voice = -1;
        moby->flags &= 0xFFBE;
        moby->collision = moby->pclass->collision;
    }
    switch (moby->command) {
    case TAXI_SHRINK_AND_RESET_TO_PATH_START: {
        char *class_defaults = moby->pclass;
        float t = moby->scale - *(float *)(class_defaults + 0x24) / 60.0f;
        moby->scale = t;
        if (t < 0.0f) {
            moby->scale = *(float *)(class_defaults + 0x24);
            qcopy(&moby->pos, path + 0x10);
            moby->command = 1;
            mission_gated_taxi_vars->t = 0.0f;
        }
        break;
    }
    case TAXI_WAIT_FOR_HERO_BOARDING:
        if (mission_gated_taxi_vars->retrigger != 0) {
            if (is_hero_on_moby(moby)) {
                if (mission_gated_taxi_vars->free_ride == 0) {
                    if (is_hero_on_moby(moby)) {
                        hero_set_state(0x72, 1);
                    }
                }
                moby->command = 2;
                mission_gated_taxi_vars->target_speed =
                    1.0f / scale_time(mission_gated_taxi_vars->travel_time * 60.0f);
                mission_gated_taxi_vars->retrigger = 0;
            }
        }
        break;
    case TAXI_TRAVERSE_PATH:
        if (mission_gated_taxi_vars->timer != 0 ||
            (mission_gated_taxi_vars->rise < 0.0f &&
             vector_distance_xy(&moby->pos, &hero.motion.pos) < 2.0f &&
             absolute_float(hero.motion.pos.f[2] - moby->pos.z) < 4.0f &&
             moby->pos.z - hero.motion.pos.f[2] > 1.0f)) {
            int r;
            if (mission_gated_taxi_vars->timer == 0) {
                mission_gated_taxi_vars->timer = scale_game_frames(30);
            }
            r = mission_gated_taxi_vars->voice;
            *(int *)&mission_gated_taxi_vars->speed = 0;
            if (r != -1) {
                unsigned char *s = (unsigned char *)((char *)&D_0013E550) + r * 0x70;
                if (*(unsigned char **)(s + 0x88) == moby && s[0x74] != 0) {
                    release_voice_slot(r);
                }
            }
            mission_gated_taxi_vars->voice = -1;
        } else {
            int   i;
            float segment_fraction_work_value, t;
            char *segment_start_position;
            if (!voice_is_playing(moby, mission_gated_taxi_vars->voice)) {
                mission_gated_taxi_vars->voice = allocate_voice_for_target_entry(0, 4, (int)moby);
            }
            mission_gated_taxi_vars->speed += mission_gated_taxi_vars->target_speed * frame_time;
            if (absolute_float(mission_gated_taxi_vars->speed) >
                absolute_float(mission_gated_taxi_vars->target_speed)) {
                mission_gated_taxi_vars->speed = mission_gated_taxi_vars->target_speed;
            }
            mission_gated_taxi_vars->t =
                mission_gated_taxi_vars->t + mission_gated_taxi_vars->speed;
            if (0.5f < absolute_float(mission_gated_taxi_vars->t - 0.5f)) {
                int r;
                if (mission_gated_taxi_vars->free_ride == 0 && is_hero_on_moby(moby)) {
                    set_hero_idle_state_for_vehicle_mode();
                }
                if (0.0f < mission_gated_taxi_vars->target_speed) {
                    moby->command = 3;
                    mission_gated_taxi_vars->t = 1.0f;
                } else {
                    moby->command = 1;
                    mission_gated_taxi_vars->t = 0.0f;
                }
                *(int *)&mission_gated_taxi_vars->speed = 0;
                mission_gated_taxi_vars->timer = scale_game_frames(15);
                r = mission_gated_taxi_vars->voice;
                if (r != -1) {
                    unsigned char *s = (unsigned char *)((char *)&D_0013E550) + r * 0x70;
                    if (*(unsigned char **)(s + 0x88) == moby && s[0x74] != 0) {
                        release_voice_slot(r);
                    }
                }
                mission_gated_taxi_vars->voice = -1;
            }
            segment_fraction_work_value = convert_integer_to_float(*(int *)path - 1);
            i = truncate_float_to_s32(segment_fraction_work_value * mission_gated_taxi_vars->t);
            segment_fraction_work_value = convert_integer_to_float(*(int *)path - 1);
            t = convert_integer_to_float(i);
            segment_fraction_work_value *= mission_gated_taxi_vars->t;
            segment_fraction_work_value -= t;
            if (i == *(int *)path - 1) {
                qcopy(&moby->pos, path + 0x10 + i * 16);
            } else {
                segment_start_position = path + 0x10 + i * 16;
                fast_vec_sub(path_segment_offset, path + 0x20 + i * 16, segment_start_position);
                fast_vec_scale(
                    path_segment_offset, path_segment_offset, segment_fraction_work_value);
                fast_vec_add(&moby->pos, path_segment_offset, segment_start_position);
            }
            mission_gated_taxi_vars->rise = moby->pos.z - orig[2];
        }
        break;
    case TAXI_WAIT_FOR_HERO_TO_LEAVE:
        if (!is_hero_on_moby(moby)) {
            moby->command = 0;
        }
        break;
    }
    fast_vec_sub(delta, &moby->pos, orig);
    update_entity_matrix(mission_gated_taxi_vars->transform, delta, &moby->rot, &moby->rot);
    if (!is_hero_on_moby(moby)) {
        if (2.0f < vector_distance_xy(&moby->pos, &hero.motion.pos)) {
            if (mission_gated_taxi_vars->timer == 0) {
                mission_gated_taxi_vars->retrigger = 1;
            }
        }
    }
}
