#include "types.h"

u32 CheckAddress(u32 address) __asm__("CheckAddress");

u32 CheckAddress(u32 address) {
    if ((address >> 28) == 7u) {
        address = address & 0x0FFFFFFFu;
        address = address | 0x80000000u;
    }
    return address;
}
