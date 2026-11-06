#ifndef LOMBYTE_RNC_GAME_MISSIONS_MISSION_ITEM_H
#define LOMBYTE_RNC_GAME_MISSIONS_MISSION_ITEM_H

#include "types.h"

/* Kinds of check_mission_condition: what its operand names. */
enum MissionCondition {
    MISSION_CONDITION_ALWAYS_TRUE = 0,
    MISSION_CONDITION_LEVEL_AVAILABLE = 1,
    MISSION_CONDITION_ITEM_AVAILABLE = 2,
    MISSION_CONDITION_ITEM_ACQUIRED = 3,
    MISSION_CONDITION_UPGRADE_LEVEL_NONZERO = 4,
    MISSION_CONDITION_UPGRADE_LEVEL_AT_LEAST_TWO = 5,
    MISSION_CONDITION_ALTERNATE_ITEM_AVAILABLE = 6,
    MISSION_CONDITION_CALLBACK_NONZERO = 7, /* operand: an int (*)(void) */
    MISSION_CONDITION_GOLD_BOLT_COLLECTED = 8,
    MISSION_CONDITION_ALWAYS_FALSE = 9
};

/* MissionItem.flags */
#define MISSION_FLAG_HIDE_WHEN_DONE 0x1 /* collect_mission_ids skips it once done */
#define MISSION_FLAG_UNCOUNTED      0x2 /* never listed or counted as open */
#define MISSION_FLAG_NEEDS_VISIT    0x4 /* unavailable until the level is visited */

/* One mission of a level's list (mission_lists_by_level[level]); the list
   ends at the first entry with id 0. update_mission_list fills state and
   result, collect_mission_ids lists the texts. */
struct MissionItem {
    s16 id;     /* 0x00: help text id of the mission */
    s16 kind1;  /* 0x02: enum MissionCondition: the mission is open */
    s32 value1; /* 0x04 */
    s16 kind2;  /* 0x08: enum MissionCondition: the mission is done */
    u8  padA[2];
    s32 value2;              /* 0x0C */
    u16 flags;               /* 0x10: MISSION_FLAG_* */
    s16 base;                /* 0x12: collect_mission_ids reports base + result */
    s16 text_ids[4];         /* 0x14: help text by result (collect_mission_ids) */
    int (*callback)(); /* 0x1C: picks result (an int (*)(void) or int (*)(int)), may be null */
    void *arg;               /* 0x20 */
    s16   state;             /* 0x24: 0 closed, 1 open, 2 done */
    s16   result;            /* 0x26 */
}; /* size 0x28 */

/* The mission list of each level, read by update_mission_list. */
extern struct MissionItem *mission_lists_by_level[20] __asm__("D_001A2B70");

#endif /* LOMBYTE_RNC_GAME_MISSIONS_MISSION_ITEM_H */
