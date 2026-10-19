#include "types.h"
#include "eeregs.h"

typedef struct {
    u32 qwc;
    u32 addr;
} CscDmaState;

typedef struct {
    u8 pad0[0xD8];
    u32 source;
    u8 padDC[0x858 - 0xDC];
    void *cbData;
} CscDecoder;

typedef struct {
    u32 dest;
    u8 pad4[0x8];
    s32 width;
    s32 height;
} CscImage;

extern s32 csc_dma_chunk_handler() __asm__("D_0012A5D8");
extern s32 AddDmacHandlerSecondary(s32 channel, void *handler, s32 next, void *arg);
extern s32 RemoveDmacHandler(s32 channel, s32 handle);
extern s32 DIntr(void);
extern s32 EnableInterrupts(void);
extern s32 _dispatchMpegCallback(void *cb, s32 *arg);
extern void _doCSC(CscDecoder *d, u32 source, s32 mbs);
extern void _doCSC2(CscDecoder *d, u32 source, s32 mbs);
extern void _sendIpuCommand(CscDecoder *d, u32 cmd);
extern s32 disable_dmac(s32 channel) __asm__("func_001190F8");
extern s32 enable_dmac(s32 channel) __asm__("func_00119160");

void _csc_storeRefImage(CscDecoder *d, CscImage *img) {
    s32 cbarg[8];
    CscDmaState dma;
    s32 mbs;
    s32 few_mbs;
    s32 handle;
    s32 intr;

    mbs = img->width * img->height;
    cbarg[0] = 2;
    _dispatchMpegCallback(d->cbData, cbarg);
    if (*IPU_CTRL & 0x4000) {
        *IPU_CTRL = 0x40000000;
    }
    few_mbs = mbs < 0x400;
    while ((s32)*IPU_CTRL < 0) {
    }
    _sendIpuCommand(d, 0);
    while ((s32)*IPU_CTRL < 0) {
    }
    dma.qwc = mbs * 0x18;
    dma.addr = img->dest & 0x0FFFFFFF;
    if (dma.qwc > 0xFFFF) {
        handle = AddDmacHandlerSecondary(4, csc_dma_chunk_handler, 0, &dma);
        *REG_DMAC_STAT = 0x10;
        enable_dmac(4);
        intr = DIntr();
        *D4_MADR = dma.addr;
        *D4_QWC = 0xFFFF;
        *D4_CHCR = 0x101;
        if (intr) {
            EnableInterrupts();
        }
        dma.addr = (dma.addr + 0xFFFF0) & 0x0FFFFFFF;
        dma.qwc -= 0xFFFF;
        if (few_mbs) {
            _doCSC(d, d->source, mbs);
        } else {
            _doCSC2(d, d->source, mbs);
        }
        disable_dmac(4);
        RemoveDmacHandler(4, handle);
    } else {
        intr = DIntr();
        *D4_MADR = img->dest & 0x0FFFFFFF;
        *D4_QWC = dma.qwc;
        *D4_CHCR = 0x101;
        if (intr) {
            EnableInterrupts();
        }
        dma.qwc = 0;
        if (few_mbs) {
            _doCSC(d, d->source, mbs);
        } else {
            _doCSC2(d, d->source, mbs);
        }
    }
    cbarg[0] = 3;
    _dispatchMpegCallback(d->cbData, cbarg);
}
