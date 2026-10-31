#ifndef LOMBYTE_RNC_OVERLAY_SHARED_PROPS_ROCK_H
#define LOMBYTE_RNC_OVERLAY_SHARED_PROPS_ROCK_H

#include "types.h"

struct Moby;

void gold_bolt_update(struct Moby *) __asm__("FUN_L01_00307ca0");
void spawn_point_update(struct Moby *) __asm__("FUN_L01_00300220");
void visibomb_zone_timer_update(struct Moby *) __asm__("FUN_L01_00302648");
void teleporter_update(struct Moby *) __asm__("FUN_L01_00308bd8");
void *
spawn_debris_chunk(void *, void *, u32, f32, s32, f32, f32, f32, s32) __asm__("FUN_L01_002f8530");
void cutscene_trigger_update(s8 *) __asm__("FUN_L01_002fb5b0");
void rail_hero_motion_update(struct Moby *) __asm__("FUN_L01_002f6328");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_PROPS_ROCK_H */
