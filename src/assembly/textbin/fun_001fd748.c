#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_001fd748/FUN_001fd748.s", FUN_001fd748);
#else
#include "types.h"

typedef struct {
    s32 x;
    s32 y;
    s32 dx;
    s32 dy;
} Marker;

typedef struct {
    s32 text;
    s32 pad4[2];
} MarkerText;

typedef struct {
    u8 pad0[0x224];
    s32 sel;
} MapState;

extern u8 D_0013DD40[];
extern u8 D_0013DD58[];
extern s32 D_0015ED80;
extern s32 D_0015F438;
extern s32 D_0015F690 __attribute__((sda));
extern MapState D_001A00F0;
extern MarkerText D_001DDD44[];
extern Marker D_001DDE28[];
extern void func_001F4280(s32);
extern void func_001F4398(void);
extern s32 func_001F6250(u8 *, s32);
extern void func_001F6530(s32, s32, u64, u8 *, s32);
extern s32 func_001F96F8(s32);
extern f32 func_001F9B20(f32 *);
extern u8 *func_001FDD10(s32);
extern s32 func_001FF960(s32, s32);
extern void func_001FFC30(s32, s32, s32, s32, s32, s32);
extern void func_00200258(s32, s32, s32, s32, s32, s32, s32, s32);
extern void func_00200C80(s32, s32, s32, s32, u64, s32);
extern void func_00200E08(s32, s32, s32, s32, u64, s32);
extern void func_00233980(s32, u64);

void FUN_001fd748(s32 x0, s32 x1, s32 y0, s32 y1)
{
    f32 v[4];
    s32 i;
    s32 mode;
    s32 w;
    s32 h;
    s32 px;
    s32 py;
    s32 x;
    s32 y;
    s32 bx;
    s32 by;
    s32 d;
    s32 t;
    s32 wid;
    s32 a;
    s32 m;
    u8 *text;
    s32 tex;

    func_001F4280(0);
    func_00233980(0x42, 0x8000000044);
    func_00233980(0x47, 0x4B);
    func_00200E08(0, 0, 0x200, 0x1C0, 0x80000000, 0);
    tex = func_001FF960(0xE99A, 0xE);
    w = (x1 - x0) * 16;
    h = (y1 - y0) * 16;
    func_00200258(tex, 0, 0, w, h, 0, 0, 0x80);
    func_00233980(8, 0);
    func_00200258(func_001FF960(0xE99A, 0xF), 0, 0, w, h, D_0015F438 & 0xFFF, 0, 0x80);
    func_00233980(8, 5);
    for (i = 1; i < 20; i++) {
        if (*(volatile s32 *)&D_001DDE28[i].x == 0) {
            continue;
        }
        mode = 3;
        if (D_0013DD58[i] == 0) {
            mode = 2;
            if (D_0013DD40[i] == 0) {
                mode = 0;
            }
        }
        if (mode == 0) {
            continue;
        }
        px = D_001DDE28[i].x;
        py = D_001DDE28[i].y;
        if (D_0015ED80 != 0) {
            py = py * 0x1C0 / 0x1A0;
        }
        if (mode == 3 || (mode == 2 && D_0015F438 % (func_001F96F8(0x16) + func_001F96F8(8)) < func_001F96F8(0x16))) {
            func_001FFC30(func_001FF960(0xE99A, 0xC), px - 5, py - 5, 10, 10, 0x80);
        }
        if (i == D_001A00F0.sel) {
            x = px + D_001DDE28[i].dx;
            y = py + D_001DDE28[i].dy;
            v[1] = D_001DDE28[i].dy;
            v[0] = D_001DDE28[i].dx;
            d = func_001F9B20(v) * 1000.0f;
            t = d - 8000;
            bx = x + (px - x) * t / d;
            by = y + (py - y) * t / d;
            text = func_001FDD10(D_001DDD44[i].text);
            wid = func_001F6250(text, -1);
            if (D_001DDE28[i].dx <= -1) {
                a = x - wid;
            } else {
                a = x + wid;
            }
            func_00200C80(bx + 1, by + 1, x + 1, y + 1, 0x80000000, 0);
            func_00200C80(x + 1, y + 1, a + 1, y + 1, 0x80000000, 0);
            m = (a < x) ? a : x;
            func_001F6530(m + 1, y - D_0015F690 + 1, 0x80000000, text, -1);
            func_00200C80(bx, by, x, y, 0x80F0F0F0, 0);
            func_00200C80(x, y, a, y, 0x80F0F0F0, 0);
            func_001F6530(m, y - D_0015F690, 0x80F0F0F0, text, -1);
            func_001FFC30(func_001FF960(0xE99A, 0xD), px - 10, py - 10, 20, 20, 0x80);
        }
    }
    func_001F4398();
}
#endif /* NON_MATCHING */
