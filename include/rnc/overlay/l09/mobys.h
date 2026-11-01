#ifndef LOMBYTE_RNC_OVERLAY_L09_MOBYS_H
#define LOMBYTE_RNC_OVERLAY_L09_MOBYS_H

#include "rnc/math/vector.h"
#include "types.h"

struct Moby;

void airship_chain_link_update() __asm__("FUN_L09_00304360");
void airship_part_update(struct Moby *) __asm__("FUN_L09_002c26c8");
void asteroid_update(struct Moby *) __asm__("FUN_L09_002eb970");
void barrier_update() __asm__("FUN_L09_003082d8");
void damage_causer_update(struct Moby *) __asm__("FUN_L09_00305a28");
void destructible_prop_update(struct Moby *) __asm__("FUN_L09_002ee110");
void display_update(struct Moby *) __asm__("FUN_L09_002edc30");
struct Moby      *
spawn_tumbling_debris(struct Moby *, Vec4 *, Vec4 *, s32, f32) __asm__("FUN_L09_002ef868");
void drifting_moby_update(struct Moby *) __asm__("FUN_L09_002f0040");
void elevator_update(unsigned char *) __asm__("FUN_L09_003033a0");
void floating_ship_piece_update(char *) __asm__("FUN_L09_003091b0");
void help_hints_update(struct Moby *) __asm__("FUN_L09_00300888");
void lamp_update(struct Moby *) __asm__("FUN_L09_0030a678");
void platform_update() __asm__("FUN_L09_002f86a0");
void refueling_station_update(struct Moby *) __asm__("FUN_L09_00303d10");
void shrinking_tumbling_debris_update(struct Moby *) __asm__("FUN_L09_002ef9b0");
void shuttle_update(struct Moby *) __asm__("FUN_L09_00309c08");
void tumbling_debris_update(struct Moby *) __asm__("FUN_L09_002ef430");
void turret_update(struct Moby *) __asm__("FUN_L09_00304c80");

#endif /* LOMBYTE_RNC_OVERLAY_L09_MOBYS_H */
