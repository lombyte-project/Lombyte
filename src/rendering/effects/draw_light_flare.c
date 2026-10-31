/* Ported from rac1-decomp (src/game/space.c, func_0022F258). */
#include "sda.h"
#include "rnc/rendering/level_render_state.h"
#include "types.h"
#include "rnc/rendering/effects/space_effect_tables.h"
#include "rnc/data_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/world/collision/ground_probe.h"

extern int get_effect_texture(int) __asm__("FUN_001f44b8");

typedef struct {
    float positions[4][4];           /* 0x00 */
    int   colors[4];                 /* 0x40 */
    char  texture_coordinates[0x20]; /* 0x50 */
    long  gs_state[4];               /* 0x70 */
} FlareQuad;

extern void draw_geometry_quad(void *, int, int) __asm__("func_001F7D30");
extern void scale_vector(void *, void *, float) __asm__("FUN_001f9a80");
extern int  FUN_001fa820(void *, int *, float);
/* Draws a light flare quad at arg0's position (+0x00; the matrix is at
   +0xC0) when FUN_001fa820 finds it on screen, with the alpha it
   returns. In D_0015F604 mode 6 with flare set 3, the flare fades as
   level_render_state's level passes scale_game_frames(150), and its depth comes from
   probe_ground_height. The corners are D_001D97D0[set]. */
void draw_light_flare(char *object) __asm__("FUN_0022df40");

extern s32 game_mode_macro __asm__("D_0015F604") MACRO_ADDR;

void draw_light_flare(char *flare_object) {
    float     scaled_position[4];
    FlareQuad quad;
    float     scaled_position_copy[4];
    int       alpha;
    float     depth;
    int       corner_index;

    alpha = 0;
    scale_vector(scaled_position, flare_object, 1.0f / 1024.0f);
    if (FUN_001fa820(scaled_position, &alpha, 32.0f) < 0) {
        return;
    }
    if (game_mode_macro == 6 && level_render_state.state == 3 &&
        level_render_state.timer > scale_game_frames(150)) {
        alpha -= (level_render_state.timer - scale_game_frames(150)) * 4;
        if (alpha <= 0) {
            return;
        }
    }
    quad.gs_state[1] = get_effect_texture(0);
    quad.gs_state[2] = 0xFF9000000260;
    quad.gs_state[3] = 0x8000000044;
    quad.gs_state[0] = 0;
    fast_mem_copy(quad.texture_coordinates, unit_quad_texture_coordinates, 0x20);
    fast_vec_scale(scaled_position_copy, flare_object, 1.0f / 1024.0f);
    depth = *(float *)(flare_object + 0x18) + 0.1f;
    if (game_mode_macro == 6) {
        depth = probe_ground_height(flare_object + 0x10, 0, 0.5f) + 0.1f;
    }
    for (corner_index = 0; corner_index < 4; corner_index++) {
        float *vertex_position;

        quad.colors[corner_index] = ((alpha >> 1) << 24) | 0x808080;
        vertex_position = quad.positions[corner_index];
        transform_vector_by_basis(
            vertex_position,
            light_flare_corner_offsets[level_render_state.content_variant][corner_index],
            flare_object + 0xC0);
        fast_vec_add(vertex_position, vertex_position, flare_object + 0x10);
        vertex_position[2] = depth;
    }
    draw_geometry_quad(&quad, 0, 0);
}

extern __typeof__(draw_light_flare) func_0022DF40 __attribute__((alias("FUN_0022df40")));

f32 unit_quad_texture_coordinates[4][2] = {
    {0.0f, 0.0f},
    {1.0f, 0.0f},
    {0.0f, 1.0f},
    {1.0f, 1.0f},
};

f32 light_flare_corner_offsets[3][4][4] = {
    {{4.0f, 1.6f, 0.0f, 0.0f},
     {4.0f, -1.6f, 0.0f, 0.0f},
     {-4.0f, 1.6f, 0.0f, 0.0f},
     {-4.0f, -1.6f, 0.0f, 0.0f}},
    {{4.0f, 2.0f, 0.0f, 0.0f},
     {4.0f, -2.0f, 0.0f, 0.0f},
     {-4.0f, 2.0f, 0.0f, 0.0f},
     {-4.0f, -2.0f, 0.0f, 0.0f}},
    {{3.5f, 1.5f, 0.0f, 0.0f},
     {3.5f, -1.5f, 0.0f, 0.0f},
     {-3.5f, 2.0f, 0.0f, 0.0f},
     {-3.5f, -2.0f, 0.0f, 0.0f}},
};
