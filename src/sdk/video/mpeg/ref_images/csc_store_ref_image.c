#include "types.h"
#include "eeregs.h"

typedef struct {
    u32 qwc;
    u32 addr;
} CscDmaState;

typedef struct {
    u8    pad0[0xD8];
    u32   source;
    u8    padDC[0x858 - 0xDC];
    void *callback_data;
} CscDecoder;

typedef struct {
    u32 dest;
    u8  pad4[0x8];
    s32 width;
    s32 height;
} CscImage;

extern s32  csc_dma_chunk_handler() __asm__("D_0012A5D8");
extern s32  AddDmacHandlerSecondary(s32 channel, void *handler, s32 next, void *arg);
extern s32  RemoveDmacHandler(s32 channel, s32 handle);
extern s32  DIntr(void);
extern s32  EnableInterrupts(void);
extern s32  _dispatchMpegCallback(void *callback_data, s32 *callback_args);
extern void _doCSC(CscDecoder *decoder, u32 source, s32 macroblocks);
extern void _doCSC2(CscDecoder *decoder, u32 source, s32 macroblocks);
extern void _sendIpuCommand(CscDecoder *decoder, u32 command);
extern s32  disable_dmac(s32 channel) __asm__("func_001190F8");
extern s32  enable_dmac(s32 channel) __asm__("func_00119160");

void _csc_storeRefImage(CscDecoder *decoder, CscImage *image) {
    s32         callback_args[8];
    CscDmaState dma;
    s32         macroblocks;
    s32         few_macroblocks;
    s32         handle;
    s32         interrupts_enabled;

    macroblocks = image->width * image->height;
    callback_args[0] = 2;
    _dispatchMpegCallback(decoder->callback_data, callback_args);
    if (*IPU_CTRL & 0x4000) {
        *IPU_CTRL = 0x40000000;
    }
    few_macroblocks = macroblocks < 0x400;
    while ((s32)*IPU_CTRL < 0) {
    }
    _sendIpuCommand(decoder, 0);
    while ((s32)*IPU_CTRL < 0) {
    }
    dma.qwc = macroblocks * 0x18;
    dma.addr = image->dest & 0x0FFFFFFF;
    if (dma.qwc > 0xFFFF) {
        handle = AddDmacHandlerSecondary(4, csc_dma_chunk_handler, 0, &dma);
        *REG_DMAC_STAT = 0x10;
        enable_dmac(4);
        interrupts_enabled = DIntr();
        *D4_MADR = dma.addr;
        *D4_QWC = 0xFFFF;
        *D4_CHCR = 0x101;
        if (interrupts_enabled) {
            EnableInterrupts();
        }
        dma.addr = (dma.addr + 0xFFFF0) & 0x0FFFFFFF;
        dma.qwc -= 0xFFFF;
        if (few_macroblocks) {
            _doCSC(decoder, decoder->source, macroblocks);
        } else {
            _doCSC2(decoder, decoder->source, macroblocks);
        }
        disable_dmac(4);
        RemoveDmacHandler(4, handle);
    } else {
        interrupts_enabled = DIntr();
        *D4_MADR = image->dest & 0x0FFFFFFF;
        *D4_QWC = dma.qwc;
        *D4_CHCR = 0x101;
        if (interrupts_enabled) {
            EnableInterrupts();
        }
        dma.qwc = 0;
        if (few_macroblocks) {
            _doCSC(decoder, decoder->source, macroblocks);
        } else {
            _doCSC2(decoder, decoder->source, macroblocks);
        }
    }
    callback_args[0] = 3;
    _dispatchMpegCallback(decoder->callback_data, callback_args);
}
