#include "types.h"

typedef struct ResourceEntry {
    s32 reserved;
    s32 value;
    s32 unused[2];
} ResourceEntry;

extern ResourceEntry ResourceTable[64] __asm__("D_001DD1D8") __attribute__((section(".data")));

s32 LookupResourceEntry(s32 resource_index) {
    if ((u32)resource_index >= 0x40u) {
        return -3;
    }
    return ResourceTable[resource_index].value;
}
