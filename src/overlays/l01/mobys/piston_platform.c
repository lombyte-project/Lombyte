/* Level 01 moby class piston_platform. */
#include "types.h"

#include "rnc/math/asm_helpers.h"
#include "rnc/audio/voice_alloc.h"

extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern s32  scale_game_frames(s32) __asm__("FUN_001f96f8");

#include "eetypes.h"
#include "qcopy.h"

typedef struct {
    u8  pad0[0x20];
    s32 x20;
    s16 x24;
    u8  pad26[2];
    u8  x28;
    u8  pad29[0x15];
    s16 x3e;
    u8  pad40[0x20];
    u8  x60[0x40];
    f32 home_z;
    u8  home_mode;
    u8  padA5;
    s16 timer;
    f32 travel;
    f32 speed;
    f32 period;
    s32 xb4;
} LiftVars;

typedef struct {
    u8        pad0[0x10];
    u128      pos;
    u8        state;
    u8        pad21[0xF];
    u8        b30;
    u8        pad31;
    s16       h32;
    u8        pad34[0xC];
    u8        x40[0x38];
    LiftVars *pvars;
    u8        pad7c[0x40];
    u8        mode;
} LiftMoby;

extern void update_entity_matrix(void *, void *, void *, void *) __asm__("FUN_L00_00260738");

void piston_platform_update(LiftMoby *m) __asm__("FUN_L01_002f95c0");

void piston_platform_update(LiftMoby *piston_platform) {
    enum {
        PISTON_PLATFORM_MODE_WAIT_BEFORE_SUBTRACT_TRAVEL = 0,
        PISTON_PLATFORM_MODE_WAIT_BEFORE_ADD_TRAVEL = 1,
        PISTON_PLATFORM_MODE_MOVING = 2
    };
    LiftVars *piston_platform_vars;
    u128      old;
    u128      delta;
    u128     *dp;

    piston_platform_vars = piston_platform->pvars;
    qcopy(&old, &piston_platform->pos);
    fast_dec_timer_s16(&piston_platform_vars->timer);
    if (piston_platform->state == 0) {
        piston_platform_vars->x28 = 4;
        piston_platform_vars->x3e = 0xD;
        piston_platform_vars->x20 = 0;
        piston_platform_vars->x24 = 0;
        piston_platform_vars->home_z = ((f32 *)&piston_platform->pos)[2];
        piston_platform->mode = piston_platform_vars->home_mode;
        piston_platform_vars->timer = scale_game_frames(0x78);
        piston_platform->state = 1;
        piston_platform->b30 = 0xFF;
        piston_platform->h32 = 0xFF;
        piston_platform_vars->xb4 = -1;
    }
    switch (piston_platform->mode) {
    case PISTON_PLATFORM_MODE_WAIT_BEFORE_SUBTRACT_TRAVEL:
        if (piston_platform_vars->timer == 0) {
            piston_platform->mode = 2;
            piston_platform_vars->speed =
                piston_platform_vars->travel /
                convert_integer_to_float(
                    scale_game_frames(truncate_float_to_s32(piston_platform_vars->period * 60.0f)));
            piston_platform_vars->timer =
                scale_game_frames(truncate_float_to_s32(piston_platform_vars->period * 60.0f));
            allocate_voice_for_target_entry(0, 0, piston_platform);
        }
        break;
    case PISTON_PLATFORM_MODE_WAIT_BEFORE_ADD_TRAVEL:
        if (piston_platform_vars->timer == 0) {
            piston_platform->mode = 2;
            piston_platform_vars->speed =
                -(piston_platform_vars->travel /
                  convert_integer_to_float(scale_game_frames(
                      truncate_float_to_s32(piston_platform_vars->period * 60.0f))));
            piston_platform_vars->timer =
                scale_game_frames(truncate_float_to_s32(piston_platform_vars->period * 60.0f));
            allocate_voice_for_target_entry(1, 0, piston_platform);
        }
        break;
    case PISTON_PLATFORM_MODE_MOVING:
        ((f32 *)&piston_platform->pos)[2] -= piston_platform_vars->speed;
        if (piston_platform_vars->timer == 0) {
            if (piston_platform_vars->speed < 0.0f) {
                piston_platform->mode = 0;
            } else {
                piston_platform->mode = 1;
            }
            if (piston_platform->mode == piston_platform_vars->home_mode) {
                ((f32 *)&piston_platform->pos)[2] = piston_platform_vars->home_z;
            }
            piston_platform_vars->timer = scale_game_frames(0x78);
        }
        break;
    }
    dp = &delta;
    fast_vec_sub(dp, &piston_platform->pos, &old);
    update_entity_matrix(piston_platform_vars->x60, dp, piston_platform->x40, piston_platform->x40);
}
