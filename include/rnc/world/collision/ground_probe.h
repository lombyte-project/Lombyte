#ifndef RCP2P_RNC_WORLD_COLLISION_GROUND_PROBE_H
#define RCP2P_RNC_WORLD_COLLISION_GROUND_PROBE_H

#include "types.h"

/* Height of the ground below a position. */
f32 probe_ground_height(void *pos, s32 flags, f32 height_offset) __asm__("FUN_00213508");

#endif /* RCP2P_RNC_WORLD_COLLISION_GROUND_PROBE_H */
