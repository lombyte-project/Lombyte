#include "types.h"
#include "rnc/game/level_object_states.h"
#include "rnc/ui/menus/menu_globals.h"
#include "rnc/globals.h"
#include "rnc/ui/map/copy_level_object_positions.h"

struct ObjectVector {
    u8  pad_0[0x10];
    f32 x;
    f32 y;
    u8  pad_18[0x30];
    f32 z;
};

void copy_level_object_positions(void) __asm__("FUN_00208770");

void copy_level_object_positions(void) {
    s32                  index;
    s32                  next_index;
    s32                  start;
    s32                  end;
    u8                  *destination;
    u8                  *source_base;
    u8                 **source;
    struct ObjectVector *source_object;

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
    destination = (u8 *)level_object_states + start * 0x10;
    source = (u8 **)(source_base + start * 4);
    start = end - start;
    do {
        source_object = (struct ObjectVector *)*source;
        if (source_object != 0) {
            *(f32 *)(destination + 0x0) = source_object->x;
            *(f32 *)(destination + 0x4) = source_object->y;
            *(f32 *)(destination + 0x8) = source_object->z;
        }
        destination += 0x10;
        start--;
        source++;
    } while (start != 0);
}

u32 level_object_index_boundaries[20] = {0,    2,    0xf,  0x13, 0x22, 0x25, 0x2e,
                                         0x32, 0x36, 0x3a, 0x3c, 0x41, 0x45, 0x4a,
                                         0x56, 0x5d, 0x64, 0x6b, 0x72, 0x79};
