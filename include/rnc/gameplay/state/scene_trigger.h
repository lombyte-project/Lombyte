#ifndef LOMBYTE_RNC_GAMEPLAY_STATE_SCENE_TRIGGER_H
#define LOMBYTE_RNC_GAMEPLAY_STATE_SCENE_TRIGGER_H

#include "types.h"

struct Moby;
struct SceneTrigger;

struct ActiveSceneTrigger {
    u8 pad_00[8];
    struct Moby *moby;             /* 0x08 */
    struct SceneTrigger *trigger;  /* 0x0C */
    s32 next_frame;                /* 0x10: no trigger fires before this game_frame */
};

extern struct ActiveSceneTrigger active_scene_trigger __asm__("D_L00_00179100");

#endif
