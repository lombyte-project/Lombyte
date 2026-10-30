#ifndef LOMBYTE_RNC_OVERLAY_SHARED_VEHICLES_VELDIN_SHIP_H
#define LOMBYTE_RNC_OVERLAY_SHARED_VEHICLES_VELDIN_SHIP_H

#include "types.h"

struct Moby;

void  decoy_glove_update(u8 *) __asm__("FUN_L00_002d2670");
void  grow_and_fade_out_update(u8 *) __asm__("FUN_L00_002d7f38");
void  helipack_update(s8 *) __asm__("FUN_L00_002d37d0");
void  metal_detector_update(struct Moby *) __asm__("FUN_L00_002d2ee8");
void  update_animated_point_light_moby(u8 *) __asm__("FUN_L00_002d3a10");
void  infobot_update(struct Moby *) __asm__("FUN_L00_002d5de8");
void *mine_camera_blast(void *, void *, s32, void *) __asm__("FUN_L00_002d3838");
void *spawn_burst_moby(void *, f32) __asm__("FUN_L00_002d7e90");
void  initialize_moby_particle_effect(void *) __asm__("FUN_L00_002d6bf0");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_VEHICLES_VELDIN_SHIP_H */
