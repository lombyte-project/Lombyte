/* Level 01 moby class environment_mapped_meshes (0x738): scrolls the reflection UVs and draws the environment-mapped meshes. */
#include "types.h"
#include "rnc/math/fast_math.h"
#include "rnc/rendering/vu1_registers.h"
#include "rnc/ui/text/font/font_queue_vu_state.h"

typedef struct {
    f32 x, y, z, w;
} EnvVec;

extern EnvVec camera_pos __asm__("D_L01_00167240");
extern f32    level01_environment_map_uv_scroll_offsets[2] __asm__("D_L01_00162110");

void generate_level01_environment_map_uvs(s32 vertex_count,
                                          f32 (*normals)[3],
                                          f32 (*verts)[3],
                                          s32 unused,
                                          f32 (*uv)[2]) __asm__("FUN_L01_0030ef18");

void generate_level01_environment_map_uvs(

    s32 vertex_count, f32 (*normals)[3], f32 (*verts)[3], s32 unused, f32 (*uv)[2]) {
    EnvVec eye;
    EnvVec vertex_position;
    EnvVec refl;
    EnvVec nrm;
    s32    i;
    f32    uv_projection_denominator;

    for (i = 0; i < vertex_count; i++) {
        vertex_position.w = 1.0f;
        vertex_position.x = verts[i][0];
        vertex_position.y = verts[i][1];
        vertex_position.z = verts[i][2];
        fast_vec_sub(&eye, &vertex_position, &camera_pos);
        fast_vec_normalize(&eye, &eye, 1.0f);
        nrm.w = 1.0f;
        nrm.x = -normals[i][0];
        nrm.y = -normals[i][1];
        nrm.z = -normals[i][2];
        fast_vec_normalize(&nrm, &nrm, 1.0f);
        fast_vec_scale(&refl, &nrm, fast_vec_dot(&nrm, &eye) * 2.0f);
        fast_vec_sub(&refl, &eye, &refl);
        fast_vec_normalize(&refl, &refl, 1.0f);
        refl.z += 1.0f;
        uv_projection_denominator = fast_vec_length(&refl) * 2.0f;
        uv[i][0] = (refl.x / uv_projection_denominator + 0.5f) * 2.0f +
                   level01_environment_map_uv_scroll_offsets[0];
        uv[i][1] = (refl.y / uv_projection_denominator + 0.5f) * 2.0f +
                   level01_environment_map_uv_scroll_offsets[1];
    }
}

extern s32 level01_environment_mesh_vertex_counts[] __asm__("D_L01_00202EA0");
extern s32 level01_environment_mesh_position_addresses[] __asm__("D_L01_00208108");
extern s32 D_L01_00208120[];
extern s32 level01_environment_mesh_normal_addresses[] __asm__("D_L01_00208138");
extern s32 level01_environment_mesh_vertex_attribute_addresses[] __asm__("D_L01_00208150");
extern u8  level01_environment_mesh_uv_scratch[] __asm__("D_L01_00208170");
extern s32 get_effect_texture(s32) __asm__("FUN_001f44b8");
void       generate_level01_environment_map_uvs();
void       FUN_L00_001fde98(s32, s32, s32, void *, s32);

void draw_level01_environment_mapped_meshes(void) __asm__("FUN_L01_0030f0e0");

void draw_level01_environment_mapped_meshes(void) {
    s32 i;

    vu1_add_g_sregister(6, get_effect_texture(0x28));
    vu1_add_g_sregister(0x42, 0x2000000064);
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x14, 0xFF9000000260);
    font_queue_vu_state();
    for (i = 0; i < 5; i++) {
        generate_level01_environment_map_uvs(level01_environment_mesh_vertex_counts[i],
                                             level01_environment_mesh_normal_addresses[i],
                                             level01_environment_mesh_position_addresses[i],
                                             D_L01_00208120[i],
                                             level01_environment_mesh_uv_scratch);
        FUN_L00_001fde98(level01_environment_mesh_vertex_counts[i],
                         level01_environment_mesh_position_addresses[i],
                         level01_environment_mesh_vertex_attribute_addresses[i],
                         level01_environment_mesh_uv_scratch,
                         1);
    }
}

typedef struct {
    u8 pad0[0x20];
    u8 state;
} ScrollMoby;

extern f32  frame_delta_time __asm__("D_0015ED7C");
extern void add_draw_callback(void *, void *) __asm__("FUN_001f4600");

void update_level01_environment_map_uv_scroll(ScrollMoby *moby) __asm__("FUN_L01_0030f208");

void update_level01_environment_map_uv_scroll(ScrollMoby *moby) {
    enum {
        UV_SCROLL_STATE_INITIALIZE = 0,
        UV_SCROLL_STATE_UPDATE_AND_QUEUE_DRAW = 1
    };
    f32 uv_scroll_delta;

    switch (moby->state) {
    case UV_SCROLL_STATE_INITIALIZE:
        level01_environment_map_uv_scroll_offsets[0] = 0.0f;
        level01_environment_map_uv_scroll_offsets[1] = 0.0f;
        moby->state = 1;
        break;
    case UV_SCROLL_STATE_UPDATE_AND_QUEUE_DRAW:
        uv_scroll_delta = frame_delta_time * 0.025f;
        level01_environment_map_uv_scroll_offsets[0] += uv_scroll_delta;
        level01_environment_map_uv_scroll_offsets[1] += uv_scroll_delta;
        if (level01_environment_map_uv_scroll_offsets[0] > 1.0f) {
            level01_environment_map_uv_scroll_offsets[0] -= 1.0f;
        }
        if (level01_environment_map_uv_scroll_offsets[0] < -1.0f) {
            level01_environment_map_uv_scroll_offsets[0] += 1.0f;
        }
        if (level01_environment_map_uv_scroll_offsets[1] > 1.0f) {
            level01_environment_map_uv_scroll_offsets[1] -= 1.0f;
        }
        if (level01_environment_map_uv_scroll_offsets[1] < -1.0f) {
            level01_environment_map_uv_scroll_offsets[1] += 1.0f;
        }
        add_draw_callback(draw_level01_environment_mapped_meshes, moby);
        break;
    }
}

f32 level01_environment_map_uv_scroll_offsets[2] __asm__("D_L01_00162110") = {0};
