#include "types.h"
#include "rnc/rendering/dma_tag.h"
extern u8 D_0013CC90[];
extern void vu1_add_g_sregister(s32, u64) __asm__("func_00233980");
void emit_rgba_draw_packet(s32 r, s32 g, s32 b, s32 a) __asm__("FUN_001f5210");

void emit_rgba_draw_packet(s32 r, s32 g, s32 b, s32 a) {
    vu1_add_g_sregister(1, (u64)r | ((u64)g << 8) | ((u64)b << 16) | ((u64)a << 24));
    *(u32 *)(render_packet_cursor.bytes + 0) = 0x30000014;
    *(u32 *)(render_packet_cursor.bytes + 4) = (u32)D_0013CC90;
    *(u32 *)(render_packet_cursor.bytes + 8) = 0;
    *(u32 *)(render_packet_cursor.bytes + 12) = 0x50000014;
    render_packet_cursor.bytes += 0x10;
}

extern __typeof__(emit_rgba_draw_packet) func_001F5210 __attribute__((alias("FUN_001f5210")));
