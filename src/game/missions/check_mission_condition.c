#include "types.h"
#include "rnc/game/level_object_states.h"
#include "rnc/data_labels.h"
#include "rnc/game/level_state.h"
#include "rnc/game/item_state.h"
#include "rnc/game/missions/mission_item.h"

s32 check_mission_condition(s16 kind, s32 value) __asm__("FUN_0020baf0");

s32 check_mission_condition(s16 kind, s32 condition_operand) {
    switch (kind) {
    case MISSION_CONDITION_ALWAYS_TRUE:
        return 1;
    case MISSION_CONDITION_LEVEL_AVAILABLE:
        return level_available[condition_operand] != 0;
    case MISSION_CONDITION_ITEM_AVAILABLE:
        return item_available[condition_operand] != 0;
    case MISSION_CONDITION_ITEM_ACQUIRED:
        return items_acquired[condition_operand] != 0;
    case MISSION_CONDITION_UPGRADE_LEVEL_NONZERO:
        if (condition_operand < 121) {
            return level_object_states[condition_operand].state != 0;
        }
        break;
    case MISSION_CONDITION_UPGRADE_LEVEL_AT_LEAST_TWO:
        if (condition_operand < 121) {
            return level_object_states[condition_operand].state >= 2;
        }
        break;
    case MISSION_CONDITION_ALTERNATE_ITEM_AVAILABLE:
        return alternate_item_available[condition_operand] != 0;
    case MISSION_CONDITION_CALLBACK_NONZERO:
        return ((s32(*)(void))condition_operand)() != 0;
    case MISSION_CONDITION_GOLD_BOLT_COLLECTED:
        return gold_bolts_collected[(condition_operand & 0xFFFF) + (condition_operand >> 16) * 4] !=
               0;
    case MISSION_CONDITION_ALWAYS_FALSE:
        return 0;
    }
    return 0;
}

extern __typeof__(check_mission_condition) func_0020BAF0 __attribute__((alias("FUN_0020baf0")));
