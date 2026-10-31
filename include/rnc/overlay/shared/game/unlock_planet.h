#ifndef LOMBYTE_RNC_OVERLAY_SHARED_GAME_UNLOCK_PLANET_H
#define LOMBYTE_RNC_OVERLAY_SHARED_GAME_UNLOCK_PLANET_H

#include "types.h"

void update_entity_matrix(void *, void *, void *, void *) __asm__("FUN_L00_00260738");
void grant_inventory_item(s32, s32) __asm__("FUN_L00_00260860");
s32  select_moby_combat_target(void *, void *, void *) __asm__("FUN_L00_0025fcb8");
void unlock_planet(s32) __asm__("FUN_L00_002607d0");

struct Moby;

int convert_pose_to_reference_local(int, char *, float *, float *, float *, float *) __asm__("FUN_L00_00260668");
int  FUN_L00_00260400(int, char *, void *, void *, float *, void *);
void spawn_randomized_bolt_drop(struct Moby *, int, int, int, int) __asm__("FUN_L00_00260a88");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_GAME_UNLOCK_PLANET_H */
