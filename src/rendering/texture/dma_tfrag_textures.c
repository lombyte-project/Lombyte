#include "types.h"
struct DmaTag { u32 w0; u32 addr; u32 w2; u32 w3; };
struct TexState { u8 pad0[0xC]; s32 count; s32 enabled; };
struct TagPtr { struct DmaTag *p; };
extern struct TagPtr D_00160F00;
extern struct TagPtr D_00160EBC;
extern struct TexState D_0018A2B0;
extern s32 D_0015EE74;
extern s32 D_00160EC4;
extern char D_001E89B0[];
extern void FUN_00234bd8(void);
extern s32 func_00234D48(s32);
extern void func_00233B68(void);
extern void DebugPrint(char *, ...);
void dma_tfrag_textures(void) __asm__("FUN_002331c0");

void dma_tfrag_textures(void) {
    struct DmaTag *tag;
    s32 size;

    tag = D_00160F00.p;
    D_00160F00.p = tag + 1;
    D_00160EBC.p->w0 = 0x20000000;
    D_00160EBC.p->addr = (u32)D_00160F00.p;
    D_00160EBC.p->w2 = 0;
    D_00160EBC.p->w3 = 0;
    if (D_0018A2B0.enabled != 0 && D_0018A2B0.count != 0) {
        FUN_00234bd8();
        size = func_00234D48(D_0015EE74);
        func_00233B68();
        if (size > 0x400000) {
            DebugPrint(D_001E89B0);
        }
        if (D_00160EC4 < size) {
            D_00160EC4 = size;
        }
    }
    D_00160F00.p->w0 = 0x20000000;
    D_00160F00.p->addr = (u32)(D_00160EBC.p + 1);
    D_00160F00.p->w2 = 0;
    D_00160F00.p->w3 = 0;
    D_00160F00.p++;
    tag->w0 = 0x20000000;
    tag->addr = (u32)D_00160F00.p;
    tag->w2 = 0;
    tag->w3 = 0;
}

extern __typeof__(dma_tfrag_textures) func_002331C0 __attribute__((alias("FUN_002331c0")));
