#include "types.h"

#include "rnc/rendering/fs_aa_packets.h"

void set_background_color(s32 red, u64 green, u64 blue) {
    u64 packed_clear_packet_word;

    packed_clear_packet_word = green << 8;
    packed_clear_packet_word |= red;
    packed_clear_packet_word |= blue << 16;
    packed_clear_packet_word |= 0x8000ULL << 16;
    fs_aa_clear_packet[7] = packed_clear_packet_word;
}
