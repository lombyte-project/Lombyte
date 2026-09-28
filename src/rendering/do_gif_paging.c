#include "types.h"

struct DmaTag { u32 w0; u32 addr; u32 w2; u32 w3; };
struct TagPtr { struct DmaTag *p; };
struct GifPaging { struct DmaTag *start; struct DmaTag *end; };
extern struct TagPtr D_00160F00;
extern struct GifPaging D_0015F450;
extern s32 D_0018A2DC[];
extern void FUN_0020b4a8(void);
extern void func_00233B68(void);

void do_gif_paging(void) __asm__("FUN_001f4398");

void do_gif_paging(void) {
    struct DmaTag *tag;

    tag = D_00160F00.p;
    D_0015F450.end = tag;
    tag = tag + 1;
    D_00160F00.p = tag;
    D_0015F450.start->w0 = 0x20000000;
    D_0015F450.start->addr = (u32)D_00160F00.p;
    D_0015F450.start->w2 = 0;
    D_0015F450.start->w3 = 0;
    if (D_0018A2DC[0] != 0) {
        FUN_0020b4a8();
        func_00233B68();
    }
    D_00160F00.p->w0 = 0x20000000;
    D_00160F00.p->addr = (u32)(D_0015F450.start + 1);
    D_00160F00.p->w2 = 0;
    D_00160F00.p->w3 = 0;
    D_00160F00.p = D_00160F00.p + 1;
    D_0015F450.end->w0 = 0x20000000;
    D_0015F450.end->addr = (u32)D_00160F00.p;
    D_0015F450.end->w2 = 0;
    D_0015F450.end->w3 = 0;
}

extern __typeof__(do_gif_paging) func_001F4398 __attribute__((alias("FUN_001f4398")));
