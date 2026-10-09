#include "types.h"

#include "rnc/rendering/fs_aa_packets.h"
extern u8 *volatile D_00160F00;

void append_gif_transfer_packet(void) __asm__("FUN_001fb368");

void append_gif_transfer_packet(void) {
    u8 *base;

    base = D_00160F00;
    if (base != 0) {
        *(s32 *)(base + 0x0) = 0x30000015;
        *(u8 **)(D_00160F00 + 0x4) = (u8 *)fs_aa_clear_packet;
        *(s32 *)(D_00160F00 + 0x8) = 0;
        *(s32 *)(D_00160F00 + 0xC) = 0x50000015;
        D_00160F00 += 0x10;
    }
}

extern __typeof__(append_gif_transfer_packet) func_001FB368 __attribute__((alias("FUN_001fb368")));
