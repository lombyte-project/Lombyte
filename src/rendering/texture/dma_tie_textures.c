#include "types.h"
#include "rnc/rendering/dma_tag.h"
struct TexState {
    u8 pad0[0x14];
    s32 count;
    s32 enabled;
};
extern struct TagPtr D_00160F00;
extern struct TagPtr D_00160F68;
extern struct TexState D_0018A2B0;
extern s32 D_0015EE74;
extern s32 D_00160F74;
extern char D_001E8A50[];
extern s32 FUN_002370c0(s32);
extern void vu1_tex_flush(void) __asm__("func_00233B68");
extern void DebugPrint(char *, ...);
void dma_tie_textures(void) __asm__("FUN_00235640");

void dma_tie_textures(void) {
    struct DmaTag *tag;
    s32 size;

    tag = D_00160F00.p;
    D_00160F00.p = tag + 1;
    D_00160F68.p->tag = 0x20000000;
    D_00160F68.p->addr = (u32)D_00160F00.p;
    D_00160F68.p->vif0 = 0;
    D_00160F68.p->vif1 = 0;
    if (D_0018A2B0.enabled != 0 && D_0018A2B0.count != 0) {
        size = FUN_002370c0(D_0015EE74);
        vu1_tex_flush();
        if (size > 0x400000) {
            DebugPrint(D_001E8A50);
        }
        if (D_00160F74 < size) {
            D_00160F74 = size;
        }
    }
    D_00160F00.p->tag = 0x20000000;
    D_00160F00.p->addr = (u32)(D_00160F68.p + 1);
    D_00160F00.p->vif0 = 0;
    D_00160F00.p->vif1 = 0;
    D_00160F00.p++;
    tag->tag = 0x20000000;
    tag->addr = (u32)D_00160F00.p;
    tag->vif0 = 0;
    tag->vif1 = 0;
}

extern __typeof__(dma_tie_textures) func_00235640 __attribute__((alias("FUN_00235640")));
