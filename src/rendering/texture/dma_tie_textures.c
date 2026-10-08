#include "types.h"
#include "rnc/rendering/draw_config.h"
#include "rnc/rendering/dma_tag.h"
#include "rnc/globals.h"
extern struct TagPtr D_00160F68;
extern s32 D_00160F74;
extern char D_001E8A50[];
extern s32 FUN_002370c0(s32);
extern void vu1_tex_flush(void) __asm__("func_00233B68");
extern void DebugPrint(char *, ...);
void dma_tie_textures(void) __asm__("FUN_00235640");

void dma_tie_textures(void) {
    struct DmaTag *tag;
    s32 size;

    tag = render_packet_cursor.tag;
    render_packet_cursor.tag = tag + 1;
    D_00160F68.p->tag = 0x20000000;
    D_00160F68.p->addr = (u32)render_packet_cursor.tag;
    D_00160F68.p->vif0 = 0;
    D_00160F68.p->vif1 = 0;
    if (draw_config.tie.enabled != 0 && draw_config.tie.count != 0) {
        size = FUN_002370c0(gs_texture_allocation_cursor);
        vu1_tex_flush();
        if (size > 0x400000) {
            DebugPrint(D_001E8A50);
        }
        if (D_00160F74 < size) {
            D_00160F74 = size;
        }
    }
    render_packet_cursor.tag->tag = 0x20000000;
    render_packet_cursor.tag->addr = (u32)(D_00160F68.p + 1);
    render_packet_cursor.tag->vif0 = 0;
    render_packet_cursor.tag->vif1 = 0;
    render_packet_cursor.tag++;
    tag->tag = 0x20000000;
    tag->addr = (u32)render_packet_cursor.tag;
    tag->vif0 = 0;
    tag->vif1 = 0;
}

extern __typeof__(dma_tie_textures) func_00235640 __attribute__((alias("FUN_00235640")));
