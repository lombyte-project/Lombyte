#include "types.h"
struct DmaTag { u32 w0; u32 addr; u32 w2; u32 w3; };
struct TexState { u8 pad0[0x24]; s32 count; s32 enabled; };
struct TagPtr { struct DmaTag *p; };
extern struct TagPtr D_00160F00;
extern struct TagPtr D_0015FF0C;
extern struct TexState D_0018A2B0;
extern s32 D_0015EE74;
extern void FUN_00211408(s32);
extern void func_00233B68(void);
void dma_moby_textures(void) __asm__("FUN_0020cdf0");

void dma_moby_textures(void) {
    struct DmaTag *tag;

    tag = D_00160F00.p;
    D_00160F00.p = tag + 1;
    D_0015FF0C.p->w0 = 0x20000000;
    D_0015FF0C.p->addr = (u32)D_00160F00.p;
    D_0015FF0C.p->w2 = 0;
    D_0015FF0C.p->w3 = 0;
    if (D_0018A2B0.enabled != 0 && D_0018A2B0.count != 0) {
        FUN_00211408(D_0015EE74);
        func_00233B68();
    }
    D_00160F00.p->w0 = 0x20000000;
    D_00160F00.p->addr = (u32)(D_0015FF0C.p + 1);
    D_00160F00.p->w2 = 0;
    D_00160F00.p->w3 = 0;
    D_00160F00.p++;
    tag->w0 = 0x20000000;
    tag->addr = (u32)D_00160F00.p;
    tag->w2 = 0;
    tag->w3 = 0;
}

extern __typeof__(dma_moby_textures) func_0020CDF0 __attribute__((alias("FUN_0020cdf0")));
