#include "types.h"
#include "rnc/game/level_object_states.h"
#include "rnc/ui/menus/menu_globals.h"
#include "rnc/globals.h"

struct ObjectVector {
    u8  pad_0[0x10];
    f32 x;
    f32 y;
    u8  pad_18[0x30];
    f32 z;
};

extern u32 level_object_index_boundaries[] __asm__("D_001E03B8");

void copy_level_object_positions(void) __asm__("FUN_00208770");

void copy_level_object_positions(void) {
    s32                  index;
    s32                  next_index;
    s32                  start;
    s32                  end;
    u8                  *destination;
    u8                  *source_base;
    u8                 **source;
    struct ObjectVector *object;

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
        object = (struct ObjectVector *)*source;
        if (object != 0) {
            *(f32 *)(destination + 0x0) = object->x;
            *(f32 *)(destination + 0x4) = object->y;
            *(f32 *)(destination + 0x8) = object->z;
        }
        destination += 0x10;
        start--;
        source++;
    } while (start != 0);
}
