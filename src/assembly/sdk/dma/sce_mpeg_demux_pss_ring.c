#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
/* Exact SDK/library unit sceMpegDemuxPssRing; symbolic expected assembly retained pending source recovery. */
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/dma/sce_mpeg_demux_pss_ring/sceMpegDemuxPssRing.s", sceMpegDemuxPssRing);
#else
#include "types.h"

/* libmpeg: demultiplex the PSS (MPEG-2 program stream) data of a ring
   buffer and hand each PES packet to the callback registered for its
   stream id; returns the number of bytes consumed. */

typedef struct {
    u8 pad0[0x18];
    u64 pos;
    u8 pad20[0x10];
} SysBit;

typedef struct {
    s32 v[6];
} PackHdr;

typedef struct {
    u64 id;
    s32 len;
    s32 scrambling;
    s64 pts;
    s64 dts;
    s32 data;
    s32 datalen;
    s32 header;
    s32 pad2C;
} PesHdr;

typedef struct {
    PackHdr pack;
    PesHdr pes;
} PssHdr;

typedef struct {
    s32 type;
    u8 *header;
    u8 *data;
    u32 len;
    s64 pts;
    s64 dts;
} sceMpegCbDataStr;

typedef s32 (*sceMpegCallback)(void *mp, sceMpegCbDataStr *cbstr, void *data);

typedef struct {
    u64 id;
    u64 mask;
    sceMpegCallback func;
    void *data;
} StreamCb;

typedef struct {
    u8 pad0[0x44];
    StreamCb *tbl;
    s32 n;
} MpegSys;

typedef struct {
    u8 pad0[0x40];
    MpegSys *sys;
} sceMpeg;

extern void _sysbitInit(SysBit *bs, u8 *start, u8 *bufstart, s32 bufsize);
extern s32 SignExtendPackedValue(SysBit *bs, s32 n);
extern u8 *GetSysbitPointer(SysBit *bs, s32 pos);
extern s32 _pack_header(SysBit *bs, PackHdr *pack);
extern s32 _PES_packet(MpegSys *sys, SysBit *bs, PesHdr *pes);

int sceMpegDemuxPssRing(sceMpeg *mp, u8 *start, int size, u8 *bufstart, int bufsize)
{
    SysBit bs;
    PssHdr hdr;
    SysBit *b;
    PssHdr *h;
    sceMpegCbDataStr cb;
    sceMpegCallback cbfunc;
    void *cbdata;
    StreamCb *tbl;
    int ret;
    int i;
    int cont;
    MpegSys *sys;

    cont = 1;
    h = &hdr;
    b = &bs;
    sys = mp->sys;
    cbfunc = 0;
    tbl = sys->tbl;
    _sysbitInit(b, start, bufstart, bufsize);
    cbdata = 0;
    ret = 0;
    i = 0;
    if (i < sys->n) {
        do {
            if (tbl[i].id == 0xBDFF000000) {
                cbdata = tbl[i].data;
                cbfunc = tbl[i].func;
            }
            if (cbfunc != 0) {
                break;
            }
            i++;
        } while (i < sys->n);
    }
    do {
        if (SignExtendPackedValue(b, 32) == 0x1BA) {
            _pack_header(b, &h->pack);
        }
        while (SignExtendPackedValue(b, 24) == 1 && SignExtendPackedValue(b, 32) != 0x1BA &&
               SignExtendPackedValue(b, 32) != 0x1B9 && b->pos < size * 8 && cont) {
            _PES_packet(sys, b, &h->pes);
            if (b->pos > size * 8) {
                continue;
            }
            for (i = 0; i < sys->n; i++) {
                if ((h->pes.id & tbl[i].mask) == tbl[i].id) {
                    cb.type = 6;
                    cb.header = GetSysbitPointer(b, h->pes.header);
                    cb.data = GetSysbitPointer(b, h->pes.data);
                    cb.len = h->pes.datalen;
                    cb.pts = h->pes.pts;
                    cb.dts = h->pes.dts;
                    cont = tbl[i].func(mp, &cb, tbl[i].data);
                    break;
                }
            }
            if (i == sys->n && cbfunc != 0) {
                cb.type = 6;
                cb.header = GetSysbitPointer(b, h->pes.header);
                cb.data = GetSysbitPointer(b, h->pes.data);
                cb.len = h->pes.datalen;
                cb.pts = h->pes.pts;
                cb.dts = h->pes.dts;
                cont = cbfunc(mp, &cb, cbdata);
            }
            if (cont) {
                ret = b->pos >> 3;
            }
        }
    } while (b->pos <= size * 8 && SignExtendPackedValue(b, 32) == 0x1BA);
    return ret;
}
#endif /* NON_MATCHING */
