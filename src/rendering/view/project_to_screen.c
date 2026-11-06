#include "types.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"

struct ScreenPoint {
    f32 x;     /* GS screen x, 1/16 pixel, offset 2048 */
    f32 y;     /* GS screen y, 1/16 pixel, offset 2048 */
    f32 depth; /* view-space depth, world units */
};

struct Locals {
    u8  pad_0[0x30];
    f32 v30;
    f32 v34;
    f32 v38;
    u8  pad_3C[4];
    f32 v40;
    u8  pad_44[0x3C];
    u8  blk80[0x0C];
    f32 v8C;
    f32 v90;
    f32 v94;
    f32 v98;
    f32 v9C;
};

#include "rnc/ui/menus/menu_globals.h"

extern f32 D_0018CF10[];
void       project_to_screen(struct ScreenPoint *screen_out, s32 world_pos) __asm__("FUN_001f2070");

void project_to_screen(struct ScreenPoint *screen_out, s32 world_pos) {
    struct Locals L;
    f32           scale;
    f32           y;
    f32           x;

    set_identity_matrix(&L);
    L.v30 = -game_camera.position.f[0] * 1024.0f;
    L.v34 = -game_camera.position.f[1] * 1024.0f;
    L.v38 = -game_camera.position.f[2] * 1024.0f;
    multiply_matrix(&L.v40, (u8 *)&game_camera + 0x40, &L);
    fast_vec_scale(&L.blk80, world_pos, 1024.0f);
    L.v8C = 1.0f;
    transform_vector(&L.v90, &L.blk80, &L.v40);
    scale = D_0018CF10[0] / L.v9C;
    screen_out->depth = L.v98 * 0.0009765625f;
    x = L.v90 * scale + 2048.0f;
    y = L.v94 * scale + 2048.0f;
    L.v90 = x;
    L.v94 = y;
    screen_out->x = x * 16.0f;
    screen_out->y = y * 16.0f;
}

extern __typeof__(project_to_screen) func_001F2070 __attribute__((alias("FUN_001f2070")));
