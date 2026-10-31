#ifndef LOMBYTE_RNC_RENDERING_EFFECTS_RENDER_QUEUED_ROTATED_SPRITES_H
#define LOMBYTE_RNC_RENDERING_EFFECTS_RENDER_QUEUED_ROTATED_SPRITES_H

#include "types.h"

/* One queued rotated sprite (0x30 bytes): render_queued_rotated_sprites
   projects position and hands the record to spawn_particle_burst, which
   draws repetition_count copies (mode 0), a mirrored quad (mode 1) or one
   centred sprite (mode 2) of texture_definition, scale * 40 wide. */
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

void render_queued_rotated_sprites(void) __asm__("FUN_001ee338");

#endif /* LOMBYTE_RNC_RENDERING_EFFECTS_RENDER_QUEUED_ROTATED_SPRITES_H */
