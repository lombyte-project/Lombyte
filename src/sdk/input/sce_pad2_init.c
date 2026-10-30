#include "types.h"

struct Pad2StateEntry {
    s32 state;
    s32 request;
    s32 result;
};

extern u32 pad2_initialized[] __asm__("D_00132DD0");
extern u8  D_0015B540[];

s32 scePad2Init(void) {
    struct Pad2StateEntry *entry;

    pad2_initialized[0] = 1;
    entry = (struct Pad2StateEntry *)D_0015B540;
    do {
        entry->state = 0;
        entry->request = 0;
        entry->result = 0;
        entry = (struct Pad2StateEntry *)((u8 *)entry + 0x330);
    } while ((s32)entry < (s32)(D_0015B540 + 0x3300));
    return 1;
}
