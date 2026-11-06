#ifndef LOMBYTE_RNC_OVERLAY_SHARED_HERO_JUMP_VELOCITY_KEYS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_HERO_JUMP_VELOCITY_KEYS_H

#include "rnc/hero/hero.h"

/* The vertical velocity curves hero_set_state gives two jumps; every level holds its own copy
   next to its hero_set_state. The last key of each holds until velocity_keys_end_frame. */

/* HERO_STATE_JUMP_WITH_TRANSFORM_TRAIL. */
#define HERO_TRANSFORM_TRAIL_JUMP_VELOCITY_KEYS_COUNT 5
#define HERO_TRANSFORM_TRAIL_JUMP_VELOCITY_KEYS                                        \
    {                                                                                  \
        {151.0f, 0.0f, 5}, {0.0f, 0.0f, 27}, {22.0f, 2.0f, 6}, {-999999.0f, -3.0f, 6}, \
            {0.0f, 2.0f, 99},                                                          \
    }

/* HERO_STATE_WATER_SURFACE_JUMP out of deep water. */
#define HERO_WATER_SURFACE_JUMP_VELOCITY_KEYS_COUNT 8
#define HERO_WATER_SURFACE_JUMP_VELOCITY_KEYS                                                      \
    {                                                                                              \
        {-27.0f, 0.0f, 9}, {40.0f, 0.0f, 1}, {55.0f, 0.0f, 1}, {75.0f, 0.0f, 1}, {90.5f, 0.0f, 9}, \
            {75.0f, 0.0f, 4}, {40.0f, 0.0f, 3}, {0.0f, 0.0f, 100},                                 \
    }

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_HERO_JUMP_VELOCITY_KEYS_H */
