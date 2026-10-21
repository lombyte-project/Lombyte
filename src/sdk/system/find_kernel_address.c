#include "types.h"

u32 *FindKernelAddress(u32 *addr, u32 *end, u32 key) {
    u32 current_word;
    for (;;) {
        current_word = *addr;
        if (current_word == key) {
            break;
        }
        if (!(addr < end)) {
            break;
        }
        addr++;
    }
    return (addr < end) ? addr : 0;
}
