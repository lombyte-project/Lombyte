#include "types.h"
extern u8 D_0018A2B0[];
void ClearDmaQueueEntry(void) {
    s32 *entry;
    s32 remaining;
    s32 value;

    entry = (s32 *)D_0018A2B0;
    value = 1;
    remaining = 0x13;
    entry = (s32 *)((u8 *)entry + 0x4C);
    do {
        *entry = value;
        remaining -= 1;
        entry -= 1;
    } while (remaining >= 0);
}
