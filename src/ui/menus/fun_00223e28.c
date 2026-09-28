#include "types.h"

struct Flash {
    u8 pad0[0x48];
    s32 active;
    s32 time;
    s32 x;
    s32 y;
    s32 w;
    s32 h;
};

struct Owner {
    u8 pad0[0x78];
    struct Flash *flash;
};

extern s32 D_0015ED80;
extern void func_00233980(s32, u64);
extern s32 func_001FF960(s32, s32);
extern void func_00200080(s32, s32, s32, s32, s32, s32);
extern s32 func_00213260(s32);
extern s32 SubtractIntegerWithClamp(s32);
extern s64 func_001F44B8(s32);
extern void func_001F5450(s32, s32, s32, s32, s32, s32, s32, s32, s64, s64);

void FUN_00223e28(struct Owner *o) {
    struct Flash *f;
    s32 x;
    s32 y;
    s32 w;
    s32 h;
    s32 u;
    s32 v;
    s32 a;
    s32 dx;
    s32 dy;

    if (o == 0) {
        return;
    }
    f = o->flash;
    if (f == 0) {
        return;
    }
    x = f->x;
    y = f->y;
    w = f->w;
    h = f->h;
    if (x >= 0x200 || x + w < 0) {
        return;
    }
    if (!(D_0015ED80 != 0 ? y < 0x1C1 : y < 0x1A1) || y + h < 0) {
        return;
    }
    func_00233980(8, 0);
    func_00233980(0x42, 0x8000000044ULL);
    func_00200080(func_001FF960(0xE99E, 7), x << 4, y << 4, w << 4, h << 4, 0x80);
    if (f->active) {
        f->time += 2;
        u = func_00213260(200);
        v = func_00213260(200);
        a = 0x80 - SubtractIntegerWithClamp(f->time - 0x80);
        func_00233980(8, 0);
        a = a * 2;
        func_00233980(0x42, ((u64)(a > 0x80 ? 0x80 : a) << 32) | 0x68);
        func_001F5450(x, y, w, h, u, v, w, h, 0x808080, func_001F44B8(0x1A));
        if (f->time >= 0x100) {
            f->active = 0;
        }
    } else if (func_00213260(2000) == 0) {
        f->time = 0;
        f->active = 1;
    }
    func_00233980(8, 0);
    func_00233980(0x42, 0x8000000044ULL);
    dx = -2;
    dy = -2;
    func_001F5450(x, y, w, h, 0, 0, w, (h * 3) >> 1, 0x50606060, func_001F44B8(0x1C));
    if (w >= 0x4C) {
        dx = -1;
    }
    if (h >= 0x4C) {
        dy = -1;
    }
    if (w >= 0x97) {
        dx = 0;
    }
    if (h >= 0x97) {
        dy = 0;
    }
    func_001F5450(x + 1, y + 1, w + dx, h + dy, 1, 1, 0x3E, 0x3E, 0x80808080, func_001F44B8(0x19));
}

extern __typeof__(FUN_00223e28) func_00223E28 __attribute__((alias("FUN_00223e28")));
