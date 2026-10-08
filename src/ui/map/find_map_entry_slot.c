#include "types.h"
#include "rnc/ui/map/map_state.h"

s32 find_map_entry_slot(s32 arg0) __asm__("FUN_002050a0");

/* Returns the cache slot holding map id arg0, or -1. */
s32 find_map_entry_slot(s32 arg0) {
    register struct MapState *map;
    register s32 *id;
    register s32 count = 0;

    map = &D_001A00F0;
    id = map->slot_id;
    do {
        if (id[-5] != 0 && *id == arg0) { /* slot[count], slot_id[count] */
            return count;
        }
        count += 1;
        id++;
    } while (count < 5);
    return -1;
}
