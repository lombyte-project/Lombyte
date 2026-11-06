#include "types.h"
#include "rnc/rendering/dma_tag.h"
#include "rnc/rendering/depth_test_packets.h"

/* Small data, defined above the code so it is reached $gp-relative. */
s32 graphics_packet_payload_words_by_slot[2] __asm__("D_00160360") = {0x7F8080FF, (s32)0x8280FF80};

extern void FUN_00228598(s32, s32);

void emit_selected_graphics_dma_packets(s32 render_selector,
                                        s32 slot,
                                        s32 mat) __asm__("FUN_00227140");

void emit_selected_graphics_dma_packets(s32 render_selector,
                                        s32 slot,

                                        s32 dma_reference_record_index) {
    render_packet_cursor.tag->tag = 0x30000003;
    render_packet_cursor.tag->addr = (u32)&gs_depth_test_packets[dma_reference_record_index];
    render_packet_cursor.tag->vif0 = 0x13000000;
    render_packet_cursor.tag->vif1 = 0x50000003;
    render_packet_cursor.tag++;
    FUN_00228598(render_selector, graphics_packet_payload_words_by_slot[slot]);
}

extern __typeof__(emit_selected_graphics_dma_packets) func_00227140
    __attribute__((alias("FUN_00227140")));

struct GsDepthTestPacket gs_depth_test_packets[4] = {
    {{GIF_TAG_NLOOP(2) | GIF_TAG_EOP | GIF_TAG_NREG(1), 0xEEEEEE},
     GS_TEST_ATE | GS_TEST_AFAIL_FB | GS_TEST_ZTE | GS_TEST_ZTST(GS_ZTST_NEVER),
     GS_REG_TEST_1, 0,
     GS_REG_TEXFLUSH},
    {{GIF_TAG_NLOOP(2) | GIF_TAG_EOP | GIF_TAG_NREG(1), 0xEEEEEE},
     GS_TEST_ATE | GS_TEST_AFAIL_FB | GS_TEST_ZTE | GS_TEST_ZTST(GS_ZTST_ALWAYS),
     GS_REG_TEST_1, 0,
     GS_REG_TEXFLUSH},
    {{GIF_TAG_NLOOP(2) | GIF_TAG_EOP | GIF_TAG_NREG(1), 0xEEEEEE},
     GS_TEST_ATE | GS_TEST_AFAIL_FB | GS_TEST_ZTE | GS_TEST_ZTST(GS_ZTST_GEQUAL),
     GS_REG_TEST_1, 0,
     GS_REG_TEXFLUSH},
    {{GIF_TAG_NLOOP(2) | GIF_TAG_EOP | GIF_TAG_NREG(1), 0xEEEEEE},
     GS_TEST_ATE | GS_TEST_AFAIL_FB | GS_TEST_ZTE | GS_TEST_ZTST(GS_ZTST_GREATER),
     GS_REG_TEST_1, 0,
     GS_REG_TEXFLUSH},
};
