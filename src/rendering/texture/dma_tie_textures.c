#include "types.h"
struct DmaTag { u32 w0; u32 addr; u32 w2; u32 w3; };
struct TexState { u8 pad0[0x14]; s32 count; s32 enabled; };
struct TagPtr { struct DmaTag *p; };
extern struct TagPtr D_00160F00;
extern struct TagPtr D_00160F68;
extern struct TexState D_0018A2B0;
extern s32 D_0015EE74;
extern s32 D_00160F74;
extern char D_001E8A50[];
extern s32 FUN_002370c0(s32);
extern void func_00233B68(void);
extern void DebugPrint(char *, ...);
void dma_tie_textures(void) __asm__("FUN_00235640");

void dma_tie_textures(void) {
    struct DmaTag *tag;
    s32 size;

    tag = D_00160F00.p;
    D_00160F00.p = tag + 1;
    D_00160F68.p->w0 = 0x20000000;
    D_00160F68.p->addr = (u32)D_00160F00.p;
    D_00160F68.p->w2 = 0;
    D_00160F68.p->w3 = 0;
    if (D_0018A2B0.enabled != 0 && D_0018A2B0.count != 0) {
        size = FUN_002370c0(D_0015EE74);
        func_00233B68();
        if (size > 0x400000) {
            DebugPrint(D_001E8A50);
        }
        if (D_00160F74 < size) {
            D_00160F74 = size;
        }
    }
    D_00160F00.p->w0 = 0x20000000;
    D_00160F00.p->addr = (u32)(D_00160F68.p + 1);
    D_00160F00.p->w2 = 0;
    D_00160F00.p->w3 = 0;
    D_00160F00.p++;
    tag->w0 = 0x20000000;
    tag->addr = (u32)D_00160F00.p;
    tag->w2 = 0;
    tag->w3 = 0;
}

extern __typeof__(dma_tie_textures) func_00235640 __attribute__((alias("FUN_00235640")));
