#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
/* Exact SDK/library unit _getRef0; symbolic expected assembly retained pending source recovery. */
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/library/_getRef0/_getRef0.s", _getRef0);
#else
#include "types.h"
#include "rnc/sdk/libmpeg.h"

/* libmpeg motion compensation: queues the forward/backward reference
   fetch of one macroblock (luma and chroma) for the IPU/DMA stage. */
extern MpegMcFunc D_00132E30[];
extern MpegMcFunc D_00132E50[];

void _getRef0(struct MpegDecoder *d, struct MpegRefImage *ref, int sfield, int dfield, int yofs,
              int h, int bx, int by, int dx, int dy, int fieldpred, int avg) {
    struct MpegMcFetch *ye;
    struct MpegMcFetch *ce;
    int n;
    u8 *buf;
    int x, y, xm, ym, xr, yr;
    int cx, cy, cxm, cym, cxr, cyr;
    int cdx, cdy;
    int xh, yh;
    int k;
    int mb;
    int ch;
    u8 *work;
    int yidx;

    work = d->mc_work;
    x = bx + (dx >> 1);
    n = d->mb_buf[d->mb_buf_index].fetch_count;
    ye = &d->mb_buf[d->mb_buf_index].luma[n];
    ce = &d->mb_buf[d->mb_buf_index].chroma[n];
    if (fieldpred) {
        y = (((dy >> 1) * 2) + by) + (sfield + yofs);
    } else {
        y = (dy >> 1) + by;
        y += sfield + yofs;
    }
    xm = x >> 4;
    mb = xm * ref->unk10;
    ym = y >> 4;
    mb += ym;
    xr = x - xm * 16;
    yr = y - ym * 16;
    ye->x = xr;
    ye->addr = work + (dfield + yofs) * 32;
    yh = dy & 1;
    xh = dx & 1;
    if (yh) {
        if (15 < yr + (h << fieldpred)) {
            k = (16 >> fieldpred) - (yr >> fieldpred) - 1;
            ye->n0 = k;
            ye->n1 = h - k;
        } else {
            ye->n0 = h;
            ye->n1 = 0;
        }
    } else {
        if (yr + (h << fieldpred) > 16) {
            k = (16 >> fieldpred) - (yr >> fieldpred);
            ye->n0 = k;
            ye->n1 = h - k;
        } else {
            ye->n0 = h;
            ye->n1 = 0;
        }
    }
    buf = (u8 *)d->mb_buf[d->mb_buf_index].spr_base + n * 0x600;
    ye->p0 = buf + yr * 16;
    ye->p1 = buf + yr * 16 + 0x300;
    ye->step = 16 << fieldpred;
    yidx = (avg << 2) | xh << 1 | yh;
    avg <<= 2;

    cdy = dy / 2;
    cdx = dx / 2;
    cx = (cdx >> 1) + (bx >> 1);
    if (fieldpred) {
        cy = ((cdy >> 1) * 2 + (by >> 1) + (yofs >> 1)) + sfield;
    } else {
        cy = ((cdy >> 1) + (yofs >> 1)) + ((by >> 1) + sfield);
    }
    cxm = cx >> 3;
    cym = cy >> 3;
    cxr = cx - cxm * 8;
    cyr = cy - cym * 8;
    ce->x = cxr;
    ce->addr = work + (dfield + (yofs >> 1)) * 16 + 0x200;
    ch = h >> 1;
    if ((cdy & 1)) {
        if (cyr + (ch << fieldpred) >= 8) {
            k = (8 >> fieldpred) - (cyr >> fieldpred) - 1;
            ce->n0 = k;
            ce->n1 = ch - k;
        } else {
            ce->n0 = ch;
            ce->n1 = 0;
        }
    } else {
        if (cyr + (ch << fieldpred) > 8) {
            k = (8 >> fieldpred) - (cyr >> fieldpred);
            ce->n0 = k;
            ce->n1 = ch - k;
        } else {
            ce->n0 = ch;
            ce->n1 = 0;
        }
    }
    ce->step = 8 << fieldpred;
    buf = buf + ((cxm - xm) * 2 + (cym - ym)) * 0x180;
    ce->p0 = buf + cyr * 8 + 0x100;
    ce->p1 = buf + cyr * 8 + 0x400;

    d->mb_buf[d->mb_buf_index].luma_func[n] = D_00132E30[yidx];
    d->mb_buf[d->mb_buf_index].chroma_func[n] = D_00132E50[avg | (cdx & 1) << 1 | (cdy & 1)];
    d->mb_buf[d->mb_buf_index].luma_ref[n] = ref->data + mb * 0x180;
    d->mb_buf[d->mb_buf_index].chroma_ref[n] = ref->data + (mb + ref->unk10) * 0x180;
    d->mb_buf[d->mb_buf_index].fetch_count++;
}
#endif /* NON_MATCHING */
