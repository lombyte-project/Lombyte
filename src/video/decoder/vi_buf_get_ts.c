#include "types.h"
#include "eetypes.h"
#include "eeregs.h"

/* viBufGetTs(ViBuf *, TimeStamp *) */

typedef struct {
    s64 pts;
    s64 dts;
    s32 pos;
    s32 len;
} TimeStamp;
typedef struct {
    s32 d4madr;
    s32 d4tadr;
    s32 d4qwc;
    s32 d4chcr;
    s32 d3madr;
    s32 d3qwc;
    s32 d3chcr;
    s32 ipubp;
    s32 ipuctrl;
} sceIpuDmaEnv;
typedef struct {
    u128 *data;
    u128 *tag;
    s32 n;
    s32 dmaStart;
    s32 dmaN;
    s32 readBytes;
    s32 buffSize;
    sceIpuDmaEnv env;
    s32 sema;
    s32 isActive;
    s64 totalBytes;
    TimeStamp *ts;
    s32 n_ts;
    s32 count_ts;
    s32 wt_ts;
} ViBuf;

#define IPU_BP         ((vu32 *)0x10002020)
#define VIBUF_ELM_SIZE 2048
#define TS_NONE        (-1)
/* Not the obvious spelling: retail's compare is `b < a`. */
#define min(a, b) ((a) > (b) ? (b) : (a))

extern s32 WaitSema(s32);
extern s32 SignalSema(s32);

static inline s32 IsPtsInRegion(s32 tgt, s32 pos, s32 len, s32 size) {
    s32 tgt1 = (tgt + size - pos) % size;
    return tgt1 < len;
}

s32 vi_buf_get_ts(ViBuf *f, TimeStamp *ts) __asm__("FUN_0023c920");

s32 vi_buf_get_ts(ViBuf *f, TimeStamp *ts) {
    u32 d4madr = *D4_MADR;
    u32 ipubp = *IPU_BP;
    s32 bp = f->env.ipubp & 0x7F;
    s32 fp = (ipubp >> 16) & 0x3;
    s32 ifc = (ipubp >> 8) & 0xF;
    u32 d4madr_next = d4madr - ((fp + ifc) << 4);
    u32 stop;
    s32 datasize = VIBUF_ELM_SIZE * f->n;
    s32 isEnd = 0;
    s32 tscount;
    s32 wt;
    s32 i;

    WaitSema(f->sema);

    ts->pts = TS_NONE;
    ts->dts = TS_NONE;

    stop = (d4madr_next + (bp >> 3) + datasize - (u32)f->data) % datasize;

    tscount = f->count_ts;
    wt = f->wt_ts;

    for (i = 0; i < tscount && !isEnd; i++) {
        s32 rd;

        rd = (wt - tscount + f->n_ts + i) % f->n_ts;
        if (IsPtsInRegion(stop, f->ts[rd].pos, f->ts[rd].len, datasize)) {
            ts->pts = f->ts[rd].pts;
            ts->dts = f->ts[rd].dts;
            f->ts[rd].pts = TS_NONE;
            f->ts[rd].dts = TS_NONE;
            isEnd = 1;
            f->count_ts -= min(1, f->count_ts);
        }
    }

    SignalSema(f->sema);

    return 1;
}

extern __typeof__(vi_buf_get_ts) func_0023C920 __attribute__((alias("FUN_0023c920")));
