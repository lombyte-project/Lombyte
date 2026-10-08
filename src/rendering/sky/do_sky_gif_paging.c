#include "types.h"

#include "rnc/rendering/dma_tag.h"

extern struct TagPtr D_00160470;
extern struct TagPtr D_00160474;
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

    tag = render_packet_cursor.tag;
    D_00160474.p = tag;
    tag = tag + 1;
    render_packet_cursor.tag = tag;
    D_00160470.p->tag = 0x20000000;
    D_00160470.p->addr = (u32)render_packet_cursor.tag;
    D_00160470.p->vif0 = 0;
    D_00160470.p->vif1 = 0;
    if (D_0018A2B4[0] != 0) {
        FUN_0020b4a8();
        vu1_tex_flush();
    }
    render_packet_cursor.tag->tag = 0x20000000;
    render_packet_cursor.tag->addr = (u32)(D_00160470.p + 1);
    render_packet_cursor.tag->vif0 = 0;
    render_packet_cursor.tag->vif1 = 0;
    render_packet_cursor.tag = render_packet_cursor.tag + 1;
    D_00160474.p->tag = 0x20000000;
    D_00160474.p->addr = (u32)render_packet_cursor.tag;
    D_00160474.p->vif0 = 0;
    D_00160474.p->vif1 = 0;
    D_0015EE74.current_page = D_0015EE74.next_page;
}

extern __typeof__(do_sky_gif_paging) func_0022B558 __attribute__((alias("FUN_0022b558")));
