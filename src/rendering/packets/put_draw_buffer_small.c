#include "types.h"
#include "rnc/rendering/dma_tag.h"
extern s32 D_0015EEB8;
void put_draw_buffer_small(void) __asm__("FUN_001fb3d0");

void put_draw_buffer_small(void) {
    *(u32 *)(render_packet_cursor.bytes + 0) = 0x30000009;
    *(u32 *)(render_packet_cursor.bytes + 4) = (D_0015EEB8 + 0xC0) & 0x0FFFFFFF;
    *(u32 *)(render_packet_cursor.bytes + 8) = 0;
    *(u32 *)(render_packet_cursor.bytes + 12) = 0x50000009;
    render_packet_cursor.bytes += 0x10;
}

extern __typeof__(put_draw_buffer_small) func_001FB3D0 __attribute__((alias("FUN_001fb3d0")));
