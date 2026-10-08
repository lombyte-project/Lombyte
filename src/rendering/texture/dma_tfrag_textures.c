#include "types.h"
#include "rnc/rendering/draw_config.h"
#include "rnc/rendering/dma_tag.h"
#include "rnc/globals.h"
extern struct TagPtr D_00160EBC;
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

    tag = render_packet_cursor.tag;
    render_packet_cursor.tag = tag + 1;
    D_00160EBC.p->tag = 0x20000000;
    D_00160EBC.p->addr = (u32)render_packet_cursor.tag;
    D_00160EBC.p->vif0 = 0;
    D_00160EBC.p->vif1 = 0;
    if (draw_config.tfrag.enabled != 0 && draw_config.tfrag.count != 0) {
        FUN_00234bd8();
        size = func_00234D48(gs_texture_allocation_cursor);
        vu1_tex_flush();
        if (size > 0x400000) {
            DebugPrint(D_001E89B0);
        }
        if (D_00160EC4 < size) {
            D_00160EC4 = size;
        }
    }
    render_packet_cursor.tag->tag = 0x20000000;
    render_packet_cursor.tag->addr = (u32)(D_00160EBC.p + 1);
    render_packet_cursor.tag->vif0 = 0;
    render_packet_cursor.tag->vif1 = 0;
    render_packet_cursor.tag++;
    tag->tag = 0x20000000;
    tag->addr = (u32)render_packet_cursor.tag;
    tag->vif0 = 0;
    tag->vif1 = 0;
}

extern __typeof__(dma_tfrag_textures) func_002331C0 __attribute__((alias("FUN_002331c0")));
