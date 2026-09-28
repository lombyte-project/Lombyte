#include "types.h"
#include "eetypes.h"
struct DmaTag { u32 w0; u32 addr; u32 w2; u32 w3; };
struct TagPtr { struct DmaTag *p; };
typedef struct { u128 data[6]; } sceGsLoadImage;
extern struct TagPtr D_00160F00;
extern s32 sceGsSetDefLoadImage(sceGsLoadImage *, s16, s16, s16, s16, s16, s16, s16);
extern void FlushCache(s32);
extern s32 sceGsExecLoadImage(sceGsLoadImage *, u128 *);
void hud_send_texture(u32 data, s32 dbp, s32 psm, s32 wlog, s32 hlog, s32 immediate) __asm__("FUN_00200b10");

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
        D_00160F00.p->w0 = 0x10000006;
        D_00160F00.p->addr = 0;
        D_00160F00.p->w2 = 0;
        D_00160F00.p->w3 = 0x50000006;
        tag = D_00160F00.p;
        li = (sceGsLoadImage *)(tag + 1);
        D_00160F00.p = tag + 7;
    } else {
        li = &local;
    }
    sceGsSetDefLoadImage(li, dbp, dbw, psm, 0, 0, 1 << wlog, 1 << hlog);
    if (immediate == 0) {
        D_00160F00.p->w0 = qwc | 0x30000000;
        D_00160F00.p->addr = data;
        D_00160F00.p->w2 = 0;
        D_00160F00.p->w3 = qwc | 0x50000000;
        D_00160F00.p++;
    } else {
        FlushCache(0);
        sceGsExecLoadImage(li, (u128 *)data);
    }
}

extern __typeof__(hud_send_texture) func_00200B10 __attribute__((alias("FUN_00200b10")));
