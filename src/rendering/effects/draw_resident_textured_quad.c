#include "types.h"
#include "rnc/rendering/effects/space_effect_tables.h"
#include "rnc/globals.h"

#include "rnc/rendering/geometry_quad.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"

extern u64  D_00160580;
extern s32  D_001604F0 __attribute__((sda));
extern void vu1_add_g_sregister(s32, u64) __asm__("FUN_00233980");
extern void draw_geometry_quad(struct GeometryQuad *, s32, s32) __asm__("func_001F7D30");

void draw_resident_textured_quad(void) __asm__("FUN_0022e8c8");

void draw_resident_textured_quad(void) {
    struct GeometryQuad quad;
    f32                 scale;
    s32                 vertex;

    vu1_add_g_sregister(0x47, 0x31801);
    scale = 1.0f;
    quad.texture = D_00160580;
    quad.texture_state = 0xFF9000000260;
    quad.primitive = 0x8000000044;
    quad.reserved.value = 0;
    fast_mem_copy(quad.texture_coordinates, unit_quad_texture_coordinates, 0x20);
    if ((u32)current_level_index < 0x13) {
        scale = resident_quad_scale_by_level[current_level_index];
    }
    for (vertex = 0; vertex < 4; vertex++) {
        quad.colors[vertex] = 0x80808080;
        fast_vec_scale(quad.positions[vertex].f, resident_quad_base_positions[vertex], scale);
        fast_vec_add(quad.positions[vertex].f, quad.positions[vertex].f, (f32 *)&D_001604F0);
    }
    draw_geometry_quad(&quad, 0, 0);
    vu1_add_g_sregister(0x47, 0x5360B);
}

extern __typeof__(draw_resident_textured_quad) func_0022E8C8 __attribute__((alias("FUN_0022e8c8")));

f32 resident_quad_base_positions[4][4] = {
    {-80.0f, 0.0f,  80.0f, 0.0f},
    { 80.0f, 0.0f,  80.0f, 0.0f},
    {-80.0f, 0.0f, -80.0f, 0.0f},
    { 80.0f, 0.0f, -80.0f, 0.0f},
};

f32 resident_quad_scale_by_level[19] = {0.8f,
                                        1.0f,
                                        0.9f,
                                        1.4f,
                                        0.9f,
                                        1.1f,
                                        1.3f,
                                        0.8f,
                                        1.2f,
                                        1.5f,
                                        1.1f,
                                        0.7f,
                                        1.3f,
                                        1.2f,
                                        1.1f,
                                        1.0f,
                                        1.2f,
                                        1.1f,
                                        0.8f};
