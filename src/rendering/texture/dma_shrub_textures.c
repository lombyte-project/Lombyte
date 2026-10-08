#include "types.h"
#include "rnc/rendering/draw_config.h"
#include "rnc/rendering/dma_tag.h"
#include "rnc/globals.h"
extern struct TagPtr D_001603F0;
extern s32 D_001603F8;
extern char D_001E88D0[];
extern s32 FUN_0022a330(s32);
extern void vu1_tex_flush(void) __asm__("func_00233B68");
extern void DebugPrint(char *, ...);
void dma_shrub_textures(void) __asm__("FUN_002288f0");

void dma_shrub_textures(void) {
    struct DmaTag *tag;
    s32 size;

    tag = render_packet_cursor.tag;
    render_packet_cursor.tag = tag + 1;
    D_001603F0.p->tag = 0x20000000;
    D_001603F0.p->addr = (u32)render_packet_cursor.tag;
    D_001603F0.p->vif0 = 0;
    D_001603F0.p->vif1 = 0;
    if (draw_config.shrub.enabled != 0 && draw_config.shrub.count != 0) {
        size = FUN_0022a330(gs_texture_allocation_cursor);
        vu1_tex_flush();
        if (size > 0x400000) {
            DebugPrint(D_001E88D0);
        }
        if (D_001603F8 < size) {
            D_001603F8 = size;
        }
    }
    render_packet_cursor.tag->tag = 0x20000000;
    render_packet_cursor.tag->addr = (u32)(D_001603F0.p + 1);
    render_packet_cursor.tag->vif0 = 0;
    render_packet_cursor.tag->vif1 = 0;
    render_packet_cursor.tag++;
    tag->tag = 0x20000000;
    tag->addr = (u32)render_packet_cursor.tag;
    tag->vif0 = 0;
    tag->vif1 = 0;
}

extern __typeof__(dma_shrub_textures) func_002288F0 __attribute__((alias("FUN_002288f0")));
