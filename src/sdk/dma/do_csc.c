#include "types.h"
typedef struct {
    u8 pad0[0x858];
    void *cbData;
} MpegDec;

extern s32 DIntr(void);
extern s32 EnableInterrupts(void);
extern void _sendIpuCommand(MpegDec *d, u32 cmd);
extern void _dispatchMpegCallback(void *cb, s32 *arg);

void _doCSC(MpegDec *d, u32 src, s32 mbs) {
    s32 cbarg[8];
    s32 intr;

    while ((s32) * (volatile u32 *)0x10002010 < 0) {
    }
    intr = DIntr();
    *(volatile u32 *)0x1000B010 = src & 0x0FFFFFFF;
    *(volatile u32 *)0x1000B020 = mbs << 6;
    *(volatile u32 *)0x1000B000 = 0x100;
    if (intr) {
        EnableInterrupts();
    }
    _sendIpuCommand(d, 0x70000000 | mbs);
    cbarg[0] = 4;
    _dispatchMpegCallback(d->cbData, cbarg);
    while ((*(volatile u32 *)0x1000B000 >> 8) & 1) {
    }
    while ((s32) * (volatile u32 *)0x10002010 < 0) {
    }
}
