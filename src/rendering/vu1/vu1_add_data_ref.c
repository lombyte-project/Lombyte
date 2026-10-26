#include "types.h"
#include "sda.h"
#define RENDER_PACKET_CURSOR_ATTR MACRO_ADDR
#include "rnc/rendering/dma_tag.h"
void vu1_add_data_ref(s32 data_addr, s32 qword_count) __asm__("FUN_00233830");

void vu1_add_data_ref(s32 data_addr, s32 qword_count) {
    *render_packet_cursor.words = qword_count | 0x30000000;
    *(s32 *)((u32)render_packet_cursor.words + 4) = data_addr;
    *(s32 *)((u32)render_packet_cursor.words + 8) = 0;
    *(s32 *)((u32)render_packet_cursor.words + 12) = 0;
    render_packet_cursor.words += 4;
}

extern __typeof__(vu1_add_data_ref) func_00233830 __attribute__((alias("FUN_00233830")));
