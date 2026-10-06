#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
/* Exact SDK/library unit _csc_storeRefImage; symbolic expected assembly retained pending source recovery. */
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/dma/csc_store_ref_image/_csc_storeRefImage.s",
            _csc_storeRefImage);
#else
#include "types.h"

typedef struct {
    u32 count;
    s32 addr;
} CscDma;

typedef struct {
    u8 pad0[0xD8];
    u32 data;
    u8 padDC[0x77C];
    void *cbData;
} CscDec;

typedef struct {
    s32 dest;
    u8 pad4[0x8];
    s32 w;
    s32 h;
} CscImage;

extern u8 D_0012A5D8[];
extern s32 AddDmacHandlerSecondary();
extern s32 DIntr();
extern s32 EnableInterrupts();
extern s32 RemoveDmacHandler();
extern s32 _dispatchMpegCallback();
extern s32 _doCSC();
extern s32 _doCSC2();
extern s32 _sendIpuCommand();
extern s32 disable_dmac() __asm__("func_001190F8");
extern s32 enable_dmac() __asm__("func_00119160");

void _csc_storeRefImage(CscDec *d, CscImage *img) __asm__("_csc_storeRefImage");

void _csc_storeRefImage(CscDec *d, CscImage *img) {
    s32 cbarg[8];
    CscDma dma;
    s32 total;
    s32 small;
    s32 handle;
    s32 intr;

    total = img->w * img->h;
    cbarg[0] = 2;
    _dispatchMpegCallback(d->cbData, cbarg);
    if (*(volatile u32 *)0x10002010 & 0x4000) {
        *(volatile u32 *)0x10002010 = 0x40000000;
    }
    small = total < 0x400;
    while ((s32) * (volatile u32 *)0x10002010 < 0) {
    }
    _sendIpuCommand(d, 0);
    while ((s32) * (volatile u32 *)0x10002010 < 0) {
    }
    dma.count = total * 0x18;
    dma.addr = img->dest & 0x0FFFFFFF;
    if (dma.count > 0xFFFF) {
        /* The handler receives the stack DMA state; the first transfer uses 0xFFFF QWC. */
        handle = AddDmacHandlerSecondary(4, D_0012A5D8, 0, &dma);
        *(volatile u32 *)0x1000E010 = 0x10;
        enable_dmac(4);
        intr = DIntr();
        *(volatile u32 *)0x1000B410 = dma.addr;
        *(volatile u32 *)0x1000B420 = 0xFFFF;
        *(volatile u32 *)0x1000B400 = 0x101;
        if (intr) {
            EnableInterrupts();
        }
        dma.addr = (dma.addr + 0xFFFF0) & 0x0FFFFFFF;
        dma.count = dma.count - 0xFFFF;
        if (small) {
            _doCSC(d, d->data, total);
        } else {
            _doCSC2(d, d->data, total);
        }
        disable_dmac(4);
        RemoveDmacHandler(4, handle);
    } else {
        /* Even the short transfer clears the stack count before running CSC. */
        intr = DIntr();
        *(volatile u32 *)0x1000B410 = img->dest & 0x0FFFFFFF;
        *(volatile u32 *)0x1000B420 = dma.count;
        *(volatile u32 *)0x1000B400 = 0x101;
        if (intr) {
            EnableInterrupts();
        }
        dma.count = 0;
        if (small) {
            _doCSC(d, d->data, total);
        } else {
            _doCSC2(d, d->data, total);
        }
    }
    cbarg[0] = 3;
    _dispatchMpegCallback(d->cbData, cbarg);
}
#endif /* NON_MATCHING */
