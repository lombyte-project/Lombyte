#include "types.h"
#include "rnc/rendering/dma_tag.h"
extern u8 D_00151900[];
void aa_blur_pass(void) __asm__("FUN_001fb680");

void aa_blur_pass(void) {
    *(u32 *)(render_packet_cursor.bytes + 0) = 0x30000026;
    *(u32 *)(render_packet_cursor.bytes + 4) = (u32)D_00151900;
    *(u32 *)(render_packet_cursor.bytes + 8) = 0;
    *(u32 *)(render_packet_cursor.bytes + 12) = 0x50000026;
    render_packet_cursor.bytes += 0x10;
}

extern __typeof__(aa_blur_pass) func_001FB680
    __attribute__((alias("FUN_001fb680")));
