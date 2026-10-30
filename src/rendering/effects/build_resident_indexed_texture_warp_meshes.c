/* Ported from rac1-decomp (src/game/space.c: func_0022F738). */
#include "types.h"
#include "rnc/rendering/effects/space_effect_tables.h"
#include "sda.h"
#include "qcopy.h"
#include "rnc/globals.h"
#include "rnc/rendering/geometry_quad.h"
#include "rnc/rendering/level_render_state.h"
#include "rnc/math/fast_math.h"

extern s32 game_mode_macro __asm__("D_0015F604") MACRO_ADDR;

extern f32 warp_trail_widths __asm__("D_001604D0") __attribute__((sda));

extern s64  get_effect_texture(s32) __asm__("func_001F44B8");
extern void draw_geometry_quad(void *, s32, s32) __asm__("func_001F7D30");
extern void fast_vec_add(void *, void *, void *) __asm__("func_001F9A10");
extern void fast_vec_sub(void *, void *, void *) __asm__("func_001F9A28");
extern void fast_vec_cross(void *, void *, void *) __asm__("func_001F9AD8");
extern void fast_vec_normalize(void *, void *, f32) __asm__("func_001F9BF8");
extern u32  fast_tween_color(u32, u32, f32) __asm__("func_001FA6E0");

static inline Vec4 *get_history_position(s32 strip, s32 history_index) {
    return &((Vec4(*)[32])level_render_state.primary_history)[strip][history_index];
}

void build_resident_indexed_texture_warp_meshes(void) __asm__("FUN_0022e420");

void build_resident_indexed_texture_warp_meshes(void) {
    struct GeometryQuad quad;
    struct {
        Vec4 cross_strip_differences[4];
        Vec4 edge_normals[4];
    } workspace;
    s32   vertex;
    s32   uv_vertex;
    s32   history_index;
    Vec4 *secondary_current;
    Vec4 *primary_current;
    Vec4 *secondary_next;
    Vec4 *primary_next;
    Vec4 *primary_after_next;
    Vec4 *position;
    Vec4 *side_position;
    Vec4 *side_normal;

    if (game_mode_macro == 6 && level_render_state.state == 4) {
        s64 texture_low;
        quad.texture = get_effect_texture(0);
        texture_low = quad.texture & 0xFF;
        /* Fakematch: the always-true test leaves the critical edge to the 0x13
           fallback unsplit, so gcse PRE places the loop invariants as retail. */
        if (texture_low >= 0)
            goto texture_ready;
    }
    quad.texture = get_effect_texture(0x13);
texture_ready:
    quad.texture_state = 0xFF9000000260;
    quad.primitive = 0x8000000048;
    quad.reserved.value = 0;
    {
        f32 *warp_uv_pair;
        f32 *dst_v;
        f32 *dst_u;
        warp_uv_pair = &warp_texture_coordinates[0][0];
        dst_v = &quad.texture_coordinates[0][1];
        dst_u = &quad.texture_coordinates[0][0];
        uv_vertex = 3;
        do {
            dst_u[0] = warp_uv_pair[0];
            dst_v[0] = warp_uv_pair[1];
            warp_uv_pair += 2;
            dst_u += 2;
            dst_v += 2;
        } while (--uv_vertex >= 0);
    }
    {
        /* Fakematch: 30 unused locals raise the page and strip pseudo numbers,
           which moves their gcse PRE hash buckets and gives retail's spill slots. */
        s32 pad0, pad1, pad2, pad3, pad4, pad5, pad6, pad7, pad8, pad9;
        s32 pad10, pad11, pad12, pad13, pad14, pad15, pad16, pad17, pad18, pad19;
        s32 pad20, pad21, pad22, pad23, pad24, pad25, pad26, pad27, pad28, pad29;
        s32 page;
        for (page = 0; page < level_render_state.history_count - 1; page++) {
            history_index = (level_render_state.history_index - page + 0x1F) & 0x1F;
            secondary_current = &level_render_state.secondary_history[history_index];
            primary_current = &level_render_state.primary_history[history_index];
            secondary_next = &level_render_state.secondary_history[(history_index + 1) & 0x1F];
            {
                s32 strip;
                primary_next = &level_render_state.primary_history[(history_index + 1) & 0x1F];
                primary_after_next =
                    &level_render_state.primary_history[(history_index + 2) & 0x1F];
                for (strip = 0; strip < 2; strip++) {
                    fast_vec_sub(
                        &workspace.cross_strip_differences[0], secondary_current, primary_current);
                    fast_vec_sub(
                        &workspace.cross_strip_differences[1], primary_current, secondary_current);
                    fast_vec_sub(
                        &workspace.cross_strip_differences[2], secondary_next, primary_next);
                    fast_vec_sub(
                        &workspace.cross_strip_differences[3], primary_next, secondary_next);
                    fast_vec_sub(&workspace.edge_normals[1],
                                 (Vec4 *)((strip << 9) + (u32)primary_next),
                                 (Vec4 *)((strip << 9) + (u32)primary_current));
                    fast_vec_sub(&workspace.edge_normals[3],
                                 (Vec4 *)((strip << 9) + (u32)primary_after_next),
                                 (Vec4 *)((strip << 9) + (u32)primary_next));
                    if (page == 0) {
                        qcopy(&workspace.edge_normals[3], &workspace.edge_normals[1]);
                    }
                    fast_vec_cross(&workspace.edge_normals[0],
                                   &workspace.cross_strip_differences[0],
                                   &workspace.edge_normals[1]);
                    fast_vec_cross(&workspace.edge_normals[1],
                                   &workspace.cross_strip_differences[1],
                                   &workspace.edge_normals[1]);
                    fast_vec_cross(&workspace.edge_normals[2],
                                   &workspace.cross_strip_differences[2],
                                   &workspace.edge_normals[3]);
                    fast_vec_cross(&workspace.edge_normals[3],
                                   &workspace.cross_strip_differences[3],
                                   &workspace.edge_normals[3]);
                    position = &quad.positions[0];
                    for (vertex = 0; vertex < 4; vertex++) {
                        s32 history_step;
                        s32 age;
                        f32 progress;
                        history_step = vertex >> 1;
                        age = history_step;
                        age--;
                        progress = convert_integer_to_float(page - age) * 0.03125f;
                        quad.colors[vertex] = fast_tween_color(
                            warp_trail_colors[level_render_state.content_variant].w0,
                            warp_trail_colors[level_render_state.content_variant].w1,
                            progress);
                        progress = 1.0f - progress * progress;
                        fast_vec_normalize(
                            position,
                            &workspace.edge_normals[vertex],
                            progress * (&warp_trail_widths)[level_render_state.content_variant]);
                        fast_vec_add(
                            position,
                            position,
                            get_history_position(strip, (history_index + history_step) & 0x1F));
                        position++;
                    }
                    draw_geometry_quad(&quad, 0, 0);
                    side_normal = &workspace.cross_strip_differences[0];
                    vertex = 0;
                    for (side_position = &quad.positions[0]; vertex < 4; vertex++) {
                        s32 history_step;
                        s32 age;
                        f32 progress;
                        history_step = vertex >> 1;
                        age = history_step;
                        age--;
                        progress = convert_integer_to_float(page - age) * 0.03125f;
                        fast_vec_normalize(
                            side_position,
                            side_normal,
                            (1.0f - progress * progress) *
                                (&warp_trail_widths)[level_render_state.content_variant]);
                        fast_vec_add(
                            side_position,
                            side_position,
                            get_history_position(strip, (history_index + history_step) & 0x1F));
                        side_normal++;
                        side_position++;
                    }
                    draw_geometry_quad(&quad, 0, 0);
                }
            }
        }
    }
}

extern __typeof__(build_resident_indexed_texture_warp_meshes) func_0022E420
    __attribute__((alias("FUN_0022e420")));

f32 warp_texture_coordinates[4][2] = {
    {   0, 0.5f},
    {1.0f, 0.5f},
    {   0, 0.5f},
    {1.0f, 0.5f}
};

ColorPair warp_trail_colors[3] = {
    {0x3880a0c0, 0x2058b0},
    {0x3880a0c0, 0x2058b0},
    {0x3880c080, 0x308030},
};
