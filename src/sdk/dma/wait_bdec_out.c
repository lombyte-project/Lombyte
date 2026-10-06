#include "types.h"
typedef struct {
    u8 pad0[0x838];
    s32 unk838;
    s32 unk83C;
    u8 pad840[0x18];
    void *cbData;
} MpegDec;

extern char D_00153788[];
extern void _waitIpuIdle(MpegDec *d);
extern void _dispatchMpegCallback(void *cb, s32 *arg);
extern void _Error(MpegDec *d, char *msg);
extern s32 DIntr(void);
extern s32 EnableInterrupts(void);

#define IPU_CTRL  ((volatile u32 *)0x10002010)
#define IPU_BP    ((volatile u32 *)0x10002020)
#define IPU_TOP   ((u64 *)0x10002030)
#define D3_CHCR   ((volatile u32 *)0x1000B400)
#define D3_QWC    ((volatile u32 *)0x1000B420)
#define D4_CHCR   ((volatile u32 *)0x1000B000)
#define D4_QWC    ((volatile u32 *)0x1000B020)
#define D_ENABLER ((volatile u32 *)0x1000F520)
#define D_ENABLEW ((volatile u32 *)0x1000F590)

s32 _waitBdecOut(MpegDec *d) {
    s32 cb1[8];
    s32 cb2[8];
    s32 ret;
    s32 intr;
    u32 bp;
    s64 top;
    u32 rest;

    ret = 1;
    _waitIpuIdle(d);
    if (*D4_QWC != 0 && !(*IPU_CTRL & 0x4000)) {
        do {
            if (*D3_QWC == 0 && !(*D3_CHCR & 0x100)) {
                cb1[0] = 1;
                _dispatchMpegCallback(d->cbData, cb1);
            }
        } while (*D4_QWC != 0 && !(*IPU_CTRL & 0x4000));
    }
    bp = *IPU_BP;
    top = *IPU_TOP;
    d->unk838 = top;
    if (top < 0) {
        rest = bp & 0x1F;
        d->unk83C = rest ? 32 - rest : 0;
    } else {
        d->unk83C = 32;
    }
    if (*IPU_CTRL & 0x4000) {
        _Error(d, D_00153788);
        cb2[0] = 2;
        _dispatchMpegCallback(d->cbData, cb2);
        *(u32 *)0x10002010 = 0x40000000;
        cb2[0] = 3;
        _dispatchMpegCallback(d->cbData, cb2);
        intr = DIntr();
        *D_ENABLEW = *D_ENABLER | 0x10000;
        *D4_CHCR = 0;
        *D_ENABLEW = *D_ENABLER & ~0x10000;
        if (intr) {
            EnableInterrupts();
        }
        ret = 0;
        *D4_QWC = 0;
    }
    return ret;
}
