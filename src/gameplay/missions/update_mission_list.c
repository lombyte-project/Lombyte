#include "types.h"
#include "rnc/ui/map/map_state.h"
struct MenuItem {
    s16 id;
    s16 kind1;
    s32 value1;
    s16 kind2;
    u8 padA[2];
    s32 value2;
    u16 flags;
    u8 pad12[0xA];
    s16 (*callback)(void *);
    void *arg;
    s16 state;
    s16 result;
};
struct MenuState {
    u8 pad0[0xC];
    s32 available;
    struct MenuItem *items;
};
extern struct MenuItem *D_001A2B70[];
extern struct MenuState D_001A2C10;
#include "rnc/gameplay/state/level_state.h"
extern s32 check_mission_condition(s16, s32) __asm__("func_0020BAF0");
s32 update_mission_list(void) __asm__("FUN_0020b950");

s32 update_mission_list(void) {
    struct MenuItem *it;
    struct MenuItem *items;

    items = D_001A2B70[level_map_selection.level];
    D_001A2C10.items = items;
    if (items == 0) {
        return 0;
    }
    for (it = items; it->id != 0; it++) {
        if ((it->flags & 4) && level_visit_state[level_map_selection.level] == 0) {
            it->state = 0;
        } else if (!check_mission_condition(it->kind1, it->value1)) {
            it->state = 0;
        } else if (!check_mission_condition(it->kind2, it->value2)) {
            it->state = 1;
        } else {
            it->state = 2;
        }
    }
    for (it = D_001A2C10.items; it->id != 0; it++) {
        if (it->callback != 0) {
            it->result = it->callback(it->arg);
        }
    }
    D_001A2C10.available = 0;
    for (it = D_001A2C10.items; it->id != 0; it++) {
        if (it->state == 1 && !(it->flags & 2)) {
            D_001A2C10.available++;
        }
    }
    return D_001A2C10.available == 0;
}

extern __typeof__(update_mission_list) func_0020B950 __attribute__((alias("FUN_0020b950")));
