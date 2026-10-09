#include "types.h"
#include "rnc/rendering/dma_tag.h"
#include "rnc/rendering/fs_aa_packets.h"
void aa_blur_pass(void) __asm__("FUN_001fb680");

void aa_blur_pass(void) {
    *(u32 *)(render_packet_cursor.bytes + 0) = 0x30000026;
    *(u32 *)(render_packet_cursor.bytes + 4) = (u32)fs_aa_draw_packet;
    *(u32 *)(render_packet_cursor.bytes + 8) = 0;
    *(u32 *)(render_packet_cursor.bytes + 12) = 0x50000026;
    render_packet_cursor.bytes += 0x10;
}

extern __typeof__(aa_blur_pass) func_001FB680
    __attribute__((alias("FUN_001fb680")));
