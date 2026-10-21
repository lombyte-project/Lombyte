#include "types.h"
#include "rnc/rendering/dma_tag.h"
extern void fast_mem_copy(u8 *, void *, s32) __asm__("func_001F98D0");
void write_vif_unpack_packet(s32 addr, void *src, s32 qwc) __asm__("FUN_00233888");

void write_vif_unpack_packet(s32 addr, void *unpack_payload, s32 qwc) {
    *(u32 *)(render_packet_cursor.bytes + 0) = qwc | 0x10000000;
    *(u32 *)(render_packet_cursor.bytes + 4) = 0;
    *(u32 *)(render_packet_cursor.bytes + 8) = 0x01000404;
    *(u32 *)(render_packet_cursor.bytes + 12) = addr | (qwc << 16) | 0x6C000000;
    render_packet_cursor.bytes += 0x10;
    fast_mem_copy(render_packet_cursor.bytes, unpack_payload, qwc * 16);
    render_packet_cursor.bytes += qwc * 16;
}

extern __typeof__(write_vif_unpack_packet) func_00233888 __attribute__((alias("FUN_00233888")));
