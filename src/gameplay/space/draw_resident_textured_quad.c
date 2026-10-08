#include "types.h"
#include "rnc/globals.h"

struct GeometryQuad {
    f32 positions[4][4];
    u32 colors[4];
    u8 texture_coordinates[0x20];
    u64 reserved;
    u64 texture;
    u64 texture_state;
    u64 primitive;
};

extern u64 D_00160580;
extern u8 D_001D97B0[];
extern f32 D_001D9A90[];
extern f32 D_001D9A50[][4];
extern s32 D_001604F0 __attribute__((sda));
extern void vu1_add_g_sregister(s32, u64) __asm__("FUN_00233980");
extern void copy_blocks_16_forward(void *, void *, s32) __asm__("FUN_001f98d0");
extern void scale_vector_xyz(f32 *, f32 *, f32) __asm__("FUN_001f9a68");
extern void add_vector_xyz(f32 *, f32 *, f32 *) __asm__("FUN_001f9a10");
extern void draw_geometry_quad(struct GeometryQuad *, s32, s32) __asm__("func_001F7D30");

void draw_resident_textured_quad(void) __asm__("FUN_0022e8c8");

void draw_resident_textured_quad(void) {
    struct GeometryQuad quad;
    f32 scale;
    s32 vertex;

    vu1_add_g_sregister(0x47, 0x31801);
    scale = 1.0f;
    quad.texture = D_00160580;
    quad.texture_state = 0xFF9000000260;
    quad.primitive = 0x8000000044;
    quad.reserved = 0;
    copy_blocks_16_forward(quad.texture_coordinates, D_001D97B0, 0x20);
    if ((u32)current_level_index < 0x13) {
        scale = D_001D9A90[current_level_index];
    }
    for (vertex = 0; vertex < 4; vertex++) {
        quad.colors[vertex] = 0x80808080;
        scale_vector_xyz(quad.positions[vertex], D_001D9A50[vertex], scale);
        add_vector_xyz(quad.positions[vertex], quad.positions[vertex], (f32 *)&D_001604F0);
    }
    draw_geometry_quad(&quad, 0, 0);
    vu1_add_g_sregister(0x47, 0x5360B);
}

extern __typeof__(draw_resident_textured_quad) func_0022E8C8 __attribute__((alias("FUN_0022e8c8")));
