#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
/* Exact SDK/library unit _sceFs_Rcv_Intr; symbolic expected assembly retained pending source recovery. */
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/library/_sceFs_Rcv_Intr/_sceFs_Rcv_Intr.s",
            _sceFs_Rcv_Intr);
#else
#include "types.h"

typedef struct {
    s32 v;
} __attribute__((packed)) Word;

typedef struct {
    Word ret;
    Word func;
    Word dest;
    Word size;
} FsRcvHead;

typedef struct {
    void *dest;
} __attribute__((packed)) FsRcvPtr;

typedef struct {
    Word dest;
    Word size;
} FsRcvBuf;

typedef struct {
    s32 n1;
    s32 n2;
    u8 *d1;
    u8 *d2;
    u8 buf1[0x40];
    u8 buf2[0x40];
} FsRcvRead;

typedef struct {
    u8 b[0x144];
} __attribute__((packed)) FsDirent;

typedef struct {
    u8 b[0x40];
} __attribute__((packed)) FsStat;

extern volatile s32 D_0012FC10[];
extern u32 D_0012FC90[];
extern s32 D_0012FC98[];
extern u8 D_00157500[];

extern s32 iSignalSema(s32);
extern void *memcpy(void *, const void *, u32);

void _sceFs_Rcv_Intr(s32 *arg) {
    FsRcvHead h;
    FsRcvHead *src;
    FsRcvBuf *bs;
    union {
        FsRcvPtr p;
        FsRcvBuf b;
    } u;
    FsRcvRead *r;
    u8 *pkt;
    u8 *base;
    u8 *d;
    u32 *bank = D_0012FC90;
    s32 idx;
    s32 i;
    u32 ret;
    u32 n;
    u32 addr;

    idx = D_0012FC98[0] != 0 ? arg[3] : 0;
    *bank = idx;
    base = D_00157500;
    addr = (u32)base;
    addr += (u32)idx * 0x440;
    pkt = (u8 *)(addr | 0x20000000);
    src = (FsRcvHead *)pkt;
    h.ret = src->ret;
    h.func = src->func;
    h.dest = src->dest;
    h.size = src->size;
    if (h.ret.v >= 0) {
        memcpy((void *)h.dest.v, pkt + 0x10, h.size.v);
    }
    switch (h.func.v) {
    case 2:
        r = (FsRcvRead *)(pkt + 0x14);
        if (r->n1 > 0) {
            d = r->d1;
            for (i = 0; i < r->n1; i++) {
                d[i] = r->buf1[i];
            }
        }
        if (r->n2 > 0) {
            d = r->d2;
            for (i = 0; i < r->n2; i++) {
                d[i] = r->buf2[i];
            }
        }
        break;
    case 11:
        u.p = *(FsRcvPtr *)(pkt + 0x14);
        *(FsDirent *)u.p.dest = *(FsDirent *)(pkt + 0x18);
        break;
    case 12:
        u.p = *(FsRcvPtr *)(pkt + 0x14);
        *(FsStat *)u.p.dest = *(FsStat *)(pkt + 0x18);
        break;
    case 23:
    case 25:
    case 26:
        bs = (FsRcvBuf *)(pkt + 0x14);
        u.b.dest = bs->dest;
        u.b.size = bs->size;
        n = u.b.size.v;
        if (n > 0x400) {
            u.b.size.v = 0x400;
            n = 0x400;
        }
        memcpy((void *)u.b.dest.v, pkt + 0x1C, n);
        break;
    }
    if (h.ret.v < 0) {
        ret = -(u32)h.ret.v;
        for (i = 0; i < 32; i++) {
            if (D_0012FC10[i] == ret) {
                D_0012FC10[i] = -1;
                break;
            }
        }
        return;
    }
    iSignalSema(h.ret.v);
}
#endif /* NON_MATCHING */
