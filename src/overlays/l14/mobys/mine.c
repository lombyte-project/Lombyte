/* Level 14 mine moby class (643): follows a looping path, spinning and pulsing its glow, and explodes on contact. */
#include "types.h"
#include "rnc/overlay/level_path.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/math/random.h"
#include "rnc/overlay/l14/mobys/mine_globals.h"

extern void debug_print(const char *, short) __asm__("FUN_001e93b0");

extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");

typedef struct {
    u8  pad0[0x20];
    f32 health[0x10]; /* 0x20 */
    s32 path;         /* 0x60 */
    s32 segment;      /* 0x64 */
    f32 along;        /* 0x68 */
    f32 dir;          /* 0x6C */
    f32 vel[3];       /* 0x70 */
    f32 phase;        /* 0x7C */
    f32 spin[3];      /* 0x80 */
} L14MineVars;

typedef struct {
    s32 count;
    s32 pad4[3];
    struct {
        f32 pos[3];
        f32 length;
    } points[1];
} L14MinePath;

typedef struct {
    u8           pad0[0x20];
    struct Moby *moby; /* 0x20 */
} L14MineHit;

extern char mine_missing_path_error_format[] __asm__("D_L14_001FCA98");

extern char mine_empty_path_error_format[] __asm__("D_L14_001FCAD8");

extern f32 mine_path_step_rate_gp __asm__("D_L14_00161C44") __attribute__((sda));

extern f32 mine_glow_pulse_phase_step_gp __asm__("D_L14_00161C48") __attribute__((sda));

extern float FUN_L00_00257c48(float, float);

extern int
FUN_L00_0025d808_mine(void *, float *, int *, float *, int, float) __asm__("FUN_L00_0025d808");

extern float FUN_L00_0025b8c0(float *p, float *v, float t, float u1, float u2, float eps);

extern s32
fun_00213928(struct Moby *, void *, f32 *, s32, s32 *, s32, s32, s32) __asm__("FUN_00213928");

extern void mine_explode(void *,
                         void *,
                         void *,
                         float,
                         float,
                         int,
                         int,
                         int,
                         float,
                         float,
                         float,
                         float,
                         int,
                         float,
                         int,
                         int,
                         int,
                         int) __asm__("FUN_L00_0025e450");

extern int mine_ftoi(float) __asm__("FUN_001fa6d0");

void update_looping_path_mine(struct Moby *moby) __asm__("FUN_L14_002ec810");

void update_looping_path_mine(struct Moby *moby) {
    enum {
        MINE_STATE_INITIALIZE = 0,
        MINE_STATE_FOLLOW_PATH = 1
    };
    L14MineVars *mine_vars = (L14MineVars *)moby->pvars;
    float        path_target_position[4];
    Vec4         vel;
    s32          count;
    L14MinePath *path;
    L14MineHit  *hit;
    int          i;
    float        glow_pulse_sine;
    int          glow_red_intensity;
    int          glow_green_blue_intensity;

    switch (moby->state) {
    case MINE_STATE_INITIALIZE:
        if (mine_vars->path == -1) {
            debug_print(mine_missing_path_error_format, moby->save_id);
            mark_moby_for_removal(moby);
            return;
        }
        path = ((L14MinePath *)l14_paths[mine_vars->path]);
        if (path->count == 0) {
            debug_print(mine_empty_path_error_format, moby->save_id);
            mark_moby_for_removal(moby);
            return;
        }
        for (i = 0; i < path->count; i++) {
            f32 path_segment_length = fast_vec_dist(
                &((f32 *)path)[i * 4 + 4], &((f32 *)path)[((i + 1) % path->count) * 4 + 4]);
            char *pt = (char *)path + i * 16;
            *(f32 *)(pt + 0x1C) = path_segment_length;
        }
        mine_vars->segment = 0;
        mine_vars->along = 0;
        mine_vars->dir = random_integer_below(2) != 0 ? 1.0f : -1.0f;
        qcopy(&moby->pos, path->points[0].pos);
        moby->state = 1;
        mine_vars->spin[0] = random_float_between(0.05235988f, 0.10471976f);
        mine_vars->spin[1] = random_float_between(0.034906585f, 0.06981317f);
        mine_vars->spin[2] = random_float_between(0.017453292f, 0.034906585f);
        clear_u64_value(mine_vars->vel);
        mine_vars->phase = FUN_L00_00257c48(0.0f, 3.1415927f);
        break;
    case MINE_STATE_FOLLOW_PATH:
        FUN_L00_0025d808_mine(((L14MinePath *)l14_paths[mine_vars->path]),
                              path_target_position,
                              &mine_vars->segment,
                              &mine_vars->along,
                              1,
                              mine_path_step_rate_gp * frame_time * mine_vars->dir);
        FUN_L00_0025b8c0(
            &moby->pos.x, &mine_vars->vel[0], path_target_position[0], 0.02f, 0.2f, 0.0f);
        FUN_L00_0025b8c0(
            &moby->pos.y, &mine_vars->vel[1], path_target_position[1], 0.02f, 0.2f, 0.0f);
        FUN_L00_0025b8c0(
            &moby->pos.z, &mine_vars->vel[2], path_target_position[2], 0.02f, 0.2f, 0.0f);
        hit = find_moby_hit(moby, 0x230000, 0);
        fun_00213928(moby, hit, mine_vars->health, 0, &count, 0, 0, 4);
        if ((count >= 2 && hit != 0 && hit->moby->oclass != 0x283) ||
            hero.coll_hit_moby == (u8 *)moby) {
            clear_u64_value(&vel);
            allocate_voice_for_target_entry(0, 0, moby);
            if (hero.coll_hit_moby == (u8 *)moby)
                mine_explode(moby,
                             &vel,
                             &moby->pos,
                             3.0f,
                             1.0f,
                             10,
                             20,
                             60,
                             3.0f,
                             5.0f,
                             9.0f,
                             1.0f,
                             -1,
                             15.0f,
                             1,
                             1,
                             -1,
                             0);
            else
                mine_explode(moby,
                             &vel,
                             &moby->pos,
                             0.0f,
                             0.0f,
                             10,
                             20,
                             60,
                             3.0f,
                             5.0f,
                             9.0f,
                             1.0f,
                             -1,
                             15.0f,
                             1,
                             1,
                             -1,
                             0);
            mark_moby_for_removal(moby);
            return;
        }
        moby->rot.x = fast_add_rotations(moby->rot.x, mine_vars->spin[0]);
        moby->rot.y = fast_add_rotations(moby->rot.y, mine_vars->spin[1]);
        moby->rot.z = fast_add_rotations(moby->rot.z, mine_vars->spin[2]);
        mine_vars->phase = fast_add_rotations(mine_vars->phase, mine_glow_pulse_phase_step_gp);
        glow_pulse_sine = fast_sin(mine_vars->phase);
        glow_red_intensity = mine_ftoi(glow_pulse_sine * 48.0f) + 0xCF;
        glow_green_blue_intensity = mine_ftoi(glow_pulse_sine * 32.0f) + 0x30;
        moby->glow_color = glow_green_blue_intensity << 16 |
                           (0xFF000000 | glow_green_blue_intensity << 8) | glow_red_intensity;
        break;
    }
}

extern float fast_sin(float);
