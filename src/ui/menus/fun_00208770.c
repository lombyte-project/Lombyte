#include "types.h"

struct ObjectVector {
    u8 pad_0[0x10];
    f32 x;
    f32 y;
    u8 pad_18[0x30];
    f32 z;
};

extern u8 D_0013D5B0[];
extern s32 D_0015ED84;
extern u8 D_00199478[];
extern u32 D_001E03B8[];

void FUN_00208770(void) {
    s32 index;
    s32 next_index;
    s32 start;
    s32 end;
    u8 *destination;
    u8 *source_base;
    u8 **source;
    struct ObjectVector *object;

    index = D_0015ED84;
    if ((u32)index >= 0x13U) {
        return;
    }
    next_index = index + 1;
    end = D_001E03B8[next_index];
    start = D_001E03B8[index];
    if (start >= end) {
        return;
    }
    source_base = D_00199478;
    destination = D_0013D5B0 + start * 0x10;
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
