#include "types.h"

extern u64 D_00152078[];

void PackDmaTag(s32 arg0, u64 arg1, u64 arg2) {
    u64 value;

    value = arg1 << 8;
    value |= arg0;
    value |= arg2 << 16;
    value |= 0x8000ULL << 16;
    D_00152078[0] = value;
}
