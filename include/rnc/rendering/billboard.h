#ifndef LOMBYTE_RNC_RENDERING_BILLBOARD_H
#define LOMBYTE_RNC_RENDERING_BILLBOARD_H

#include "types.h"

/* One glow billboard; append_billboard_batch draws the active ones. */
typedef struct BillboardRecord {
    f32 position[4]; /* x, y, z, w */
    s16 active_count;
    s16 alpha;
    u8 pad14[4];
    f32 angle;
    f32 radius_scale;
} BillboardRecord; /* size 0x20 */

extern BillboardRecord billboard_records[16] __asm__("D_0018ED00");

#endif /* LOMBYTE_RNC_RENDERING_BILLBOARD_H */
