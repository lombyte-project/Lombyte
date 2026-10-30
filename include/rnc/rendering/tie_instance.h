#ifndef LOMBYTE_RNC_RENDERING_TIE_INSTANCE_H
#define LOMBYTE_RNC_RENDERING_TIE_INSTANCE_H

#include "types.h"

struct TieData;

/* One placed tie in the level, 0x20 bytes. */
typedef struct TieInstance {
    f32             position[3];
    f32             scale;  /* 0x0C */
    struct TieData *data;   /* 0x10 */
    f32             x14;    /* 0x14 */
    s16             x18;    /* 0x18 */
    u8              cls;    /* 0x1A */
    u8              x1B;    /* 0x1B */
    u16             x1C;    /* 0x1C */
    u16             lights; /* 0x1E: as ShrubInstance.lights */
} TieInstance;

extern TieInstance *ties __asm__("D_00160F50");
extern TieInstance *ties_end __asm__("D_00160F54");

#endif /* LOMBYTE_RNC_RENDERING_TIE_INSTANCE_H */
