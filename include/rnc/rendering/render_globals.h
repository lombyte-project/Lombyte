#ifndef LOMBYTE_RNC_RENDERING_RENDER_GLOBALS_H
#define LOMBYTE_RNC_RENDERING_RENDER_GLOBALS_H

#include "types.h"

extern int  draw_buffer_address __asm__("D_0015EE84");
extern s32  gs_texture_allocation_base __asm__("D_0015EE8C");
extern s32  screen_overlay_enabled __asm__("D_0015F34C") __attribute__((sda));
extern s32  occl_debug_overlay_enabled __asm__("D_0015F370") __attribute__((sda));
extern s32  draw_enable_mask __asm__("D_0015F434");
extern s32  occl_chain_active __asm__("D_0015F444");
extern s32  occl_chain_frames __asm__("D_0015F448");
extern s32  pending_texture_upload_count __asm__("D_0015F458");
extern s32  effect_tex_cnt __asm__("D_0015F45C");
extern s32  level_texture_payload_address __asm__("D_0015F460");
extern s32  draw_callback_count __asm__("D_0015F464");
extern s32  draw_callback2_count __asm__("D_0015F468");
extern s32  draw_callback3_count __asm__("D_0015F46C");
extern s32  draw_callback4_count __asm__("D_0015F470");
extern s32  effect_quad_count __asm__("D_0015F474");
extern u8   fog_color_red __asm__("D_0015F484");
extern u8   fog_color_green __asm__("D_0015F485");
extern u8   fog_color_blue __asm__("D_0015F486");
extern f32  fog_near_distance __asm__("D_0015F488");
extern f32  fog_far_distance __asm__("D_0015F48C");
extern f32  fog_near_intensity __asm__("D_0015F490");
extern f32  fog_far_intensity __asm__("D_0015F494");
extern s32  part_clip_dist __asm__("D_001600BC");
extern char textured_quad_header[] __asm__("D_00160840");
extern s32  current_vu_chain __asm__("D_00160F0C");
extern s32  surface_height_tiles __asm__("D_00161190");
extern s32  occl_cell_table __asm__("D_00161194");
extern s32  occl_cell_count __asm__("D_00161198");
extern void (*draw_callback_funcs[])(s32) __asm__("D_0018DB40");
extern s32 draw_callback_args[] __asm__("D_0018DC40");
extern void (*draw_callback2_funcs[])(s32) __asm__("D_0018DD40");
extern s32 draw_callback2_args[] __asm__("D_0018DE40");
extern u8  occl_visibility[] __asm__("D_00193FC0");
extern u8  hud_chan_slots[] __asm__("D_00199B60");
extern u8  point_light_data[] __asm__("D_0019C3C0");
extern u8  tie_light_data[] __asm__("D_001E3200");

#endif /* LOMBYTE_RNC_RENDERING_RENDER_GLOBALS_H */
