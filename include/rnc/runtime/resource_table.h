#ifndef LOMBYTE_RNC_RUNTIME_RESOURCE_TABLE_H
#define LOMBYTE_RNC_RUNTIME_RESOURCE_TABLE_H

#include "types.h"

typedef struct ResourceEntry {
    s32 reserved;
    s32 value;
    s32 unused[2];
} ResourceEntry;

extern ResourceEntry ResourceTable[64] __asm__("D_001DD1D8") __attribute__((section(".data")));

#endif /* LOMBYTE_RNC_RUNTIME_RESOURCE_TABLE_H */
