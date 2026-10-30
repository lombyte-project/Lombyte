#ifndef LOMBYTE_RNC_RENDERING_EFFECTS_DRAW_LENS_FLARE_H
#define LOMBYTE_RNC_RENDERING_EFFECTS_DRAW_LENS_FLARE_H

#include "types.h"

typedef struct {
    u8  pad_0[0x10];
    f32 pos[4];
    u8  flags;
} FlareMoby;

/* The lens flare draw_lens_flare draws: 16 sprites, each with its own
   alpha, texture, colour, distance along the flare line and scale. */
typedef struct {
    FlareMoby *moby;
    s32        state;
    f32        intensity;
    s32        pad_c;
    s16        alpha[16];
    s16        tex[16];
    s32        color[16];
    f32        dist[16];
    f32        scale[16];
} LensFlare;

extern LensFlare effect_ribbon __asm__("D_00187300");

#endif /* LOMBYTE_RNC_RENDERING_EFFECTS_DRAW_LENS_FLARE_H */
