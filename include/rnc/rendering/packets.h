#ifndef LOMBYTE_RNC_RENDERING_PACKETS_H
#define LOMBYTE_RNC_RENDERING_PACKETS_H

#include "types.h"

void append_dma_tag(u32);
void append_power_of_two_textured_screen_sprite(
    u64, s32, s32, s32, s32, s32, s32, s32, s32, s32) __asm__("FUN_00200468");
void emit_textured_rectangle(f32, f32, f32, f32, s32, s32, s32, s32, u64, u64) __asm__("FUN_001f5808");
void pack_render_command_fields(u64 *, u64, u64, u64, u64) __asm__("PackRenderCommandFields");
void put_disp_buffer(void) __asm__("FUN_001fb2a8");
void set_up_vis_gif_viewer(u64 *, s32, s32, s32, s32, s32) __asm__("FUN_00202fd0");
void write_vif_unpack_packet(s32, void *, s32) __asm__("FUN_00233888");

#endif /* LOMBYTE_RNC_RENDERING_PACKETS_H */
