#include "types.h"
#include "asm.h"

#include "types.h"
typedef struct MpegVideoLibraryContext {
    u8 reserved[0x40];
    void *pDecoderState;
} MpegVideoLibraryContext;

extern void sceIpuRestartDMA(void *ipuDmaSnapshot);

void resume_mpeg_decoder_ipu_dma(MpegVideoLibraryContext *pContext) __asm__("FUN_0012cb40");

void resume_mpeg_decoder_ipu_dma(MpegVideoLibraryContext *pContext) {
    sceIpuRestartDMA((u8 *)pContext->pDecoderState + 0x4c);
}

extern __typeof__(resume_mpeg_decoder_ipu_dma) D_0012CB40 __attribute__((alias("FUN_0012cb40")));
extern __typeof__(resume_mpeg_decoder_ipu_dma) func_0012CB40 __attribute__((alias("FUN_0012cb40")));
