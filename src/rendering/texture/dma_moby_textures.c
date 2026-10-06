#include "types.h"
#include "rnc/rendering/dma_tag.h"
struct TexState {
    u8 pad0[0x24];
    s32 count;
    s32 enabled;
};
extern struct TagPtr D_00160F00;
extern struct TagPtr D_0015FF0C;
extern struct TexState D_0018A2B0;
extern s32 D_0015EE74;
extern void FUN_00211408(s32);
extern void vu1_tex_flush(void) __asm__("func_00233B68");
void dma_moby_textures(void) __asm__("FUN_0020cdf0");

void dma_moby_textures(void) {
    struct DmaTag *tag;

    tag = D_00160F00.p;
    D_00160F00.p = tag + 1;
    D_0015FF0C.p->tag = 0x20000000;
    D_0015FF0C.p->addr = (u32)D_00160F00.p;
    D_0015FF0C.p->vif0 = 0;
    D_0015FF0C.p->vif1 = 0;
    if (D_0018A2B0.enabled != 0 && D_0018A2B0.count != 0) {
        FUN_00211408(D_0015EE74);
        vu1_tex_flush();
    }
    D_00160F00.p->tag = 0x20000000;
    D_00160F00.p->addr = (u32)(D_0015FF0C.p + 1);
    D_00160F00.p->vif0 = 0;
    D_00160F00.p->vif1 = 0;
    D_00160F00.p++;
    tag->tag = 0x20000000;
    tag->addr = (u32)D_00160F00.p;
    tag->vif0 = 0;
    tag->vif1 = 0;
}

extern __typeof__(dma_moby_textures) func_0020CDF0 __attribute__((alias("FUN_0020cdf0")));
