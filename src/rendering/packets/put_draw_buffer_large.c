#include "types.h"
#include "rnc/rendering/dma_tag.h"
extern s32 D_0015EEB8;
extern void sceGsPutDrawEnv(s32);
void put_draw_buffer_large(void) __asm__("FUN_001fb2d0");

void put_draw_buffer_large(void) {
    if (render_packet_cursor.bytes != 0) {
        *(u32 *)(render_packet_cursor.bytes + 0) = 0x30000009;
        *(u32 *)(render_packet_cursor.bytes + 4) = (D_0015EEB8 + 0x30) & 0x0FFFFFFF;
        *(u32 *)(render_packet_cursor.bytes + 8) = 0;
        *(u32 *)(render_packet_cursor.bytes + 12) = 0x50000009;
        render_packet_cursor.bytes += 0x10;
    } else {
        sceGsPutDrawEnv(D_0015EEB8 + 0x30);
    }
}

extern __typeof__(put_draw_buffer_large) func_001FB2D0
    __attribute__((alias("FUN_001fb2d0")));
