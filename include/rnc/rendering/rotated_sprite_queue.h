#ifndef LOMBYTE_RNC_RENDERING_ROTATED_SPRITE_QUEUE_H
#define LOMBYTE_RNC_RENDERING_ROTATED_SPRITE_QUEUE_H

#include "types.h"

/* The queue of rotated sprites to draw this frame: 0x30-byte records, the count
   at +0xC0. */
typedef struct {
    f32 position[4];
    f32 scale;
    u32 color;
    s32 texture_definition;
    f32 angle;
    u8 *resource;         /* 0x20 */
    s16 project_position; /* 0x24 */
    s16 repetition_count;
    f32 angle_step;
    s32 mode;
} QueuedRotatedSprite;

typedef struct {
    QueuedRotatedSprite records[4];
    s32 count; /* 0xC0 */
} RotatedSpriteQueue;

extern RotatedSpriteQueue rotated_sprite_queue __asm__("D_00189300");

#endif /* LOMBYTE_RNC_RENDERING_ROTATED_SPRITE_QUEUE_H */
