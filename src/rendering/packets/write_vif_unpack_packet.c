#include "types.h"
#include "rnc/rendering/dma_tag.h"
extern void func_001F98D0(u8 *, void *, s32);
void write_vif_unpack_packet(s32 addr, void *src, s32 qwc) __asm__("FUN_00233888");

void write_vif_unpack_packet(s32 addr, void *src, s32 qwc) {
    *(u32 *)(render_packet_cursor.bytes + 0) = qwc | 0x10000000;
    *(u32 *)(render_packet_cursor.bytes + 4) = 0;
    *(u32 *)(render_packet_cursor.bytes + 8) = 0x01000404;
    *(u32 *)(render_packet_cursor.bytes + 12) = addr | (qwc << 16) | 0x6C000000;
    render_packet_cursor.bytes += 0x10;
    func_001F98D0(render_packet_cursor.bytes, src, qwc * 16);
    render_packet_cursor.bytes += qwc * 16;
}

extern __typeof__(write_vif_unpack_packet) func_00233888 __attribute__((alias("FUN_00233888")));
