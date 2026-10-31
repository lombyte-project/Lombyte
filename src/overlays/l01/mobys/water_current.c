/* Level 01 moby class water_current. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"

#include "rnc/math/asm_helpers.h"
#include "rnc/math/fast_math.h"

#include "rnc/overlay/level_path.h"

#include "qcopy.h"

#include "rnc/mobys/moby.h"

/*
 * Path that carries the hero along (a path of l01_paths). While the hero's control mode is 0x11 or
 * 0x12 and he is within `radius` of the path, he is pushed along
 * the path (and toward it when `pull` is set), capped at max_speed; the
 * nearest such path this frame wins through hero.current_path_dist. When he left it
 * (prev control mode 0x12, now 4 or 5) the push is damped instead.
 */
typedef struct {
    Vec4 vel; /* 0x00: path velocity, copied to the hero push (motion.unkF0) */
    u8   pad_10[0x10];
    s32  path;      /* 0x20: index into D_L01_001B0930, -1 none */
    f32  max_speed; /* 0x24 */
    f32  radius;    /* 0x28: reach from the path */
    s16  pull;      /* 0x2C: nonzero pulls the hero onto the path */
    s16  ready;     /* 0x2E: segment lengths computed */
} PathCarrierVars;

/* nearest point of the path to pos, with its segment and position on it */
extern void FUN_L00_0025df68(LevelPath *, void *, void *, f32 *, s32 *, s32, f32, f32, f32);
/* the point a step further along the path; nonzero at the path's end */
extern s32 sample_path(LevelPath *, void *, f32 *, s32 *, s32, f32) __asm__("FUN_L00_0025d808");

void water_current_update(struct Moby *self) __asm__("FUN_L01_002f3120");

/* Moby class 613 */
void water_current_update(struct Moby *water_current_moby) {
    PathCarrierVars *water_current_vars;
    LevelPath       *path;
    Vec4             nearest;
    Vec4             ahead;
    Vec4             current_acceleration;
    Vec4             pull;
    f32              path_segment_index;
    s32              seg;
    s32              at_end;
    f32              hero_path_distance;
    f32              limit;
    f32              current_speed;
    f32              cap;
    s32              i;

    water_current_vars = (PathCarrierVars *)water_current_moby->pvars;
    if (water_current_vars == NULL || water_current_vars->path == -1) {
        return;
    }
    path = l01_paths[water_current_vars->path];
    if (water_current_vars->ready == 0) {
        /* store each segment's length in its start point's w */
        water_current_vars->ready = 1;
        for (i = 0; i < path->count - 1; i++) {
            path->points[i].f[3] = fast_vec_dist(&path->points[i], &path->points[i + 1]);
        }
        path->points[i].f[3] = fast_vec_dist(&path->points[i], &path->points[0]);
        water_current_moby->update_dist = 0xFF;
    }

    if ((u32)(hero.state.control_mode - 0x11) >= 2 && hero.state.prev_control_mode != 0x11 &&
        hero.state.prev_control_mode != 0x12 && hero.state.current != 0x12 &&
        hero.state.prev != 0x12) {
        return;
    }
    FUN_L00_0025df68(
        path, &hero.motion.pos, &nearest, &path_segment_index, &seg, 0, 999.0f, 5.0f, 0.0f);
    at_end = sample_path(path, &ahead, &path_segment_index, &seg, 0, 2.0f);
    hero_path_distance = vector_distance_xy(&hero.motion.pos, &nearest);
    if (water_current_vars->radius < hero_path_distance) {
        return;
    }

    if ((u32)(hero.state.control_mode - 0x11) < 2) {
        if (hero.current_path_dist < hero_path_distance) {
            return;
        }
        hero.current_path_dist = hero_path_distance;
        if (at_end == 0) {
            fast_vec_sub(&current_acceleration, &ahead, &nearest);
            fast_vec_normalize(&current_acceleration, &current_acceleration, frame_time_sq * 7.0f);
            fast_vec_add(water_current_vars, water_current_vars, &current_acceleration);
            water_current_vars->vel.f[2] = 0.0f;
            if (water_current_vars->max_speed < fast_vec_length(water_current_vars)) {
                fast_vec_normalize(
                    water_current_vars, water_current_vars, water_current_vars->max_speed);
            }
            qcopy(&hero.motion.platform_delta, water_current_vars);
            if (water_current_vars->pull != 0) {
                fast_vec_sub(&pull, &nearest, &hero.motion.pos);
                pull.f[2] = 0.0f;
                limit = water_current_vars->max_speed * 0.4f;
                if (limit < fast_vec_length(&pull)) {
                    fast_vec_normalize(&pull, &pull, limit);
                }
                fast_vec_add(&hero.motion.platform_delta, &hero.motion.platform_delta, &pull);
            }
        } else {
            /* past the end: slow down (factor 1 - 0.005 per frame unit) */
            current_speed = fast_vec_length(water_current_vars);
            fast_vec_normalize(water_current_vars,
                               water_current_vars,
                               frame_scale * -0.004999995231628418f * current_speed +
                                   current_speed);
            qcopy(&hero.motion.platform_delta, water_current_vars);
        }
        if (hero.state.control_mode == 0x12 && hero.state.prev == hero.state.control_mode) {
            if (hero.state_timer < scale_game_frames(3)) {
                cap = frame_time * 1.5f;
                if (cap < hero.ground_speed) {
                    hero.ground_speed = cap;
                }
                clear_u64_value(&hero.motion.momentum);
            }
        }
    } else if ((hero.state.prev_control_mode == 0x12 || hero.state.prev2_control_mode == 0x12) &&
               (u32)(hero.state.control_mode - 4) < 2) {
        current_speed = fast_vec_length(water_current_vars);
        fast_vec_normalize(water_current_vars,
                           water_current_vars,
                           frame_scale * -0.014999985694885254f * current_speed + current_speed);
        qcopy(&hero.motion.platform_delta, water_current_vars);
        if (hero.jump_lockout_timer < scale_game_frames(0x23)) {
            hero.jump_lockout_timer = scale_game_frames(0x23);
        }
    }
}
