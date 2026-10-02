#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00239f58/FUN_00239f58.s", FUN_00239f58);
#else
#include "types.h"

typedef struct {
    f32 h[16][16];
    f32 pad_400[0x20];
    f32 row16[16];
    f32 col16[16];
    f32 pad_500[0x23];
    f32 corner;
    f32 pad_590[0xC];
} HeightLayer;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    u8 pad_C[0x44];
    HeightLayer layers[3];
} HeightTile;

typedef struct {
    u8 pad_0[8];
    f32 ox;
    f32 oy;
    f32 cw;
    f32 ch;
} HeightInfo;

extern HeightTile *D_00161190;
extern HeightInfo D_001E66E0;
extern s32 D_001610E0 __attribute__((sda));

extern s32 func_00239D60(f32, f32);
extern s32 func_001FA6D0(f32);
extern f32 func_001FA6C0(s32);
extern void func_001F9AD8(f32 *, f32 *, f32 *);
extern void func_001F9BF8(f32 *, f32 *, f32);

s32 fun_00239f58(f32 *height, f32 *normal, f32 x, f32 y) __asm__("FUN_00239f58");

s32 fun_00239f58(f32 *height, f32 *normal, f32 x, f32 y) {
    f32 v0[4];
    f32 v1[4];
    HeightTile *t;
    s32 idx;
    s32 c;
    s32 r;
    f32 fx;
    f32 fy;
    f32 h00;
    f32 h10;
    f32 h01;
    f32 h11;
    f32 a;

    idx = func_00239D60(x, y);
    if (idx < 0) {
        return 0;
    }
    t = &D_00161190[idx];
    fx = t->x;
    fy = t->y;
    fx += D_001E66E0.ox;
    fy += D_001E66E0.oy;
    fx = x - fx;
    fy = y - fy;
    c = func_001FA6D0(fx / D_001E66E0.cw);
    r = func_001FA6D0(fy / D_001E66E0.ch);
    fx -= func_001FA6C0(c) * D_001E66E0.cw;
    fx /= D_001E66E0.cw;
    fy -= func_001FA6C0(r) * D_001E66E0.ch;
    fy /= D_001E66E0.ch;
    h00 = t->layers[D_001610E0].h[r][c];
    if (c == 15) {
        h10 = t->layers[D_001610E0].col16[r];
    } else {
        h10 = t->layers[D_001610E0].h[r][c + 1];
    }
    if (r == 15) {
        h01 = t->layers[D_001610E0].row16[c];
    } else {
        h01 = t->layers[D_001610E0].h[r + 1][c];
    }
    if (c == 15) {
        if (r == 15) {
            h11 = t->layers[D_001610E0].corner;
        } else {
            h11 = t->layers[D_001610E0].col16[r + 1];
        }
    } else if (r == 15) {
        h11 = t->layers[D_001610E0].row16[c + 1];
    } else {
        h11 = t->layers[D_001610E0].h[r + 1][c + 1];
    }
    if (height != NULL) {
        a = h00 + (h10 - h00) * fx;
        *height = a + ((h01 + (h11 - h01) * fx) - a) * fy + t->z;
    }
    if (normal != NULL) {
        v0[0] = D_001E66E0.cw;
        v0[1] = 0.0f;
        v0[2] = h10 - h00;
        v0[3] = 1.0f;
        v1[0] = 0.0f;
        v1[1] = D_001E66E0.ch;
        v1[2] = h01 - h00;
        v1[3] = 1.0f;
        func_001F9AD8(normal, v0, v1);
        func_001F9BF8(normal, normal, 1.0f);
    }
    return 1;
}
#endif /* NON_MATCHING */
