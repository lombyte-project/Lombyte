#include "types.h"
#include "sda.h"
#define RENDER_PACKET_CURSOR_ATTR MACRO_ADDR
#include "rnc/rendering/dma_tag.h"

void vu1_add_vif_code(s32 arg0) __asm__("FUN_00233938");

void vu1_add_vif_code(s32 arg0) {
    *render_packet_cursor.words = 0x10000000;
    *(s32 *)((u32)render_packet_cursor.words + 4) = 0;
    *(s32 *)((u32)render_packet_cursor.words + 8) = 0;
    *(s32 *)((u32)render_packet_cursor.words + 12) = arg0;
    render_packet_cursor.words += 4;
}

extern __typeof__(vu1_add_vif_code) func_00233938 __attribute__((alias("FUN_00233938")));
