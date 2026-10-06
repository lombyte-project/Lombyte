#include "types.h"

struct CameraView {
    u8 pad_0[0x140];
    f32 unk140;
    f32 unk144;
    f32 unk148;
};

struct ScreenPoint {
    f32 unk0;
    f32 unk4;
    f32 unk8;
};

struct Locals_001f2070 {
    u8 pad_0[0x30];
    f32 v30;
    f32 v34;
    f32 v38;
    u8 pad_3C[4];
    f32 v40;
    u8 pad_44[0x3C];
    u8 blk80[0x0C];
    f32 v8C;
    f32 v90;
    f32 v94;
    f32 v98;
    f32 v9C;
};

extern struct CameraView D_00186F40;
extern f32 D_0018CF10[];
extern void FUN_001f9a68(void *a, s64 b, f32 c);
extern void FUN_001f9d20(void *a, void *b, void *c);
extern void FUN_001f9fc8(void *a);
extern void FUN_001fa378(void *a, void *b, void *c);
void project_to_screen(struct ScreenPoint *screen_out, s32 world_pos) __asm__("FUN_001f2070");

void project_to_screen(struct ScreenPoint *screen_out, s32 world_pos) {
    struct Locals_001f2070 L;
    f32 scale;
    f32 y;
    f32 x;

    FUN_001f9fc8(&L);
    L.v30 = -D_00186F40.unk140 * 1024.0f;
    L.v34 = -D_00186F40.unk144 * 1024.0f;
    L.v38 = -D_00186F40.unk148 * 1024.0f;
    FUN_001fa378(&L.v40, (u8 *)&D_00186F40 + 0x40, &L);
    FUN_001f9a68(&L.blk80, world_pos, 1024.0f);
    L.v8C = 1.0f;
    FUN_001f9d20(&L.v90, &L.blk80, &L.v40);
    scale = D_0018CF10[0] / L.v9C;
    screen_out->unk8 = L.v98 * 0.0009765625f;
    x = L.v90 * scale + 2048.0f;
    y = L.v94 * scale + 2048.0f;
    L.v90 = x;
    L.v94 = y;
    screen_out->unk0 = x * 16.0f;
    screen_out->unk4 = y * 16.0f;
}

extern __typeof__(project_to_screen) func_001F2070 __attribute__((alias("FUN_001f2070")));
