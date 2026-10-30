#ifndef LOMBYTE_RNC_OVERLAY_SHARED_PROPS_ROCK_H
#define LOMBYTE_RNC_OVERLAY_SHARED_PROPS_ROCK_H

#include "types.h"

struct Moby;

void gold_bolt_update(struct Moby *) __asm__("FUN_L01_00307ca0");
void spawn_point_update(struct Moby *) __asm__("FUN_L01_00300220");
void update_target_outside_zone_timeout(struct Moby *) __asm__("FUN_L01_00302648");
void teleporter_update(struct Moby *) __asm__("FUN_L01_00308bd8");
void *
spawn_debris_chunk(void *, void *, u32, f32, s32, f32, f32, f32, s32) __asm__("FUN_L01_002f8530");
void update_cutscene_trigger(s8 *) __asm__("FUN_L01_002fb5b0");
void update_rail_hero_motion(struct Moby *) __asm__("FUN_L01_002f6328");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_PROPS_ROCK_H */
