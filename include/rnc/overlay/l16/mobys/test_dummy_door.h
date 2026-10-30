#ifndef LOMBYTE_RNC_OVERLAY_L16_MOBYS_TEST_DUMMY_DOOR_H
#define LOMBYTE_RNC_OVERLAY_L16_MOBYS_TEST_DUMMY_DOOR_H

#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"

/* Declarations test_dummy_door.c shares with other files of its level. */
void *find_nearest_test_dummy_door(int index, void *position) __asm__("FUN_L16_002d5db0");

void refresh_test_dummy_door_timer(struct Moby *moby) __asm__("FUN_L16_002d5e80");

#endif /* LOMBYTE_RNC_OVERLAY_L16_MOBYS_TEST_DUMMY_DOOR_H */
