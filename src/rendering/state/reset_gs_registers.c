#include "types.h"
#include "rnc/rendering/dma_tag.h"
#include "rnc/rendering/view.h"
extern u8 D_0013CFC0[];
extern u8 D_0013CF10[];
extern void vu1_add_g_sregister(s32, u64) __asm__("func_00233980");
void reset_gs_registers(void) __asm__("FUN_001f3868");

void reset_gs_registers(void) {
    struct DmaTag *p;

    render_packet_cursor.tag->tag = 0x30000013;
    render_packet_cursor.tag->addr = (u32)D_0013CFC0;
    render_packet_cursor.tag->vif0 = 0;
    render_packet_cursor.tag->vif1 = 0x50000013;
    p = render_packet_cursor.tag;
    render_packet_cursor.tag = p + 1;
    p[1].tag = 0x3000000B;
    render_packet_cursor.tag->addr = (u32)D_0013CF10;
    render_packet_cursor.tag->vif0 = 0;
    render_packet_cursor.tag->vif1 = 0x5000000B;
    render_packet_cursor.tag++;
    vu1_add_g_sregister(0x3D, view_context.fog_r | ((u64)view_context.fog_g << 8) |
                                  ((u64)view_context.fog_b << 16));
}

extern __typeof__(reset_gs_registers) func_001F3868 __attribute__((alias("FUN_001f3868")));
