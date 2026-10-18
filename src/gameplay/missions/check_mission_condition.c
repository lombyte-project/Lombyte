#include "types.h"
#include "rnc/data_labels.h"
struct Upgrade {
    u8 pad0[0xC];
    s32 level;
};
#include "rnc/gameplay/state/level_state.h"
extern struct Upgrade D_0013D5B0[];
#include "rnc/gameplay/state/item_state.h"
s32 check_mission_condition(s16 kind, s32 value) __asm__("FUN_0020baf0");

s32 check_mission_condition(s16 kind, s32 value) {
    enum {
        MISSION_CONDITION_ALWAYS_TRUE = 0,
        MISSION_CONDITION_LEVEL_AVAILABLE = 1,
        MISSION_CONDITION_ITEM_AVAILABLE = 2,
        MISSION_CONDITION_ITEM_ACQUIRED = 3,
        MISSION_CONDITION_UPGRADE_LEVEL_NONZERO = 4,
        MISSION_CONDITION_UPGRADE_LEVEL_AT_LEAST_TWO = 5,
        MISSION_CONDITION_ALTERNATE_ITEM_AVAILABLE = 6,
        MISSION_CONDITION_CALLBACK_NONZERO = 7,
        MISSION_CONDITION_ALWAYS_FALSE = 9
    };
    switch (kind) {
    case MISSION_CONDITION_ALWAYS_TRUE:
        return 1;
    case MISSION_CONDITION_LEVEL_AVAILABLE:
        return level_available[value] != 0;
    case MISSION_CONDITION_ITEM_AVAILABLE:
        return item_available[value] != 0;
    case MISSION_CONDITION_ITEM_ACQUIRED:
        return items_acquired[value] != 0;
    case MISSION_CONDITION_UPGRADE_LEVEL_NONZERO:
        if (value < 121) {
            return D_0013D5B0[value].level != 0;
        }
        break;
    case MISSION_CONDITION_UPGRADE_LEVEL_AT_LEAST_TWO:
        if (value < 121) {
            return D_0013D5B0[value].level >= 2;
        }
        break;
    case MISSION_CONDITION_ALTERNATE_ITEM_AVAILABLE:
        return alternate_item_available[value] != 0;
    case MISSION_CONDITION_CALLBACK_NONZERO:
        return ((s32(*)(void))value)() != 0;
    case 8:
        return D_0014BEC0[(value & 0xFFFF) + (value >> 16) * 4] != 0;
    case MISSION_CONDITION_ALWAYS_FALSE:
        return 0;
    }
    return 0;
}

extern __typeof__(check_mission_condition) func_0020BAF0 __attribute__((alias("FUN_0020baf0")));
