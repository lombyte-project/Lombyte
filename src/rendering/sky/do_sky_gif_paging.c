#include "types.h"

#include "rnc/rendering/dma_tag.h"

struct DmaTagCursor {
    struct DmaTag *tag;
};
extern struct DmaTagCursor D_00160F00;
extern struct DmaTagCursor D_00160470;
extern struct DmaTagCursor D_00160474;
extern s32 D_0018A2B4[];
struct SkyPagingState {
    s32 current_page;
    s32 next_page;
};
extern struct SkyPagingState D_0015EE74;
extern void FUN_0020b4a8(void);
extern void vu1_tex_flush(void) __asm__("func_00233B68");

void do_sky_gif_paging(void) __asm__("FUN_0022b558");

void do_sky_gif_paging(void) {
    struct DmaTag *tag;

    tag = D_00160F00.tag;
    D_00160474.tag = tag;
    tag = tag + 1;
    D_00160F00.tag = tag;
    D_00160470.tag->tag = 0x20000000;
    D_00160470.tag->addr = (u32)D_00160F00.tag;
    D_00160470.tag->vif0 = 0;
    D_00160470.tag->vif1 = 0;
    if (D_0018A2B4[0] != 0) {
        FUN_0020b4a8();
        vu1_tex_flush();
    }
    D_00160F00.tag->tag = 0x20000000;
    D_00160F00.tag->addr = (u32)(D_00160470.tag + 1);
    D_00160F00.tag->vif0 = 0;
    D_00160F00.tag->vif1 = 0;
    D_00160F00.tag = D_00160F00.tag + 1;
    D_00160474.tag->tag = 0x20000000;
    D_00160474.tag->addr = (u32)D_00160F00.tag;
    D_00160474.tag->vif0 = 0;
    D_00160474.tag->vif1 = 0;
    D_0015EE74.current_page = D_0015EE74.next_page;
}

extern __typeof__(do_sky_gif_paging) func_0022B558 __attribute__((alias("FUN_0022b558")));
