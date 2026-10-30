#ifndef LOMBYTE_RNC_OVERLAY_L16_MOBYS_TEST_DUMMY_H
#define LOMBYTE_RNC_OVERLAY_L16_MOBYS_TEST_DUMMY_H

#include "types.h"

/* Declarations test_dummy.c shares with other files of its level. */
// For each listed moby of class 0x21D: sets its state bytes and clears bit 0 of its flags.
void activate_test_dummy_group(int moby_group_index) __asm__("FUN_L16_002ceca8");

#endif /* LOMBYTE_RNC_OVERLAY_L16_MOBYS_TEST_DUMMY_H */
