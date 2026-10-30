#ifndef LOMBYTE_RNC_OVERLAY_SHARED_MOBYS_SPAWN_TYPE_52_PARTICLE_NEAR_MOBY_H
#define LOMBYTE_RNC_OVERLAY_SHARED_MOBYS_SPAWN_TYPE_52_PARTICLE_NEAR_MOBY_H

#include "types.h"

struct UpdateCam;

void activate_descriptor(s32) __asm__("FUN_L10_002f5a50");
s32  evaluate_descriptor_eligibility(s8 *, s8 *) __asm__("FUN_L10_002f65f8");
void initialize_path_camera(struct UpdateCam *) __asm__("FUN_L10_002f5b58");
void spawn_type_52_particle_near_moby(void *) __asm__("FUN_L10_002e3de8");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_MOBYS_SPAWN_TYPE_52_PARTICLE_NEAR_MOBY_H */
