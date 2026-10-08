#include "types.h"
#include "asm.h"
#include "rnc/sdk/libmpeg.h"

/* The callback loads the decoder at +0x40, then passes its +0x4c DMA state. */
typedef struct MpegDecoder MpegDecoder;
extern void suspend_ipu_dma_state(void *state) __asm__("FUN_0012cc20");

void suspend_mpeg_decoder_ipu_dma(struct sceMpeg *mpeg) __asm__("FUN_0012cb30");

void suspend_mpeg_decoder_ipu_dma(struct sceMpeg *mpeg) {
    suspend_ipu_dma_state(&mpeg->sys->ipu_dma_state);
}

/* The MPEG constructor also refers to this callback by its address label. */
extern __typeof__(suspend_mpeg_decoder_ipu_dma) D_0012CB30 __attribute__((alias("FUN_0012cb30")));
extern __typeof__(suspend_mpeg_decoder_ipu_dma) func_0012CB30
    __attribute__((alias("FUN_0012cb30")));
