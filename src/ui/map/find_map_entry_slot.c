#include "types.h"

extern u8 D_001A00F0[];

s32 find_map_entry_slot(s32 arg0) __asm__("FUN_002050a0");

s32 find_map_entry_slot(s32 arg0) {
    register u8 *base;
    register u8 *ptr;
    register s32 count = 0;

    base = D_001A00F0;
    ptr = base + 0x28C;
    do {
        if (*(s32 *)(ptr - 0x14) != 0 && *(s32 *)ptr == arg0) {
            return count;
        }
        count += 1;
        ptr += 4;
    } while (count < 5);
    return -1;
}
