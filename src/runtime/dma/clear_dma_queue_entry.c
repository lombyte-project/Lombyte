#include "types.h"
#include "rnc/rendering/draw_config.h"
void ClearDmaQueueEntry(void) {
    s32 *entry;
    s32 remaining;
    s32 value;

    entry = (s32 *)&draw_config;
    value = 1;
    remaining = 0x13;
    entry = (s32 *)((u8 *)entry + 0x4C);
    do {
        *entry = value;
        remaining -= 1;
        entry -= 1;
    } while (remaining >= 0);
}
