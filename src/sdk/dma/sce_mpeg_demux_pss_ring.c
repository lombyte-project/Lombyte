#include "types.h"
#include "asm.h"

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

int sceMpegDemuxPssRing(sceMpeg *mp, u8 *start, int size, u8 *bufstart, int bufsize) {
    SysBit bs;
    PssHdr hdr;
    SysBit *bitstream;
    PssHdr *pss_header;
    sceMpegCbDataStr cb;
    sceMpegCallback cbfunc = 0;
    void *cbdata = 0;
    StreamCb *tbl;
    int consumed_bytes;
    int i;
    int cont;
    MpegSys *sys;
    int headerpos;

    cont = 1;
    pss_header = &hdr;
    sys = mp->sys;
    tbl = sys->tbl;
    _sysbitInit(&bs, start, bufstart, bufsize);
    consumed_bytes = 0;
    i = 0;
    bitstream = &bs;
    if (i < sys->n) {
        do {
            if (tbl[i].id == 0xBDFF000000) {
                cbfunc = tbl[i].func;
                cbdata = tbl[i].data;
            }
            if (cbfunc != 0) {
                break;
            }
            i++;
        } while (i < sys->n);
    }
    for (;;) {
        if (SignExtendPackedValue(bitstream, 32) == 0x1BA) {
            _pack_header(bitstream, &pss_header->pack);
        }
        while (SignExtendPackedValue(bitstream, 24) == 1 && SignExtendPackedValue(bitstream, 32) != 0x1BA &&
               SignExtendPackedValue(bitstream, 32) != 0x1B9 && bitstream->pos < size * 8 && cont) {
            _PES_packet(sys, bitstream, &pss_header->pes);
            if (bitstream->pos > size * 8) {
                continue;
            }
            for (i = 0; i < sys->n; i++) {
                if (tbl[i].id == (pss_header->pes.id & tbl[i].mask)) {
                    headerpos = pss_header->pes.header;
                    cb.type = 6;
                    cb.header = GetSysbitPointer(bitstream, headerpos);
                    cb.data = GetSysbitPointer(bitstream, pss_header->pes.data);
                    cb.len = pss_header->pes.datalen;
                    cb.pts = pss_header->pes.pts;
                    cb.dts = pss_header->pes.dts;
                    cont = tbl[i].func(mp, &cb, tbl[i].data);
                    break;
                }
            }
            if (i == sys->n && cbfunc != 0) {
                headerpos = pss_header->pes.header;
                cb.type = 6;
                cb.header = GetSysbitPointer(bitstream, headerpos);
                cb.data = GetSysbitPointer(bitstream, pss_header->pes.data);
                cb.len = pss_header->pes.datalen;
                cb.pts = pss_header->pes.pts;
                cb.dts = pss_header->pes.dts;
                cont = cbfunc(mp, &cb, cbdata);
            }
            if (cont) {
                consumed_bytes = bitstream->pos >> 3;
            }
        }
        if (bitstream->pos > size * 8 || SignExtendPackedValue(bitstream, 32) != 0x1BA) {
            break;
        }
    }
    return consumed_bytes;
}
