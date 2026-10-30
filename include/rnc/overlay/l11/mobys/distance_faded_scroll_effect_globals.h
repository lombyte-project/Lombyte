#ifndef LOMBYTE_RNC_OVERLAY_L11_MOBYS_DISTANCE_FADED_SCROLL_EFFECT_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L11_MOBYS_DISTANCE_FADED_SCROLL_EFFECT_GLOBALS_H

#include "types.h"
#include "rnc/overlay/mesh_batch.h"

/* Scalar data read by src/overlays/l11/mobys/distance_faded_scroll_effect.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern int   distance_faded_effect_texture_29_fixed_alpha_scale __asm__("D_L11_001625CC");
extern int   distance_faded_effect_texture_2a_pair_one_fixed_alpha __asm__("D_L11_001625EC");
extern float distance_faded_effect_position_x __asm__("D_L11_001625B8");
extern float distance_faded_effect_position_y __asm__("D_L11_001625BC");
extern float distance_faded_effect_position_z __asm__("D_L11_001625C0");
extern int   distance_faded_effect_color_tables_initialized __asm__("D_L11_001625FC");

/* The oil tanker front texture 2b batches (5): vertex counts and colour tables,
   also read by queue_oil_tanker_front_texture_2b_batches. */
extern s32  oil_tanker_front_texture_2b_batch_vertex_counts[5] __asm__("D_L11_00215268");
extern u32 *oil_tanker_front_texture_2b_batch_color_tables[5] __asm__("D_L11_00217740");

#endif /* LOMBYTE_RNC_OVERLAY_L11_MOBYS_DISTANCE_FADED_SCROLL_EFFECT_GLOBALS_H */
