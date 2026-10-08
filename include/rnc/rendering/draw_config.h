#ifndef LOMBYTE_RNC_RENDERING_DRAW_CONFIG_H
#define LOMBYTE_RNC_RENDERING_DRAW_CONFIG_H

#include "types.h"

/* Texture upload count and on/off switch of one world renderer. */
struct DrawPass {
    s32 count;
    s32 enabled;
};

/*
 * Per-renderer switches at D_0018A2B0, read by the draw loops, the texture
 * uploads and the debug profiler. Size 0x4C as far as src/ reads it.
 */
struct DrawConfig {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    struct DrawPass tfrag; /* 0xC */
    struct DrawPass tie;   /* 0x14 */
    struct DrawPass shrub; /* 0x1C */
    struct DrawPass moby;  /* 0x24 */
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
};

extern struct DrawConfig draw_config __asm__("D_0018A2B0");

#endif /* LOMBYTE_RNC_RENDERING_DRAW_CONFIG_H */
