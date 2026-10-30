#ifndef LOMBYTE_RNC_OVERLAY_L01_MOBYS_BIRDBOT_PROJECTILE_H
#define LOMBYTE_RNC_OVERLAY_L01_MOBYS_BIRDBOT_PROJECTILE_H

#include "types.h"

/* Spawns a birdbot projectile (class 0x2D2) at pos with starting velocity vel
   (both quadwords); birdbot_update fires it. Returns the moby or NULL. */
void *spawn_birdbot_projectile(
    void *owner, void *pos, void *vel, f32 yaw, f32 pitch, s32 color) __asm__("FUN_L01_002fa068");

#endif /* LOMBYTE_RNC_OVERLAY_L01_MOBYS_BIRDBOT_PROJECTILE_H */
