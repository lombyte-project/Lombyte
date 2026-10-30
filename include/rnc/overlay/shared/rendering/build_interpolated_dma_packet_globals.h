#ifndef LOMBYTE_RNC_OVERLAY_SHARED_RENDERING_BUILD_INTERPOLATED_DMA_PACKET_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_RENDERING_BUILD_INTERPOLATED_DMA_PACKET_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/rendering/build_interpolated_dma_packet.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern s32   scaled_table_header_leading_word __asm__("D_L02_001612A0");
extern s32   scaled_table_header_second_word __asm__("D_L02_001612A4");
extern s32   scaled_table_header_third_word __asm__("D_L02_001612A8");
extern s32   scaled_table_header_final_word __asm__("D_L02_001612AC");
extern int   textured_grid_fog_source_mode __asm__("D_L02_0015F458");
extern float textured_grid_saved_fog_near_distance __asm__("D_L02_001612F8");
extern float textured_grid_saved_fog_far_distance __asm__("D_L02_001612FC");
extern float textured_grid_saved_fog_near_intensity __asm__("D_L02_00161300");
extern float textured_grid_saved_fog_far_intensity __asm__("D_L02_00161304");
extern int   tresspasser_ring_arc_first_vertex_uv __asm__("D_L02_00161B0C");
extern int   tresspasser_ring_arc_second_vertex_uv __asm__("D_L02_00161B10");
extern int   tresspasser_ring_arc_third_vertex_uv __asm__("D_L02_00161B14");
extern int   tresspasser_ring_arc_fourth_vertex_uv __asm__("D_L02_00161B18");
extern int   tresspasser_arrow_newly_reached_first_vertex_uv __asm__("D_L02_00161ABC");
extern int   tresspasser_arrow_newly_reached_second_vertex_uv __asm__("D_L02_00161AC0");
extern int   tresspasser_arrow_newly_reached_third_vertex_uv __asm__("D_L02_00161AC4");
extern int   tresspasser_arrow_newly_reached_fourth_vertex_uv __asm__("D_L02_00161AC8");
extern int   tresspasser_arrow_occupied_first_vertex_uv __asm__("D_L02_00161ACC");
extern int   tresspasser_arrow_occupied_second_vertex_uv __asm__("D_L02_00161AD0");
extern int   tresspasser_arrow_occupied_third_vertex_uv __asm__("D_L02_00161AD4");
extern int   tresspasser_arrow_occupied_fourth_vertex_uv __asm__("D_L02_00161AD8");
extern int   tresspasser_arrow_lit_first_vertex_uv __asm__("D_L02_00161ADC");
extern int   tresspasser_arrow_lit_second_vertex_uv __asm__("D_L02_00161AE0");
extern int   tresspasser_arrow_lit_third_vertex_uv __asm__("D_L02_00161AE4");
extern int   tresspasser_arrow_lit_fourth_vertex_uv __asm__("D_L02_00161AE8");
extern int   tresspasser_arrow_vertex_color __asm__("D_L02_00161B9C");
extern int   tresspasser_textured_quad_color __asm__("D_L02_00161B94");
extern int   tresspasser_quad_first_vertex_uv __asm__("D_L02_00161AAC");
extern int   tresspasser_quad_second_vertex_uv __asm__("D_L02_00161AB0");
extern int   tresspasser_quad_third_vertex_uv __asm__("D_L02_00161AB4");
extern int   tresspasser_quad_fourth_vertex_uv __asm__("D_L02_00161AB8");
extern int   escalator_direction_timer_frames __asm__("D_L02_00161C18");
extern float escalator_target_speed __asm__("D_L02_00161C14");
extern float escalator_acceleration __asm__("D_L02_00161C1C");
extern float versatarget_particle_base_lifetime __asm__("D_L02_00161CB0");
extern int   versatarget_particle_color __asm__("D_L02_00161CB4");
extern float versatarget_particle_scale __asm__("D_L02_00161CB8");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_RENDERING_BUILD_INTERPOLATED_DMA_PACKET_GLOBALS_H */
