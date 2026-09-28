#include "types.h"

struct DmaTag {
    u32 w0;
    u32 addr;
    u32 w2;
    u32 w3;
};

struct TagPtr {
    struct DmaTag *p;
};

extern struct TagPtr D_00160F00;
extern void func_00233938(s32);
extern void func_00233980(s32, u64);

void draw_framebuffer_rect(s32 x0, s32 y0, s32 x1, s32 y1, s32 ox, s32 oy, u32 color) __asm__("FUN_001fb8f0");

void draw_framebuffer_rect(s32 x0, s32 y0, s32 x1, s32 y1, s32 ox, s32 oy, u32 color) {
    struct DmaTag *tag;
    u64 *q;
    s32 ax;
    s32 ay;
    s32 bx;
    s32 by;

    func_00233938(0x13000000);
    func_00233980(0x42, 0x64);
    D_00160F00.p->w0 = 0x10000006;
    D_00160F00.p->addr = 0;
    D_00160F00.p->w2 = 0;
    D_00160F00.p->w3 = 0x50000006;
    ay = y0 << 4;
    by = y1 << 4;
    ax = x0 << 4;
    bx = x1 << 4;
    by += 0x8000;
    ay += 0x8000;
    bx += 0x8000;
    ax += 0x8000;
    by -= oy << 3;
    ay -= oy << 3;
    bx -= ox << 3;
    ax -= ox << 3;
    tag = D_00160F00.p;
    q = (u64 *)(tag + 1);
    D_00160F00.p = tag + 1;
    q[0] = 0x1000000000000001;
    q[1] = 0xE;
    q[2] = 0x33003;
    q[3] = 0x47;
    q[4] = 0x2400000000000001;
    q[5] = 0x10;
    q[6] = 0x106;
    q[7] = color;
    q[8] = 0x2400000000008001;
    q[9] = 0x44;
    q[10] = ax | ((u64)ay << 16);
    q[11] = bx | ((u64)by << 16);
    D_00160F00.p = (struct DmaTag *)((u8 *)D_00160F00.p + 0x60);
    func_00233980(0x42, 0x8000000044ULL);
    func_00233938(0x13000000);
}

extern __typeof__(draw_framebuffer_rect) func_001FB8F0 __attribute__((alias("FUN_001fb8f0")));
