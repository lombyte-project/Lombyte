#include "types.h"

#include "rnc/rendering/dma_tag.h"
struct GifPaging {
    struct DmaTag *start;
    struct DmaTag *end;
};
extern struct GifPaging D_0015F450;
extern s32 D_0018A2DC[];
extern void FUN_0020b4a8(void);
extern void vu1_tex_flush(void) __asm__("func_00233B68");

void do_gif_paging(void) __asm__("FUN_001f4398");

void do_gif_paging(void) {
    struct DmaTag *tag;

    tag = render_packet_cursor.tag;
    D_0015F450.end = tag;
    tag = tag + 1;
    render_packet_cursor.tag = tag;
    D_0015F450.start->tag = 0x20000000;
    D_0015F450.start->addr = (u32)render_packet_cursor.tag;
    D_0015F450.start->vif0 = 0;
    D_0015F450.start->vif1 = 0;
    if (D_0018A2DC[0] != 0) {
        FUN_0020b4a8();
        vu1_tex_flush();
    }
    render_packet_cursor.tag->tag = 0x20000000;
    render_packet_cursor.tag->addr = (u32)(D_0015F450.start + 1);
    render_packet_cursor.tag->vif0 = 0;
    render_packet_cursor.tag->vif1 = 0;
    render_packet_cursor.tag = render_packet_cursor.tag + 1;
    D_0015F450.end->tag = 0x20000000;
    D_0015F450.end->addr = (u32)render_packet_cursor.tag;
    D_0015F450.end->vif0 = 0;
    D_0015F450.end->vif1 = 0;
}

extern __typeof__(do_gif_paging) func_001F4398 __attribute__((alias("FUN_001f4398")));
