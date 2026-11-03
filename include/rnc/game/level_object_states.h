#ifndef LOMBYTE_RNC_GAME_LEVEL_OBJECT_STATES_H
#define LOMBYTE_RNC_GAME_LEVEL_OBJECT_STATES_H

#include "types.h"

/*
 * One record per level object of the save state, indexed by the global
 * object index (level_object_index_boundaries splits it by level).
 * copy_level_object_positions writes the position; the mission checks,
 * map icons and overlay triggers read and set the state word.
 */
struct LevelObjectState {
    f32 x;     /* 0x0 */
    f32 y;     /* 0x4 */
    f32 z;     /* 0x8: map icon angle */
    s32 state; /* 0xC: 0 untouched, set to 1 once triggered; mission checks also test >= 2 */
};

/* mc_gamedata saves the whole table: 0x790 bytes, 121 records. */
#define LEVEL_OBJECT_STATE_COUNT 121
extern struct LevelObjectState level_object_states[LEVEL_OBJECT_STATE_COUNT] __asm__("D_0013D5B0");

#endif /* LOMBYTE_RNC_GAME_LEVEL_OBJECT_STATES_H */
