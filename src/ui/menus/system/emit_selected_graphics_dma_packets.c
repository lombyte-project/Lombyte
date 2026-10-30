#include "types.h"
#include "rnc/rendering/dma_tag.h"

struct Mat {
    u8 pad[0x30];
};

extern struct Mat graphics_dma_reference_records[] __asm__("D_001D7D90");
extern s32  graphics_packet_payload_words_by_slot[2] __asm__("D_00160360") __attribute__((sda));
extern void FUN_00228598(s32, s32);

void emit_selected_graphics_dma_packets(s32 render_selector,
                                        s32 slot,
                                        s32 mat) __asm__("FUN_00227140");

void emit_selected_graphics_dma_packets(s32 render_selector,
                                        s32 slot,

                                        s32 dma_reference_record_index) {
    render_packet_cursor.tag->tag = 0x30000003;
    render_packet_cursor.tag->addr =
        (u32)&graphics_dma_reference_records[dma_reference_record_index];
    render_packet_cursor.tag->vif0 = 0x13000000;
    render_packet_cursor.tag->vif1 = 0x50000003;
    render_packet_cursor.tag++;
    FUN_00228598(render_selector, graphics_packet_payload_words_by_slot[slot]);
}

extern __typeof__(emit_selected_graphics_dma_packets) func_00227140
    __attribute__((alias("FUN_00227140")));
