/* Shared moby class ocean_pokitaru: the level's water sheet. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/rendering/vu1_registers.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/ui/text/font/font_queue_vu_state.h"
#include "qcopy.h"

typedef struct {
    float u, v;
} UV;

typedef struct {
    float x, y, z;
} Pos;

typedef struct {
    Vec4f v[4];
    int   col[4];
    UV    uv[4];
    long  z70;
    long  z78;
    long  q80;
    long  q88;
} Quad;

typedef struct {
    int   f0;
    int   f4;
    int   f8;
    int   fC;
    int   f10;
    float f14;
    float f18;
    int   f1C;
    int   f20;
    float f24;
    float f28;
    float f2C;
} Data;

typedef struct {
    char  pad[0x228];
    float f228, f22C;
} Fog;

/* Ocean update: scrolls the two wave layers with the camera and the time, keeps their offsets in -1..1 and sets the level's tide values. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002C99E0.c: func_L11_0030C728), where it is exact; names translated to the US level program. */
typedef float V4_30b358[4] __attribute__((aligned(16)));

typedef struct {
    char  pad00[0x14];
    float level; /* 0x14 */
    float scale; /* 0x18 */
    char  pad1C[0x4];
    float limit; /* 0x20 */
} OceanVars;

typedef struct {
    char          pad00[0x20];
    unsigned char state; /* 0x20 */
    char          pad21[0xF];
    unsigned char f30; /* 0x30 */
    char          pad31[0x47];
    OceanVars    *vars; /* 0x78 */
} OceanMoby;

extern int fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");

extern void raise_kernel_trap(void) __asm__("FUN_001f97b0");

extern int   get_effect_texture(int) __asm__("FUN_001f44b8");
extern void  func_L00_001FDE48_30a908(int, void *, void *, void *, int) __asm__("FUN_L00_001fde98");
extern void  draw_geometry_quad(void *, void *, int) __asm__("FUN_001f7d30");
extern float D_L11_001677C0_30a908[] __asm__("D_L11_001677C0");
extern UV    wave_offset[] __asm__("D_L11_00161E08") MACRO_ADDR;

extern char  render_fog_context[] __asm__("D_L11_0016D440");
extern char  ocean_strip_orientation_basis[] __asm__("D_L11_001679D0");
extern Pos   l11_ocean_strip_vertex_positions[] __asm__("D_L11_001D9B08");
extern int   ocean_strip_vertex_colors[] __asm__("D_L11_001D9E08");
extern UV    l11_ocean_strip_vertex_uvs[] __asm__("D_L11_001DA310");
extern Pos   l11_ocean_wave_grid_vertex_positions[] __asm__("D_L11_001D9F08");
extern int   D_L11_001DA208[];
extern UV    l11_ocean_wave_grid_vertex_uvs[] __asm__("D_L11_001DA510");
extern float wave_speed[2] __asm__("D_L11_00161DF8") __attribute__((sda));
extern float tide_level_macro __asm__("D_L11_00161E18") MACRO_ADDR;

extern float camera_pos_b358[] __asm__("D_L11_001677C0");
extern char  last_camera_pos[4] __asm__("D_L11_00162840")
    __attribute__((sda)); /* a 16-byte vector; declared small so it is reached through $gp */
extern float ocean_level __asm__("D_L11_00161E1C");
extern UV    wave_drift[] __asm__("D_L11_001D9AF0");
extern float frame_delta_time __asm__("D_0015ED7C");

/* Draws the level's water sheet: a fading edge strip, a far quad, and two scrolling layers of grid cells around the camera. */

void draw_pokitaru_ocean(struct Moby *moby) __asm__("FUN_L11_0030a908");

void draw_pokitaru_ocean(struct Moby *moby) {
    Vec4f scratch_vector;
    Vec4f side_offset;
    Vec4f strip_length_direction;
    Vec4f strip_width_direction;
    Vec4f strip_corners[4];
    Vec4f first_strip_edge_delta;
    Vec4f second_strip_edge_delta;
    Quad  far_quad;
    Data *ocean_vars = (Data *)moby->pvars;
    float saved_fog_228 = ((Fog *)render_fog_context)->f228;
    float s22C = ((Fog *)render_fog_context)->f22C;
    int   i;
    int   layer;
    int   colA;
    int   colB;
    int   vertex_count;
    float color_interpolation_scalar;
    float x;
    float y;

    if (hero.state.current != 0x1D && D_L11_0016D180 == 0) {
        ((Fog *)render_fog_context)->f228 = 255.0f;
        ((Fog *)render_fog_context)->f22C = 249.9f;
    }
    vu1_add_g_sregister(0x42, 0x3200000044L);
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x14, 0xFF9000000260L);
    vu1_add_g_sregister(0x47, 0x513F1);
    vu1_add_g_sregister(6, get_effect_texture(0x16));
    font_queue_vu_state();
    strip_length_direction.x = 1.0f;
    strip_length_direction.y = 0.0f;
    strip_length_direction.z = 0.0f;
    strip_length_direction.w = 1.0f;
    transform_vector_by_basis(
        &strip_length_direction, &strip_length_direction, ocean_strip_orientation_basis);
    strip_width_direction.x = 0.0f;
    strip_width_direction.y = 1.0f;
    strip_width_direction.z = 0.0f;
    strip_width_direction.w = 1.0f;
    transform_vector_by_basis(
        &strip_width_direction, &strip_width_direction, ocean_strip_orientation_basis);
    strip_length_direction.z = 0.0f;
    fast_vec_normalize(&strip_length_direction, &strip_length_direction, 1.0f);
    strip_width_direction.z = 0.0f;
    fast_vec_normalize(&strip_width_direction, &strip_width_direction, 1.0f);
    fast_vec_normalize(&scratch_vector, &strip_length_direction, -75.0f);
    fast_vec_normalize(&side_offset, &strip_width_direction, -87.0f);
    fast_vec_add(&strip_corners[0], &scratch_vector, &side_offset);
    fast_vec_normalize(&side_offset, &strip_width_direction, 87.0f);
    fast_vec_add(&strip_corners[2], &scratch_vector, &side_offset);
    fast_vec_normalize(&scratch_vector, &strip_length_direction, ocean_vars->f28);
    fast_vec_normalize(&side_offset, &strip_width_direction, -750.0f);
    fast_vec_add(&strip_corners[1], &scratch_vector, &side_offset);
    fast_vec_normalize(&side_offset, &strip_width_direction, 750.0f);
    fast_vec_add(&strip_corners[3], &scratch_vector, &side_offset);
    for (i = 0; i < 4; i++) {
        Vec4f *strip_corner = &strip_corners[i];
        fast_vec_add(strip_corner, strip_corner, D_L11_001677C0_30a908);
        strip_corner->z = tide_level_macro;
    }
    fast_vec_sub(&first_strip_edge_delta, &strip_corners[1], &strip_corners[0]);
    fast_vec_sub(&second_strip_edge_delta, &strip_corners[3], &strip_corners[2]);
    color_interpolation_scalar = 10.0f;
    for (i = 0; i < 12; i += 2) {
        float strip_fraction = convert_integer_to_float(i) / 10.0f;
        int   col = fast_tween_color(ocean_vars->f0,
                                   ocean_vars->f4,
                                   convert_integer_to_float(i) / color_interpolation_scalar);
        fast_vec_scale(&scratch_vector, &first_strip_edge_delta, strip_fraction);
        ocean_strip_vertex_colors[i] = col;
        l11_ocean_strip_vertex_uvs[i].u = 0.0f;
        l11_ocean_strip_vertex_uvs[i].v = 0.0f;
        fast_vec_add(&scratch_vector, &scratch_vector, &strip_corners[0]);
        l11_ocean_strip_vertex_positions[i].x = scratch_vector.x;
        l11_ocean_strip_vertex_positions[i].y = scratch_vector.y;
        l11_ocean_strip_vertex_positions[i].z = scratch_vector.z;
        fast_vec_scale(&scratch_vector, &second_strip_edge_delta, strip_fraction);
        ocean_strip_vertex_colors[i + 1] = col;
        l11_ocean_strip_vertex_uvs[i + 1].u = 0.0f;
        l11_ocean_strip_vertex_uvs[i + 1].v = 0.0f;
        fast_vec_add(&scratch_vector, &scratch_vector, &strip_corners[2]);
        l11_ocean_strip_vertex_positions[i + 1].x = scratch_vector.x;
        l11_ocean_strip_vertex_positions[i + 1].y = scratch_vector.y;
        l11_ocean_strip_vertex_positions[i + 1].z = scratch_vector.z;
    }
    func_L00_001FDE48_30a908(0xC,
                             l11_ocean_strip_vertex_positions,
                             ocean_strip_vertex_colors,
                             l11_ocean_strip_vertex_uvs,
                             1);
    far_quad.z78 = get_effect_texture(0x16);
    far_quad.q80 = 0xFF9000000260L;
    far_quad.q88 = 0x8000000044L;
    far_quad.z70 = 0;
    if (ocean_vars->f14 < l11_camera.pos.f[2] - ocean_vars->f24 && hero.state.current != 0x1D &&
        D_L11_0016D180 == 0) {
        far_quad.uv[0].u = 0.0f;
        far_quad.uv[0].v = 0.0f;
        far_quad.uv[1].u = 0.0f;
        far_quad.uv[1].v = 0.0f;
        far_quad.uv[2].u = 0.0f;
        far_quad.uv[2].v = 0.0f;
        far_quad.uv[3].u = 0.0f;
        far_quad.uv[3].v = 0.0f;
        far_quad.col[0] = ocean_vars->f4;
        far_quad.col[1] = ocean_vars->f8;
        far_quad.col[2] = far_quad.col[0];
        far_quad.col[3] = far_quad.col[1];
        *(u128 *)&far_quad.v[0] = *(u128 *)&strip_corners[1];
        *(u128 *)&far_quad.v[1] = *(u128 *)&strip_corners[1];
        far_quad.v[1].z = l11_camera.pos.f[2] - ocean_vars->f24;
        *(u128 *)&far_quad.v[2] = *(u128 *)&strip_corners[3];
        *(u128 *)&far_quad.v[3] = *(u128 *)&strip_corners[3];
        far_quad.v[3].z = l11_camera.pos.f[2] - ocean_vars->f24;
        draw_geometry_quad(&far_quad, 0, 0);
    }
    vu1_add_g_sregister(0x42, 0x8000000048L);
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x14, 0xFF9000000260L);
    vu1_add_g_sregister(0x47, 0x513F1);
    colA = ocean_vars->fC;
    colB = ocean_vars->f10;
    color_interpolation_scalar = 1.0f - (l11_camera.pos.f[2] - tide_level_macro) / ocean_vars->f2C;
    if (color_interpolation_scalar > 1.0f) {
        color_interpolation_scalar = 1.0f;
    }
    if (color_interpolation_scalar >= 0.0f) {
        colA = fast_tween_color(0, colA, color_interpolation_scalar);
        colB = fast_tween_color(0, colB, color_interpolation_scalar);
        for (layer = 0; layer < 2; layer++) {
            int id = layer + 0x28;
            vu1_add_g_sregister(6, get_effect_texture(ocean_vars->f1C + id));
            for (y = ocean_vars->f18 * -2.0f; y < ocean_vars->f18 * 2.0f; y += ocean_vars->f18) {
                vertex_count = 0;
                for (x = ocean_vars->f18 * -2.0f; x < ocean_vars->f18 * 2.0f;
                     x += ocean_vars->f18) {
                    l11_ocean_wave_grid_vertex_uvs[vertex_count].u =
                        x / (ocean_vars->f18 * 2.0f) * wave_speed[layer] + wave_offset[layer].u;
                    l11_ocean_wave_grid_vertex_uvs[vertex_count].v =
                        y / (ocean_vars->f18 * 2.0f) * wave_speed[layer] + wave_offset[layer].v;
                    l11_ocean_wave_grid_vertex_positions[vertex_count].x =
                        x + l11_camera.pos.f[0] + 1.0f;
                    l11_ocean_wave_grid_vertex_positions[vertex_count].y =
                        y + l11_camera.pos.f[1] + 1.0f;
                    l11_ocean_wave_grid_vertex_positions[vertex_count].z = tide_level_macro;
                    i = vertex_count + 1;
                    l11_ocean_wave_grid_vertex_uvs[i].u =
                        x / (ocean_vars->f18 * 2.0f) * wave_speed[layer] + wave_offset[layer].u;
                    l11_ocean_wave_grid_vertex_uvs[i].v =
                        (y + ocean_vars->f18) / (ocean_vars->f18 * 2.0f) * wave_speed[layer] +
                        wave_offset[layer].v;
                    l11_ocean_wave_grid_vertex_positions[i].x = x + l11_camera.pos.f[0] + 1.0f;
                    l11_ocean_wave_grid_vertex_positions[i].y =
                        y + ocean_vars->f18 + l11_camera.pos.f[1] + 1.0f;
                    l11_ocean_wave_grid_vertex_positions[i].z = tide_level_macro;
                    i = vertex_count + 2;
                    l11_ocean_wave_grid_vertex_uvs[i].u =
                        (x + ocean_vars->f18) / (ocean_vars->f18 * 2.0f) * wave_speed[layer] +
                        wave_offset[layer].u;
                    l11_ocean_wave_grid_vertex_uvs[i].v =
                        y / (ocean_vars->f18 * 2.0f) * wave_speed[layer] + wave_offset[layer].v;
                    l11_ocean_wave_grid_vertex_positions[i].x =
                        x + ocean_vars->f18 + l11_camera.pos.f[0] + 1.0f;
                    l11_ocean_wave_grid_vertex_positions[i].y = y + l11_camera.pos.f[1] + 1.0f;
                    l11_ocean_wave_grid_vertex_positions[i].z = tide_level_macro;
                    i = vertex_count + 3;
                    l11_ocean_wave_grid_vertex_uvs[i].u =
                        (x + ocean_vars->f18) / (ocean_vars->f18 * 2.0f) * wave_speed[layer] +
                        wave_offset[layer].u;
                    l11_ocean_wave_grid_vertex_uvs[i].v =
                        (y + ocean_vars->f18) / (ocean_vars->f18 * 2.0f) * wave_speed[layer] +
                        wave_offset[layer].v;
                    l11_ocean_wave_grid_vertex_positions[i].x =
                        x + ocean_vars->f18 + l11_camera.pos.f[0] + 1.0f;
                    l11_ocean_wave_grid_vertex_positions[i].y =
                        y + ocean_vars->f18 + l11_camera.pos.f[1] + 1.0f;
                    l11_ocean_wave_grid_vertex_positions[i].z = tide_level_macro;
                    for (i = vertex_count; i < vertex_count + 4; i++) {
                        scratch_vector.x =
                            l11_ocean_wave_grid_vertex_positions[i].x - l11_camera.pos.f[0];
                        scratch_vector.y =
                            l11_ocean_wave_grid_vertex_positions[i].y - l11_camera.pos.f[1];
                        scratch_vector.z =
                            l11_ocean_wave_grid_vertex_positions[i].z - l11_camera.pos.f[2];
                        scratch_vector.w = 1.0f;
                        color_interpolation_scalar =
                            vector_length_xy(&scratch_vector) / (ocean_vars->f18 * 2.0f);
                        if (color_interpolation_scalar > 1.0f) {
                            color_interpolation_scalar = 1.0f;
                        }
                        D_L11_001DA208[i] =
                            fast_tween_color(colA, colB, color_interpolation_scalar);
                    }
                    vertex_count += 4;
                    if (vertex_count >= 0x40) {
                        raise_kernel_trap();
                    }
                }
                func_L00_001FDE48_30a908(vertex_count,
                                         l11_ocean_wave_grid_vertex_positions,
                                         D_L11_001DA208,
                                         l11_ocean_wave_grid_vertex_uvs,
                                         1);
            }
        }
    }
    ((Fog *)render_fog_context)->f228 = saved_fog_228;
    ((Fog *)render_fog_context)->f22C = s22C;
    vu1_add_g_sregister(0x47, 0x5360B);
}

void ocean_pokitaru_update(OceanMoby *moby) __asm__("FUN_L11_0030b358");

void ocean_pokitaru_update(OceanMoby *moby) {
    OceanVars *ocean_vars = moby->vars;
    float      camera_displacement[4];
    float      out[4];
    int        k;

    if (moby->state == 0) {
        moby->state = 1;
        moby->f30 = 0xFF;
        for (k = 0; k < 2; k++) {
            wave_offset[k].u = 0.0f;
            wave_offset[k].v = 0.0f;
        }
        qcopy(last_camera_pos, camera_pos_b358);
        ocean_level = ocean_vars->level;
        return;
    }
    fast_vec_sub(camera_displacement, camera_pos_b358, last_camera_pos);
    qcopy(last_camera_pos, camera_pos_b358);
    if (!(ocean_vars->limit < camera_pos_b358[2])) {
        return;
    }
    for (k = 0; k < 2; k++) {
        fast_vec_scale(
            out, camera_displacement, (wave_speed[k] + wave_speed[k]) / (ocean_vars->scale * 5.0f));
        if (1.0f < out[0] || out[0] < 1.0f) {
            out[0] = out[0] - convert_integer_to_float(truncate_float_to_s32(out[0]));
        }
        if (1.0f < out[1] || out[1] < 1.0f) {
            out[1] = out[1] - convert_integer_to_float(truncate_float_to_s32(out[1]));
        }
        wave_offset[k].u = wave_offset[k].u + out[0];
        wave_offset[k].v = wave_offset[k].v + out[1];
        if (1.0f < wave_offset[k].u) {
            wave_offset[k].u = wave_offset[k].u - 1.0f;
        }
        if (wave_offset[k].u < -1.0f) {
            wave_offset[k].u = wave_offset[k].u + 1.0f;
        }
        if (1.0f < wave_offset[k].v) {
            wave_offset[k].v = wave_offset[k].v - 1.0f;
        }
        if (wave_offset[k].v < -1.0f) {
            wave_offset[k].v = wave_offset[k].v + 1.0f;
        }
    }
    tide_level =
        ocean_level +
        fast_sin(convert_integer_to_float(level_frame % 360) * 0.017444445f - 3.14f) * 0.25f;
    for (k = 0; k < 2; k++) {
        float sum = wave_offset[k].u + wave_drift[k].u * frame_delta_time;
        wave_offset[k].u = sum;
        if (1.0f < sum) {
            wave_offset[k].u = sum - 1.0f;
        }
        if (wave_offset[k].u < -1.0f) {
            wave_offset[k].u = wave_offset[k].u + 1.0f;
        }
        sum = wave_offset[k].v + wave_drift[k].v * frame_delta_time;
        wave_offset[k].v = sum;
        if (1.0f < sum) {
            wave_offset[k].v = sum - 1.0f;
        }
        if (wave_offset[k].v < -1.0f) {
            wave_offset[k].v = wave_offset[k].v + 1.0f;
        }
    }
    add_draw_callback((void *)draw_pokitaru_ocean, moby);
}

f32 D_L16_001D8A28[1] __attribute__((section(".data"))) = {0};

f32 D_L16_001D8A2C[1] __attribute__((section(".data"))) = {0};

f32 D_L16_001D8A30[1] __attribute__((section(".data"))) = {0};

f32 D_L16_001D8A34[1] __attribute__((section(".data"))) = {0};

f32 D_L16_001D8A38[1] __attribute__((section(".data"))) = {0};

f32 D_L16_001D8A3C[1] __attribute__((section(".data"))) = {0};

s32 D_L16_001D8D28[1] __attribute__((section(".data"))) = {0};

s32 D_L16_001D8D2C[1] __attribute__((section(".data"))) = {0};

s32 D_L16_001D9230[1] __attribute__((section(".data"))) = {0};

s32 D_L16_001D9234[1] __attribute__((section(".data"))) = {0};

s32 D_L16_001D9238[1] __attribute__((section(".data"))) = {0};

s32 D_L16_001D923C[1] __attribute__((section(".data"))) = {0};

int D_L16_001D9128 NOT_SDA = 0;
float              D_L16_00161C50[2] = {2.0f, 3.0f};

float D_L16_00161C70 = 0;

char D_L16_00162240[4] __attribute__((section(".bss")));

float ocean_level __asm__("D_L11_00161E1C") = 0;
