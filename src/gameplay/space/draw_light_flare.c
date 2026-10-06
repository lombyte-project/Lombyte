/* Ported from rac1-decomp (src/game/space.c, func_0022F258). */
#include "sda.h"
extern void copy_blocks_16_forward(void *, void *, int) __asm__("FUN_001f98d0");
extern int get_effect_texture(int) __asm__("FUN_001f44b8");
extern void scale_vector_xyz(void *, void *, float) __asm__("FUN_001f9a68");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void FUN_001f9cf8(void *, void *, void *);
typedef struct {
    float positions[4][4];          /* 0x00 */
    int colors[4];                  /* 0x40 */
    char texture_coordinates[0x20]; /* 0x50 */
    long gs_state[4];               /* 0x70 */
} FlareQuad;
extern char D_001D97B0[];
extern void draw_geometry_quad(void *, int, int) __asm__("func_001F7D30");
extern int D_0015F604 MACRO_ADDR;
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern void FUN_001f9a80(void *, void *, float);
extern int FUN_001fa820(void *, int *, float);
extern float probe_ground_height(void *, int, float) __asm__("func_00213508");
typedef struct {
    char pad[0x20];
    int mode;
    short elapsed_ticks;
    short flare_variant;
} FlareConfiguration;
extern FlareConfiguration D_0013E030;
extern float D_001D97D0[][4][4];
/* Draws a light flare quad at arg0's position (+0x00; the matrix is at
   +0xC0) when FUN_001fa820 finds it on screen, with the alpha it
   returns. In D_0015F604 mode 6 with flare set 3, the flare fades as
   D_0013E030's level passes FUN_001f96f8(150), and its depth comes from
   func_00213508. The corners are D_001D97D0[set]. */
void draw_light_flare(char *object) __asm__("FUN_0022df40");

void draw_light_flare(char *object) {
    float scaled_position[4];
    FlareQuad quad;
    float scaled_position_copy[4];
    int alpha;
    float depth;
    int corner_index;

    alpha = 0;
    FUN_001f9a80(scaled_position, object, 1.0f / 1024.0f);
    if (FUN_001fa820(scaled_position, &alpha, 32.0f) < 0) {
        return;
    }
    if (D_0015F604 == 6 && D_0013E030.mode == 3 &&
        D_0013E030.elapsed_ticks > scale_game_frames(150)) {
        alpha -= (D_0013E030.elapsed_ticks - scale_game_frames(150)) * 4;
        if (alpha <= 0) {
            return;
        }
    }
    quad.gs_state[1] = get_effect_texture(0);
    quad.gs_state[2] = 0xFF9000000260;
    quad.gs_state[3] = 0x8000000044;
    quad.gs_state[0] = 0;
    copy_blocks_16_forward(quad.texture_coordinates, D_001D97B0, 0x20);
    scale_vector_xyz(scaled_position_copy, object, 1.0f / 1024.0f);
    depth = *(float *)(object + 0x18) + 0.1f;
    if (D_0015F604 == 6) {
        depth = probe_ground_height(object + 0x10, 0, 0.5f) + 0.1f;
    }
    for (corner_index = 0; corner_index < 4; corner_index++) {
        float *vertex_position;

        quad.colors[corner_index] = ((alpha >> 1) << 24) | 0x808080;
        vertex_position = quad.positions[corner_index];
        FUN_001f9cf8(vertex_position, D_001D97D0[D_0013E030.flare_variant][corner_index],
                     object + 0xC0);
        add_vector_xyz(vertex_position, vertex_position, object + 0x10);
        vertex_position[2] = depth;
    }
    draw_geometry_quad(&quad, 0, 0);
}

extern __typeof__(draw_light_flare) func_0022DF40 __attribute__((alias("FUN_0022df40")));
