/* Shared moby class 0x2A7: the grind rail that snaps the hero to its paths and pulls it along. */

#include "types.h"
#include "rnc/overlay/level_path.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"

/* A path of points: the point count, then 16-byte points whose w is the length of the segment to the next one */
typedef struct {
    s32  count;
    u8   pad4[0xC];
    Vec4 pts[1]; /* 0x10 */
} RailPath;

/* pvars of the rail: paths[] are D_L01_001B0930 rows (paths[0] -1: none) */
typedef struct {
    s32 paths[0x18];
    f32 speed; /* 0x60: speed it pulls the hero along with */
    f32 range; /* 0x64: farthest the hero may be from it */
    s32 ready; /* 0x68: segment lengths measured */
    u8  pad6C[4];
    s32 count; /* 0x70 */
} RailVars;

extern void approach_value(float *, float, float) __asm__("FUN_00213ed8");
extern s32  FUN_L00_0025df68(void *, void *, void *, s32 *, f32 *, s32, f32, f32, f32);
extern s32  sample_path(void *, void *, s32 *, f32 *, s32, f32) __asm__("FUN_L00_0025d808");
extern f32 vendor_spring(f32 *value, f32 target, f32 *speed, f32 accel, f32 decel, f32 max) __asm__("FUN_L00_0025b8c0");

extern s32 D_0015ED84;
extern int D_0015ED84; /* no foreign declaration */

/* Grind rail: measures its paths once, then while the hero is in control mode 0x10 snaps it to the nearest path
   point in range and pulls it along, faster on level 1 the more rail is left. */
void rail_hero_motion_update(struct Moby *rail_moby) __asm__("FUN_L01_002f6328");

/* Moby class 679 */
void rail_hero_motion_update(struct Moby *rail_moby) {
    RailVars *rail_vars = (RailVars *)rail_moby->pvars;
    RailPath *path;
    RailPath *best_path;
    Vec4      best_pt;
    Vec4      pt;
    Vec4      rail_step_delta;
    Vec4      pull;
    s32       seg;
    f32       candidate_fraction;
    s32       best_seg;
    f32       best_t;
    f32       drag;
    f32       best;
    f32       candidate_path_distance;
    f32       left;
    f32       rate;
    s32       i, j, k, remaining_segment_index, end;

    if (rail_vars == 0 || rail_vars->paths[0] == -1 || rail_vars->count == 0) {
        return;
    }
    if (rail_vars->ready == 0) {
        rail_vars->ready = 1;
        for (i = 0; i < rail_vars->count; i++) {
            RailPath *measured_path = ((RailPath *)l01_paths[rail_vars->paths[i]]);
            for (j = 0; j < measured_path->count - 1; j++) {
                measured_path->pts[j].f[3] =
                    fast_vec_dist(&measured_path->pts[j], &measured_path->pts[j + 1]);
            }
            measured_path->pts[j].f[3] =
                fast_vec_dist(&measured_path->pts[j], &measured_path->pts[0]);
        }
        rail_moby->update_dist = 0xFF;
    }
    if (hero.state.control_mode != 0x10) {
        return;
    }
    best = 9999.0f;
    best_path = 0;
    for (k = 0; k < rail_vars->count; k++) {
        path = ((RailPath *)l01_paths[rail_vars->paths[k]]);
        FUN_L00_0025df68(
            path, &hero.motion.pos, &pt, &seg, &candidate_fraction, 0, 999.0f, 5.0f, 0.0f);
        candidate_path_distance = vector_distance_xy(&hero.motion.pos, &pt);
        if (candidate_path_distance < best) {
            best_path = path;
            qcopy(&best_pt, &pt);
            best = candidate_path_distance;
            best_seg = seg;
            best_t = candidate_fraction;
        }
    }
    if (best_path == 0) {
        return;
    }
    end = sample_path(best_path, &pt, &best_seg, &best_t, 0, 0.3f);
    if (rail_vars->range < best) {
        return;
    }
    if (D_0015ED84 == 1) {
        left = 0.0f;
        for (remaining_segment_index = best_seg; remaining_segment_index < best_path->count - 1;
             remaining_segment_index++) {
            left += best_path->pts[remaining_segment_index].f[3];
        }
        if (left < 8.0f) {
            rail_vars->speed = frame_time * 3.0f;
        } else if (left < 15.0f) {
            rail_vars->speed = frame_time * 4.0f;
        } else if (left < 30.0f) {
            rail_vars->speed = frame_time * 5.0f;
        } else {
            rail_vars->speed = frame_time * 9.0f;
        }
    }
    if (end == 0) {
        fast_vec_sub(&rail_step_delta, &pt, &best_pt);
        hero.rail_ride_pitch =
            fast_arc_tan(vector_length_xy(&rail_step_delta), rail_step_delta.f[2]);
        hero.rail_yaw = fast_arc_tan(rail_step_delta.f[0], rail_step_delta.f[1]);
        rail_step_delta.i[2] = 0;
        if (hero.unk9DC < rail_vars->speed) {
            approach_value(&hero.unk9DC, rail_vars->speed, frame_time_sq * 6.0f);
        } else {
            approach_value(&hero.unk9DC, rail_vars->speed, frame_time_sq * 6.0f);
        }
        if (0.001f < fast_vec_length(&rail_step_delta)) {
            fast_vec_normalize(&hero.motion.platform_delta, &rail_step_delta, hero.unk9DC);
        }
        fast_vec_sub(&pull, &best_pt, &hero.motion.pos);
        rate = frame_time * 5.0f;
        pull.f[2] = 0.0f;
        drag = -vector_length_xy(&pull);
        vendor_spring(&drag,
                      0.0f,
                      &hero.rail_pull_step,
                      frame_scale_sq * 0.008f,
                      frame_scale_sq * 0.3f,
                      rate);
        {
            f32 horizontal_pull_distance = fast_vec_length(&pull);
            if (horizontal_pull_distance < hero.rail_pull_step) {
                hero.rail_pull_step = horizontal_pull_distance;
            }
            if (hero.rail_pull_step < horizontal_pull_distance) {
                fast_vec_normalize(&pull, &pull, hero.rail_pull_step);
            }
        }
        fast_vec_add(&hero.motion.platform_delta, &hero.motion.platform_delta, &pull);
        hero.motion.platform_delta.f[3] = 0.0f;
        qcopy(&hero.motion.momentum, &hero.motion.platform_delta);
        if (hero.unk1E0 < scale_game_frames(30)) {
            hero.unk1E0 = scale_game_frames(30);
        }
    } else {
        fast_vec_scale(
            &hero.motion.momentum, &hero.motion.momentum, frame_scale * -0.0099999905f + 1.0f);
        qcopy(&hero.motion.platform_delta, &hero.motion.momentum);
        hero.motion.platform_delta.i[3] = 0;
    }
}
