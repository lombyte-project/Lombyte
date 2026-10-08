#include "types.h"
#include "sda.h"
#define RENDER_PACKET_CURSOR_ATTR MACRO_ADDR
#include "rnc/rendering/dma_tag.h"
extern u8 D_001DEE00[];
void vu1_tex_flush(void) __asm__("FUN_00233b68");

void vu1_tex_flush(void) {
    *render_packet_cursor.words = 0x30000003;
    *(s32 *)((u32)render_packet_cursor.words + 4) = (s32)D_001DEE00;
    *(s32 *)((u32)render_packet_cursor.words + 8) = 0;
    *(s32 *)((u32)render_packet_cursor.words + 12) = 0x50000003;
    render_packet_cursor.words += 4;
}

extern __typeof__(vu1_tex_flush) func_00233B68 __attribute__((alias("FUN_00233b68")));
