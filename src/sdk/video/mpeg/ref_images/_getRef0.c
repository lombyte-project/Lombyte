/* Ported from rac1-decomp (src/core/00125630.c, func_00125F88). */
#include "types.h"

#include "types.h"
#include "rnc/sdk/libmpeg.h"

/* libmpeg motion compensation: queues the forward/backward reference
   fetch of one macroblock (luma and chroma) for the IPU/DMA stage. */
extern MpegMcFunc D_00132E30[];
extern MpegMcFunc D_00132E50[];

void _getRef0(struct MpegDecoder  *d,
              struct MpegRefImage *ref,
              int                  sfield,
              int                  dfield,
              int                  yofs,
              int                  h,
              int                  bx,
              int                  by,
              int                  dx,
              int                  dy,
              int                  fieldpred,

              int avg) {
    struct MpegMcFetch *fl;
    struct MpegMcFetch *fc;
    u8                 *mc;
    int                 n, x, y, mbx, mby, ly, lx, v, base, code, mb;
    int                 cmvx, cmvy, ch, cmbx, cmby;
    int                 xh, yh, cyo, o0, o1;
    u8                 *data;

    n = d->mb_buf[d->mb_buf_index].fetch_count;
    mc = d->mc_work;
    fl = &d->mb_buf[d->mb_buf_index].luma[n];
    fc = &d->mb_buf[d->mb_buf_index].chroma[n];

    x = (dx >> 1) + bx;
    if (fieldpred) {
        y = ((dy >> 1) << 1) + yofs + (by + sfield);
    } else {
        y = (dy >> 1) + yofs + (by + sfield);
    }
    mbx = x >> 4;
    mby = y >> 4;
    mb = mbx * ref->mb_height + mby;
    fl->x = x - (mbx << 4);
    fl->addr = mc + ((dfield + yofs) << 5);
    ly = y - (mby << 4);
    yh = dy & 1;
    xh = dx & 1;
    if (yh) {
        if (ly + (h << fieldpred) >= 16) {
            v = (16 >> fieldpred) - (ly >> fieldpred) - 1;
            fl->n0 = v;
            fl->n1 = h - v;
        } else {
            fl->n0 = h;
            fl->n1 = 0;
        }
    } else {
        if (ly + (h << fieldpred) >= 17) {
            v = (16 >> fieldpred) - (ly >> fieldpred);
            fl->n0 = v;
            fl->n1 = h - v;
        } else {
            fl->n0 = h;
            fl->n1 = 0;
        }
    }
    base = d->mb_buf[d->mb_buf_index].spr_base + n * 0x600;
    cmvx = dx / 2;
    o0 = ly << 4;
    fl->p0 = (u8 *)(base + o0);
    o1 = o0 + 0x300;
    fl->p1 = (u8 *)(base + o1);
    fl->step = 16 << fieldpred;
    cmvy = dy / 2;
    code = (avg << 2) | (xh << 1) | yh;

    ch = h >> 1;
    x = (cmvx >> 1) + (bx >> 1);
    cyo = yofs >> 1;
    if (fieldpred) {
        y = ((cmvy >> 1) << 1) + cyo + ((by >> 1) + sfield);
    } else {
        y = (cmvy >> 1) + cyo + ((by >> 1) + sfield);
    }
    cmby = y >> 3;
    cmbx = x >> 3;
    xh = cmvx & 1;
    yh = cmvy & 1;
    fc->x = x - (cmbx << 3);
    fc->addr = mc + (((dfield + cyo) << 4) + 0x200);
    ly = y - (cmby << 3);
    if (yh) {
        if (ly + (ch << fieldpred) >= 8) {
            v = (8 >> fieldpred) - (ly >> fieldpred) - 1;
            fc->n0 = v;
            fc->n1 = ch - v;
        } else {
            fc->n0 = ch;
            fc->n1 = 0;
        }
    } else {
        if (ly + (ch << fieldpred) >= 9) {
            v = (8 >> fieldpred) - (ly >> fieldpred);
            fc->n0 = v;
            fc->n1 = ch - v;
        } else {
            fc->n0 = ch;
            fc->n1 = 0;
        }
    }
    fc->step = 8 << fieldpred;
    d->mb_buf[d->mb_buf_index].luma_func[n] = D_00132E30[code];
    data = ref->data;
    {
        int c0 = (ly << 3) + 0x100;
        int c1 = (ly << 3) + 0x400;
        u8 *dst0 = (u8 *)((((cmbx - mbx) << 1) + (cmby - mby)) * 0x180 + base + c0);
        u8 *dst1 = (u8 *)((((cmbx - mbx) << 1) + (cmby - mby)) * 0x180 + base + c1);
        fc->p1 = dst1;
        fc->p0 = dst0;
    }
    d->mb_buf[d->mb_buf_index].luma_ref[n] = data + mb * 0x180;
    d->mb_buf[d->mb_buf_index].chroma_ref[n] = data + (mb + ref->mb_height) * 0x180;
    d->mb_buf[d->mb_buf_index].chroma_func[n] = D_00132E50[(avg << 2) | (xh << 1) | yh];
    d->mb_buf[d->mb_buf_index].fetch_count++;
}
