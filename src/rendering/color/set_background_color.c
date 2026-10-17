#include "types.h"

#include "rnc/rendering/fs_aa_packets.h"

void PackDmaTag(s32 low_component, u64 middle_component, u64 high_component) {
    u64 value;

    value = middle_component << 8;
    value |= low_component;
    value |= high_component << 16;
    value |= 0x8000ULL << 16;
    fs_aa_clear_packet[7] = value;
}
