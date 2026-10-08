#include "types.h"
#include "rnc/video/decoder/vi_buf.h"

u32 get_fifo_index(struct ViBuf *fifo, s32 dma_address) __asm__("FUN_0023baf8");

u32 get_fifo_index(struct ViBuf *fifo, s32 dma_address) {
    if (dma_address == (((fifo->n * 0x10) + (s32)fifo->tag + 0x10) & 0x0FFFFFFF)) {
        return 0U;
    }
    return (u32)(dma_address - (s32)fifo->data) >> 0xB;
}
