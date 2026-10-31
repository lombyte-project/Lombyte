#include "types.h"
#include "qcopy.h"
#include "rnc/rendering/view.h"
#include "rnc/rendering/dma_tag.h"
#include "rnc/data_labels.h"
#include "rnc/ui/text/font_state.h"
#include "rnc/camera/camera_globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"

extern void vu1_add_data_ref(u8 *, s32) __asm__("func_00233830");
extern void vu1_gs_regs_font(void) __asm__("func_00233C90");

void font_queue_vu_state(void) __asm__("FUN_001f76a0");

void font_queue_vu_state(void) {
    f32            camera_relative_scale_matrix[4][4];
    struct DmaTag *dma_packet_start;
    u8            *packet_write_ptr;

    set_scale_matrix(camera_relative_scale_matrix, 1024.0f);
    fast_vec_scale(camera_relative_scale_matrix[3], camera_position, -1024.0f);
    camera_relative_scale_matrix[3][3] = 1.0f;
    if (font_state != 7) {
        vu1_add_data_ref(font_vu_program, font_vu_program_tag[0]);
        font_state = 7;
    }
    render_packet_cursor.tag->tag = 0x10000000;
    render_packet_cursor.tag->addr = 0;
    render_packet_cursor.tag->vif0 = 0x11000000;
    render_packet_cursor.tag->vif1 = 0x1000404;
    dma_packet_start = render_packet_cursor.tag;
    dma_packet_start[1].tag = 0;
    dma_packet_start[1].addr = 0;
    dma_packet_start[1].vif0 = 0;
    dma_packet_start[1].vif1 = 0x6C0C43A4;
    packet_write_ptr = (u8 *)(dma_packet_start + 2);
    multiply_matrix(packet_write_ptr, camera_position - 0x100, camera_relative_scale_matrix);
    *(f32 *)(packet_write_ptr + 0x38) += font_depth_bias;
    packet_write_ptr = (u8 *)(dma_packet_start + 6);
    multiply_matrix(packet_write_ptr, camera_position - 0x80, camera_relative_scale_matrix);
    *(f32 *)(packet_write_ptr + 0x38) += font_depth_bias;
    dma_packet_start[10].tag = 0x8000;
    dma_packet_start[10].addr = 0x303EC000;
    dma_packet_start[10].vif0 = 0x412;
    *(f32 *)&dma_packet_start[10].vif1 = view_context.fog_mul;
    packet_write_ptr = (u8 *)(dma_packet_start + 11);
    qcopy(packet_write_ptr, &view_context.screen_scale);
    packet_write_ptr = (u8 *)(dma_packet_start + 12);
    qcopy(packet_write_ptr, &view_context.screen_bias);
    *(f32 *)&dma_packet_start[13].tag = view_context.fog_far_int;
    *(f32 *)&dma_packet_start[13].addr = view_context.fog_near_int;
    dma_packet_start[13].vif0 = 0;
    dma_packet_start[13].vif1 = 0;
    dma_packet_start[14].tag = 0x3000000;
    dma_packet_start[14].addr = 0x20001D2;
    dma_packet_start[14].vif0 = 0x15000000;
    dma_packet_start[14].vif1 = 0;
    packet_write_ptr = (u8 *)(dma_packet_start + 15);
    render_packet_cursor.tag->tag |=
        (((u8 *)packet_write_ptr - (u8 *)render_packet_cursor.tag) >> 4) - 1;
    render_packet_cursor.tag = (struct DmaTag *)packet_write_ptr;
    vu1_gs_regs_font();
}

extern __typeof__(font_queue_vu_state) func_001F76A0 __attribute__((alias("FUN_001f76a0")));
/* Recovered original symbol name. */
extern __typeof__(font_queue_vu_state) FontQueueVUState __attribute__((alias("FUN_001f76a0")));
