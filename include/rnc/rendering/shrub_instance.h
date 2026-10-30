#ifndef LOMBYTE_RNC_RENDERING_SHRUB_INSTANCE_H
#define LOMBYTE_RNC_RENDERING_SHRUB_INSTANCE_H

#include "types.h"

/* One placed shrub in the level, 0x20 bytes. */
typedef struct ShrubInstance {
    f32 position[3];
    f32 scale;  /* 0x0C */
    f32 radius; /* 0x10 */
    u8  pad14[3];
    u8  x17;    /* 0x17 */
    s16 index;  /* 0x18 */
    u8  cls;    /* 0x1A */
    u8  x1B;    /* 0x1B */
    u16 x1C;    /* 0x1C */
    u16 lights; /* 0x1E: up to four point light nibbles, 0xFFFF when unlit */
} ShrubInstance;

extern s32            shrub_count __asm__("D_001603D0");
extern ShrubInstance *shrubs __asm__("D_001603D4");
extern ShrubInstance *shrubs_end __asm__("D_001603D8");

#endif /* LOMBYTE_RNC_RENDERING_SHRUB_INSTANCE_H */
