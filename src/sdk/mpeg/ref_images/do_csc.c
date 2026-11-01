#include "types.h"
#include "rnc/sdk/interrupts.h"
#include "rnc/sdk/ipu.h"
#include "rnc/sdk/mpeg/api.h"

typedef struct {
    u8    pad0[0x858];
    void *cbData;
} MpegDec;

extern s32 DIntr(void);

void _doCSC(MpegDec *decoder, u32 ipu_output_address, s32 mbs) {
    s32 cbarg[8];
    s32 intr;

    while ((s32) * (volatile u32 *)0x10002010 < 0) {
    }
    intr = DIntr();
    *(volatile u32 *)0x1000B010 = ipu_output_address & 0x0FFFFFFF;
    *(volatile u32 *)0x1000B020 = mbs << 6;
    *(volatile u32 *)0x1000B000 = 0x100;
    if (intr) {
        EnableInterrupts();
    }
    SendIpuCommand(decoder, 0x70000000 | mbs);
    cbarg[0] = 4;
    _dispatchMpegCallback(decoder->cbData, cbarg);
    while ((*(volatile u32 *)0x1000B000 >> 8) & 1) {
    }
    while ((s32) * (volatile u32 *)0x10002010 < 0) {
    }
}
