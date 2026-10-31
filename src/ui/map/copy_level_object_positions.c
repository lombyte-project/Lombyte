#include "types.h"
#include "rnc/game/level_object_states.h"
#include "rnc/ui/menus/menu_globals.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/ui/map/copy_level_object_positions.h"

void copy_level_object_positions(void) __asm__("FUN_00208770");

void copy_level_object_positions(void) {
    s32                      index;
    s32                      next_index;
    s32                      start;
    s32                      end;
    struct LevelObjectState *destination;
    u8                      *source_base;
    struct Moby            **source;
    struct Moby             *source_object;

    index = current_level_index;
    if ((u32)index >= 0x13U) {
        return;
    }
    next_index = index + 1;
    end = level_object_index_boundaries[next_index];
    start = level_object_index_boundaries[index];
    if (start >= end) {
        return;
    }
    source_base = ((u8 *)menu_action_messages);
    destination = &level_object_states[start];
    source = (struct Moby **)(source_base + start * 4);
    start = end - start;
    do {
        source_object = *source;
        if (source_object != 0) {
            destination->x = source_object->pos.x;
            destination->y = source_object->pos.y;
            destination->z = source_object->rot.z;
        }
        destination++;
        start--;
        source++;
    } while (start != 0);
}

u32 level_object_index_boundaries[20] = {0,    2,    0xf,  0x13, 0x22, 0x25, 0x2e,
                                         0x32, 0x36, 0x3a, 0x3c, 0x41, 0x45, 0x4a,
                                         0x56, 0x5d, 0x64, 0x6b, 0x72, 0x79};
