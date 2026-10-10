#include "types.h"
#include "rnc/globals.h"
#include "rnc/rendering/level_render_state.h"
#include "sda.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_0022e420/FUN_0022e420.s", FUN_0022e420);
#else
#include "rnc/globals.h"
#include "rnc/rendering/level_render_state.h"
#include "types.h"
#include "eetypes.h"
#include "sda.h"
#include "qcopy.h"

#include "rnc/rendering/geometry_quad.h"

typedef struct {
    u32 w0;
    u32 w1;
} ColorPair;

extern f32 D_001604D0 __attribute__((sda));
extern u32 D_001D9A30[];
extern u32 D_001D9A34[];

extern u64 get_effect_texture(s32) __asm__("func_001F44B8");
extern void draw_geometry_quad(void *, s32, s32) __asm__("func_001F7D30");
extern void add_vectors(void *, void *, void *) __asm__("func_001F9A10");
extern void subtract_vectors(void *, void *, void *) __asm__("func_001F9A28");
extern void fast_vec_cross(void *, void *, void *) __asm__("func_001F9AD8");
extern void normalize_vector(void *, void *, f32) __asm__("func_001F9BF8");
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern u32 blend_colors(u32, u32, f32) __asm__("func_001FA6E0");

static inline Vec4 *get_primary_history_position(s32 history_index) {
    return &level_render_state.primary_history[history_index];
}

void build_resident_indexed_texture_warp_meshes(void) __asm__("FUN_0022e420");

void build_resident_indexed_texture_warp_meshes(void) {
    struct GeometryQuad quad;
    Vec4 cross_strip_differences[4];
    Vec4 edge_normals[4];
    s32 vertex;
    s32 uv_vertex;
    s32 side_vertex;
    s32 side_history_step;
    f32 side_progress;
    s32 page;
    s32 strip;
    s32 history_index;
    s32 history_step;
    s32 strip_offset;
    f32 progress;
    Vec4 *primary_current;
    Vec4 *primary_next;
    Vec4 *primary_after_next;
    Vec4 *secondary_current;
    Vec4 *secondary_next;

    if (game_mode != 6 || level_render_state.state != 4) {
        quad.texture = get_effect_texture(0x13);
    } else {
        quad.texture = get_effect_texture(0);
    }
    quad.texture_state = 0xFF9000000260;
    quad.primitive = 0x8000000048;
    quad.reserved.ordered = 0;
    for (uv_vertex = 0; uv_vertex < 4; uv_vertex++) {
        quad.texture_coordinates[uv_vertex][0] = warp_texture_coordinates[uv_vertex][0];
        quad.texture_coordinates[uv_vertex][1] = warp_texture_coordinates[uv_vertex][1];
    }
    for (page = 0; page < level_render_state.history_count - 1; page++) {
        history_index = (level_render_state.history_index - page + 0x1F) & 0x1F;
        primary_next = get_primary_history_position((history_index + 1) & 0x1F);
        secondary_next =
            &level_render_state.secondary_history[(history_index + 1) & 0x1F];
        primary_current = get_primary_history_position(history_index);
        secondary_current = &level_render_state.secondary_history[history_index];
        primary_after_next = get_primary_history_position((history_index + 2) & 0x1F);
        for (strip = 0; strip < 2; strip++) {
            strip_offset = strip * 32;
            quad.reserved.ordered = 0;
            subtract_vectors(&cross_strip_differences[0], secondary_current, primary_current);
            subtract_vectors(&cross_strip_differences[1], primary_current, secondary_current);
            subtract_vectors(&cross_strip_differences[2], secondary_next, primary_next);
            subtract_vectors(&cross_strip_differences[3], primary_next, secondary_next);
            subtract_vectors(&edge_normals[1], primary_next + strip_offset,
                             primary_current + strip_offset);
            subtract_vectors(&edge_normals[3], primary_after_next + strip_offset,
                             primary_next + strip_offset);
            if (page == 0) {
                qcopy(&edge_normals[3], &edge_normals[1]);
            }
            fast_vec_cross(&edge_normals[0], &cross_strip_differences[0], &edge_normals[1]);
            fast_vec_cross(&edge_normals[1], &cross_strip_differences[1], &edge_normals[1]);
            fast_vec_cross(&edge_normals[2], &cross_strip_differences[2], &edge_normals[3]);
            fast_vec_cross(&edge_normals[3], &cross_strip_differences[3], &edge_normals[3]);
            for (vertex = 0; vertex < 4; vertex++) {
                history_step = vertex >> 1;
                progress = convert_integer_to_float(page + 1 - history_step) * 0.03125f;
                quad.colors[vertex] =
                    blend_colors(D_001D9A30[level_render_state.content_variant * 2],
                                 D_001D9A34[level_render_state.content_variant * 2], progress);
                normalize_vector(&quad.positions[vertex], &edge_normals[vertex],
                                 (1.0f - progress * progress) *
                                     (&D_001604D0)[level_render_state.content_variant]);
                add_vectors(&quad.positions[vertex], &quad.positions[vertex],
                            get_primary_history_position((history_index + history_step) & 0x1F) +
                                strip_offset);
            }
            draw_geometry_quad(&quad, 0, 0);
            for (side_vertex = 0; side_vertex < 4; side_vertex++) {
                side_history_step = side_vertex >> 1;
                side_progress = convert_integer_to_float(page + 1 - side_history_step) * 0.03125f;
                normalize_vector(&quad.positions[side_vertex],
                                 &cross_strip_differences[side_vertex],
                                 (1.0f - side_progress * side_progress) *
                                     (&D_001604D0)[level_render_state.content_variant]);
                add_vectors(
                    &quad.positions[side_vertex], &quad.positions[side_vertex],
                    get_primary_history_position((history_index + side_history_step) & 0x1F) +
                        strip_offset);
            }
            draw_geometry_quad(&quad, 0, 0);
        }
    }
}

extern __typeof__(build_resident_indexed_texture_warp_meshes) func_0022E420
    __attribute__((alias("FUN_0022e420")));

#endif /* NON_MATCHING */

f32 warp_texture_coordinates[4][2] = {{0, 0.5f}, {1.0f, 0.5f}, {0, 0.5f}, {1.0f, 0.5f}};
