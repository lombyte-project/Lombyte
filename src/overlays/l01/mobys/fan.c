/* Level 01 moby class fan. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"

typedef struct {
    f32 sway;
    f32 dir;
    s32 timer;
} SwayVars;

typedef struct {
    u8        pad0[0x10];
    u8        pos[0x10];
    u8        state;
    u8        pad21[0x27];
    f32       rotz;
    u8        pad4c[0x2C];
    SwayVars *pvars;
} SwayMoby;

typedef struct {
    s64   t;
    u8    pad8[8];
    void *moby;
    s32   flags;
    s32   x18;
    s32   x1c;
    s32   x20;
} CollQuery;

extern void copy_hero_moby_field_38() __asm__("FUN_L00_0025d1b8");
extern s32  rand(void) __asm__("FUN_001160d8");
extern void collect_mobies_in_sphere(void *, s32, void *, void *, f32) __asm__("FUN_L00_001f2868");

void fan_update(SwayMoby *m) __asm__("FUN_L01_002f9bf0");

void fan_update(SwayMoby *fan_moby) {
    SwayVars *sway_vars;
    CollQuery collision_query;

    sway_vars = fan_moby->pvars;
    if (fan_moby->state == 0) {
        copy_hero_moby_field_38(fan_moby);
        fan_moby->state = 1;
        sway_vars->sway = random_float_between(-1.0f, 1.0f);
        sway_vars->dir = (rand() & 1) ? 1.0f : -1.0f;
    }
    collision_query.moby = fan_moby;
    collision_query.flags = 0x10001;
    collision_query.x1c = 0;
    clear_u64_value(&collision_query);
    collision_query.x20 = 0;
    collect_mobies_in_sphere(fan_moby->pos, 0x10, fan_moby, &collision_query, 1.2f);
    if (sway_vars->dir == sway_vars->sway) {
        if (fast_dec_timer(&sway_vars->timer)) {
            sway_vars->dir = -sway_vars->dir;
            sway_vars->timer = scale_game_frames(0x5A);
        }
    }
    sway_vars->sway += frame_time * 4.0f * sway_vars->dir;
    if (sway_vars->sway > 1.0f) {
        sway_vars->sway = 1.0f;
    }
    if (sway_vars->sway < -1.0f) {
        sway_vars->sway = -1.0f;
    }
    fan_moby->rotz = fast_add_rotations(fan_moby->rotz, frame_time * -12.566371f * sway_vars->sway);
}

extern float random_float_between(float, float);
extern int   scale_game_frames(int);

#include "qcopy.h"
