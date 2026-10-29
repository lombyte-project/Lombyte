#include "types.h"

void ClearMemoryRange(u8 *destination, s32 length) __asm__("Memclr");

void ClearMemoryRange(u8 *destination, s32 length) {
    register s32 remaining = length - 1;
    if (length != 0) {
        do {
            *destination = 0;
            remaining -= 1;
            destination += 1;
        } while (remaining != -1);
    }
}
