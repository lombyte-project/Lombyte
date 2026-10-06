#include "types.h"
#include "rnc/rendering/dma_tag.h"
struct TexState {
    u8 pad0[0xC];
    s32 count;
    s32 enabled;
};
extern struct TagPtr D_00160F00;
extern struct TagPtr D_00160EBC;
extern struct TexState D_0018A2B0;
extern s32 D_0015EE74;
extern s32 D_00160EC4;
extern char D_001E89B0[];
extern void FUN_00234bd8(void);
extern s32 func_00234D48(s32);
extern void vu1_tex_flush(void) __asm__("func_00233B68");
extern void DebugPrint(char *, ...);
void dma_tfrag_textures(void) __asm__("FUN_002331c0");

void dma_tfrag_textures(void) {
    struct DmaTag *tag;
    s32 size;

    tag = D_00160F00.p;
    D_00160F00.p = tag + 1;
    D_00160EBC.p->tag = 0x20000000;
    D_00160EBC.p->addr = (u32)D_00160F00.p;
    D_00160EBC.p->vif0 = 0;
    D_00160EBC.p->vif1 = 0;
    if (D_0018A2B0.enabled != 0 && D_0018A2B0.count != 0) {
        FUN_00234bd8();
        size = func_00234D48(D_0015EE74);
        vu1_tex_flush();
        if (size > 0x400000) {
            DebugPrint(D_001E89B0);
        }
        if (D_00160EC4 < size) {
            D_00160EC4 = size;
        }
    }
    D_00160F00.p->tag = 0x20000000;
    D_00160F00.p->addr = (u32)(D_00160EBC.p + 1);
    D_00160F00.p->vif0 = 0;
    D_00160F00.p->vif1 = 0;
    D_00160F00.p++;
    tag->tag = 0x20000000;
    tag->addr = (u32)D_00160F00.p;
    tag->vif0 = 0;
    tag->vif1 = 0;
}

extern __typeof__(dma_tfrag_textures) func_002331C0 __attribute__((alias("FUN_002331c0")));
