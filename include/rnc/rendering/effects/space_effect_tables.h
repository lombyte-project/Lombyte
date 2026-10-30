#ifndef LOMBYTE_RNC_RENDERING_EFFECTS_SPACE_EFFECT_TABLES_H
#define LOMBYTE_RNC_RENDERING_EFFECTS_SPACE_EFFECT_TABLES_H

#include "types.h"
#include "rnc/math/vector.h"

/* Tables of the space scene code (draw_light_flare .. update_resident_gameplay_state).
   Their retail .data follows the text order of their readers.

   level_render_state.content_variant is 0, 1 or 2 (do_space_transition sets nothing
   else): the variant tables hold three entries. level_render_state.mode is 0..3
   (a random step & 3) or 4 (update_level_gameplay_frame): the mode tables hold five. */

/* Texture coordinates of a whole-texture quad, (0,0) (1,0) (0,1) (1,1): draw_light_flare
   and draw_resident_textured_quad copy all 0x20 bytes into their GeometryQuad. */
extern f32 unit_quad_texture_coordinates[4][2] __asm__("D_001D97B0");

/* Corner offsets of the light flare quad, per content variant. */
extern f32 light_flare_corner_offsets[3][4][4] __asm__("D_001D97D0");

/* Two GS colours of the warp trail per content variant, as RGBA words 0xAABBGGRR;
   w0's alpha is the one update_level_gameplay_frame fades for all three. */
typedef struct ColorPair {
    u32 w0;
    u32 w1;
} ColorPair;

extern ColorPair warp_trail_colors[3] __asm__("D_001D9A30");

/* draw_resident_textured_quad: the four corners, scaled by the entry of the current
   level (levels 0..0x12; other levels keep scale 1). */
extern f32 resident_quad_base_positions[4][4] __asm__("D_001D9A50");
extern f32 resident_quad_scale_by_level[19] __asm__("D_001D9A90");

/* update_level_gameplay_frame: per level_render_state.mode. */
extern f32 level_render_motion_vectors_by_mode[5][4] __asm__("D_001D9AE0");
extern f32 level_render_depth_by_mode[5] __asm__("D_001D9B30");
extern f32 cam_mode_min_fov[5] __asm__("D_001D9B48");

/* emit_six_spark_effects: six spawn points per content variant. */
extern f32 spark_spawn_points_by_variant[3][6][4] __asm__("D_001D9B60");

/* update_resident_gameplay_state: where the two companions sit relative to the
   player, per content variant. */
extern f32 resident_path_companion_a_local_offsets[3][4] __asm__("D_001D9C80");
extern f32 resident_path_companion_b_local_offsets[3][4] __asm__("D_001D9CB0");

#endif /* LOMBYTE_RNC_RENDERING_EFFECTS_SPACE_EFFECT_TABLES_H */
