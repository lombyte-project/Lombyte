#include "types.h"
#include "rnc/ui/map/map_state.h"
#include "rnc/game/missions/mission_item.h"

struct MenuState {
    u8                  pad0[0xC];
    s32                 available;
    struct MissionItem *items;
};

extern struct MenuState D_001A2C10;

#include "rnc/game/level_state.h"

extern s32 check_mission_condition(s16, s32) __asm__("func_0020BAF0");
s32        update_mission_list(void) __asm__("FUN_0020b950");

s32 update_mission_list(void) {
    enum {
        MISSION_STATE_UNAVAILABLE = 0,
        MISSION_STATE_AVAILABLE = 1,
        MISSION_STATE_BOTH_CONDITIONS_MET = 2
    };
    struct MissionItem *it;
    struct MissionItem *items;

    items = mission_lists_by_level[level_map_selection.level];
    D_001A2C10.items = items;
    if (items == 0) {
        return 0;
    }
    for (it = items; it->id != 0; it++) {
        if ((it->flags & 4) && level_visit_state[level_map_selection.level] == 0) {
            it->state = MISSION_STATE_UNAVAILABLE;
        } else if (!check_mission_condition(it->kind1, it->value1)) {
            it->state = MISSION_STATE_UNAVAILABLE;
        } else if (!check_mission_condition(it->kind2, it->value2)) {
            it->state = MISSION_STATE_AVAILABLE;
        } else {
            it->state = MISSION_STATE_BOTH_CONDITIONS_MET;
        }
    }
    for (it = D_001A2C10.items; it->id != 0; it++) {
        if (it->callback != 0) {
            it->result = it->callback(it->arg);
        }
    }
    D_001A2C10.available = 0;
    for (it = D_001A2C10.items; it->id != 0; it++) {
        if (it->state == MISSION_STATE_AVAILABLE && !(it->flags & 2)) {
            D_001A2C10.available++;
        }
    }
    return D_001A2C10.available == 0;
}

extern __typeof__(update_mission_list) func_0020B950 __attribute__((alias("FUN_0020b950")));
