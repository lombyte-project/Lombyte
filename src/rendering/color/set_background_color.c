#include "types.h"

#include "rnc/rendering/fs_aa_packets.h"

void PackDmaTag(s32 arg0, u64 arg1, u64 arg2) {
    u64 value;

    value = arg1 << 8;
    value |= arg0;
    value |= arg2 << 16;
    value |= 0x8000ULL << 16;
    fs_aa_clear_packet[7] = value;
}
