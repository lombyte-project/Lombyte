#include "types.h"
#include "rnc/sdk/libgraph.h"
#include "eetypes.h"
#include "rnc/rendering/dma_tag.h"
extern s32 sceGsSetDefLoadImage(sceGsLoadImage *, s16, s16, s16, s16, s16, s16, s16);
extern void FlushCache(s32);
extern s32 sceGsExecLoadImage(sceGsLoadImage *, u128 *);
void hud_send_texture(u32 data, s32 dbp, s32 psm, s32 wlog, s32 hlog,
                      s32 immediate) __asm__("FUN_00200b10");

void hud_send_texture(u32 data, s32 dbp, s32 psm, s32 wlog, s32 hlog, s32 immediate) {
    sceGsLoadImage local;
    sceGsLoadImage *li;
    struct DmaTag *tag;
    s32 dbw;
    s32 qwc;

    qwc = 1 << (wlog + hlog - 4);
    dbw = (1 << wlog) >> 6;
    if (dbw <= 0) {
        dbw = 1;
    }
    if (immediate == 0) {
        render_packet_cursor.tag->tag = 0x10000006;
        render_packet_cursor.tag->addr = 0;
        render_packet_cursor.tag->vif0 = 0;
        render_packet_cursor.tag->vif1 = 0x50000006;
        tag = render_packet_cursor.tag;
        li = (sceGsLoadImage *)(tag + 1);
        render_packet_cursor.tag = tag + 7;
    } else {
        li = &local;
    }
    sceGsSetDefLoadImage(li, dbp, dbw, psm, 0, 0, 1 << wlog, 1 << hlog);
    if (immediate == 0) {
        render_packet_cursor.tag->tag = qwc | 0x30000000;
        render_packet_cursor.tag->addr = data;
        render_packet_cursor.tag->vif0 = 0;
        render_packet_cursor.tag->vif1 = qwc | 0x50000000;
        render_packet_cursor.tag++;
    } else {
        FlushCache(0);
        sceGsExecLoadImage(li, (u128 *)data);
    }
}

extern __typeof__(hud_send_texture) func_00200B10 __attribute__((alias("FUN_00200b10")));
