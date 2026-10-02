#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
/* Exact SDK/library unit _getRef0; symbolic expected assembly retained pending source recovery. */
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/library/_getRef0/_getRef0.s", _getRef0);
#else
#include "types.h"

/* libmpeg motion compensation: queues the forward/backward reference
   fetch of one macroblock (luma and chroma) for the IPU/DMA stage. */
typedef void (*MCFunc)(void);

typedef struct {
    u8 *addr;
    int x;
    int n0;
    int n1;
    int step;
    u8 *p0;
    u8 *p1;
} RefEnt;

typedef struct {
    u8 *buf;
    int pad4;
    u8 *yref[4];
    u8 *cref[4];
    MCFunc yfunc[4];
    MCFunc cfunc[4];
    RefEnt y[4];
    RefEnt c[4];
    int pad128;
    int n;
    char pad130[0x10];
} MCQueue;

typedef struct {
    char pad0[0x590];
    MCQueue mc[2];
    int cur;
    char pad814[8];
    u8 *work;
} Decoder;

typedef struct {
    u8 *base;
    char pad4[0xC];
    int width;
} Frame;

extern MCFunc D_00132E30[];
extern MCFunc D_00132E50[];

void _getRef0(Decoder *d, Frame *ref, int sfield, int dfield, int yofs, int h,
              int bx, int by, int dx, int dy, int fieldpred, int avg)
{
    RefEnt *ye;
    RefEnt *ce;
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

    work = d->work;
    n = d->mc[d->cur].n;
    ye = &d->mc[d->cur].y[n];
    ce = &d->mc[d->cur].c[n];

    x = (dx >> 1) + bx;
    if (fieldpred) {
        y = (dy >> 1) * 2 + by; y += yofs + sfield; } else { y = (dy >> 1) + by; y += yofs + sfield;
    }
    xm = x >> 4;
    mb = xm * ref->width;
    ym = y >> 4;
    mb += ym;
    xr = x - xm * 16;
    yr = y - ym * 16;
    ye->addr = work + (dfield + yofs) * 32;
    ye->x = xr;
    yh = dy & 1;
    xh = dx & 1;
    if (yh) {
        if (yr + (h << fieldpred) >= 16) {
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
    buf = d->mc[d->cur].buf + n * 0x600;
    ye->step = 16 << fieldpred;
    ye->p0 = buf + yr * 16;
    ye->p1 = buf + yr * 16 + 0x300;
    avg <<= 2;
    yidx = avg | xh << 1 | yh;

    cdx = dx / 2;
    cdy = dy / 2;
    ch = h >> 1;
    cx = (cdx >> 1) + (bx >> 1);
    if (fieldpred) {
        cy = (cdy >> 1) * 2 + (by >> 1);
        cy += (yofs >> 1) + sfield;
    } else {
        cy = (cdy >> 1) + (by >> 1);
        cy += (yofs >> 1) + sfield;
    }
    cxm = cx >> 3;
    cym = cy >> 3;
    cxr = cx - cxm * 8;
    cyr = cy - cym * 8;
    ce->addr = work + (dfield + (yofs >> 1)) * 16 + 0x200;
    ce->x = cxr;
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

    d->mc[d->cur].yfunc[n] = D_00132E30[yidx];
    d->mc[d->cur].cfunc[n] = D_00132E50[avg | (cdx & 1) << 1 | (cdy & 1)];
    d->mc[d->cur].yref[n] = ref->base + mb * 0x180;
    d->mc[d->cur].cref[n] = ref->base + (mb + ref->width) * 0x180;
    d->mc[d->cur].n++;
}
#endif /* NON_MATCHING */
