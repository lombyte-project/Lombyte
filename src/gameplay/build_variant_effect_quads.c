#include "rnc/rendering/geometry_quad.h"
#include "rnc/rendering/level_render_state.h"
#include "sda.h"
#include "types.h"

typedef struct { float f[4]; } __attribute__((aligned(16))) QuadOffset;

extern f32 quad_uvs[4][2] __asm__("D_001D9890") __attribute__((section(".data")));
extern QuadOffset quad_positions[8] __asm__("D_001D98B0") __attribute__((section(".data")));
extern QuadOffset quad_positions_variant1[2] __asm__("D_001D9930") __attribute__((section(".data")));
extern QuadOffset quad_positions_variant2[2] __asm__("D_001D9950") __attribute__((section(".data")));
extern QuadOffset quad_corners[10] __asm__("D_001D9970") __attribute__((section(".data")));

void FUN_001f9a68_28efc8(float, void *, void *) __asm__("FUN_001f9a68");
void FUN_001f9a10_28efc8(void *, void *, void *) __asm__("FUN_001f9a10");
void FUN_001f9cf8_28efc8(void *, void *, void *) __asm__("FUN_001f9cf8");
long FUN_001f44b8_28efc8(int) __asm__("FUN_001f44b8");
int FUN_00213260_28efc8(int) __asm__("FUN_00213260");
float FUN_001fa6c0_28efc8(int) __asm__("func_001FA6C0");
void FUN_L00_001fd228_28efc8(void *, void *, int) __asm__("func_001F7D30");
extern int D_001604C0[1] __attribute__((sda));
void build_variant_effect_quads(unsigned char *quad_source) __asm__("FUN_0022e1b0");

void build_variant_effect_quads(unsigned char *quad_source) {
    struct GeometryQuad quad;
    QuadOffset quad_source_translation;
    QuadOffset *selected_quad_offsets;
    int i, j, k;
    selected_quad_offsets = quad_positions;
    if (level_render_state.content_variant == 1) selected_quad_offsets = quad_positions_variant1;
    else if (level_render_state.content_variant == 2) selected_quad_offsets = quad_positions_variant2;
    quad.texture = FUN_001f44b8_28efc8(5);
    quad.texture_state = 0xFF9000000260L;
    quad.primitive = 0x8000000048L;
    quad.reserved.value = 0;
    for (j = 0; j < 4; j++) {
        quad.texture_coordinates[j][0] = quad_uvs[j][0];
        quad.texture_coordinates[j][1] = quad_uvs[j][1];
    }
    FUN_001f9a68_28efc8(0.0009765625f, &quad_source_translation, quad_source);
    for (i = 0; i < D_001604C0[level_render_state.content_variant]; i++) {
        int quad_alpha = quad_source[0xBC];
        unsigned int col;
        float quad_scale;
        if (*(short *)(quad_source + 0xB2)) quad_alpha += FUN_00213260_28efc8(*(short *)(quad_source + 0xB2));
        quad_scale = FUN_001fa6c0_28efc8(quad_alpha) * (selected_quad_offsets[i].f[3] / 40.0f);
        col = (quad_alpha << 24) | 0x2058B0;
        if (*(short *)(quad_source + 0xA6) == 0x215) col = (quad_alpha << 24) | 0x308000;
        for (k = 0; k < 4; k++) {
            quad.colors[k] = col;
            FUN_001f9a68_28efc8(quad_scale, &quad.positions[k], &quad_corners[k]);
            FUN_001f9a10_28efc8(&quad.positions[k], &quad.positions[k], &selected_quad_offsets[i]);
            FUN_001f9cf8_28efc8(&quad.positions[k], &quad.positions[k], (char *)level_render_state.player + 0xC0);
            FUN_001f9a10_28efc8(&quad.positions[k], &quad.positions[k], &quad_source_translation);
        }
        FUN_L00_001fd228_28efc8(&quad, 0, 0);
    }
}

extern __typeof__(build_variant_effect_quads) func_0022E1B0 __attribute__((alias("FUN_0022e1b0")));

f32 quad_uvs[4][2] = {{0, 0}, {1.0f, 0}, {0, 1.0f}, {1.0f, 1.0f}};

QuadOffset quad_positions[8] = {{{-3.3f, 1.85f, 0.13f, 0.7f}}, {{-3.3f, 1.42f, 0.42f, 0.7f}}, {{-3.3f, 1.42f, -0.15f, 0.7f}}, {{-3.3f, 0.8f, 1.6f, 0.7f}}, {{-3.3f, -1.85f, 0.13f, 0.7f}}, {{-3.3f, -1.42f, 0.42f, 0.7f}}, {{-3.3f, -1.42f, -0.15f, 0.7f}}, {{-3.3f, -0.8f, 1.6f, 0.7f}}};

QuadOffset quad_positions_variant1[2] = {{{-4.75f, 1.75f, 0.5f, 1.2f}}, {{-4.75f, -1.75f, 0.5f, 1.2f}}};

QuadOffset quad_positions_variant2[2] = {{{-5.7f, 0.9f, -0.4f, 1.15f}}, {{-5.7f, -0.9f, -0.4f, 1.15f}}};

QuadOffset quad_corners[10] = {{{0.0f, -1.0f, 1.0f, 0.0f}}, {{0.0f, 1.0f, 1.0f, 0.0f}}, {{0.0f, -1.0f, -1.0f, 0.0f}}, {{0.0f, 1.0f, -1.0f, 0.0f}}, {{-3.5f, 1.5f, 1.2f, 0.0f}}, {{-3.5f, -1.5f, 1.2f, 0.0f}}, {{-3.5f, 1.8f, 2.6f, 0.0f}}, {{-3.5f, -1.8f, 2.6f, 0.0f}}, {{-3.5f, 1.2f, 1.2f, 0.0f}}, {{-3.5f, -1.2f, 1.2f, 0.0f}}};
