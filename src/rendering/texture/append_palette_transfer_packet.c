#include "types.h"
#include "rnc/rendering/dma_tag.h"
extern u8 D_00151B60[];
void append_palette_transfer_packet(void) __asm__("FUN_001fb6e0");

void append_palette_transfer_packet(void) {
    *(u32 *)(render_packet_cursor.bytes + 0) = 0x30000029;
    *(u32 *)(render_packet_cursor.bytes + 4) = (u32)D_00151B60;
    *(u32 *)(render_packet_cursor.bytes + 8) = 0;
    *(u32 *)(render_packet_cursor.bytes + 12) = 0x50000029;
    render_packet_cursor.bytes += 0x10;
}

extern __typeof__(append_palette_transfer_packet) func_001FB6E0
    __attribute__((alias("FUN_001fb6e0")));
