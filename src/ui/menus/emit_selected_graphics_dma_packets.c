#include "types.h"
#include "rnc/rendering/dma_tag.h"
struct Mat {
    u8 pad[0x30];
};
extern struct Mat D_001D7D90[];
extern s32 D_00160360[2] __attribute__((sda));
extern void FUN_00228598(s32, s32);

void emit_selected_graphics_dma_packets(s32 render_selector, s32 slot, s32 mat) __asm__("FUN_00227140");

void emit_selected_graphics_dma_packets(s32 render_selector, s32 slot, s32 dma_reference_record_index) {
    render_packet_cursor.tag->tag = 0x30000003;
    render_packet_cursor.tag->addr = (u32)&D_001D7D90[dma_reference_record_index];
    render_packet_cursor.tag->vif0 = 0x13000000;
    render_packet_cursor.tag->vif1 = 0x50000003;
    render_packet_cursor.tag++;
    FUN_00228598(render_selector, D_00160360[slot]);
}

extern __typeof__(emit_selected_graphics_dma_packets) func_00227140 __attribute__((alias("FUN_00227140")));
