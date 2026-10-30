#include "types.h"

#include "rnc/rendering/fs_aa_packets.h"

void set_background_color(s32 red, u64 green, u64 blue) {
    u64 value;

    value = green << 8;
    value |= red;
    value |= blue << 16;
    value |= 0x8000ULL << 16;
    fs_aa_clear_packet[7] = value;
}
