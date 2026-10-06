#include "types.h"
struct ViBuf {
    s32 unk0;
    s32 unk4;
    s32 unk8;
};

u32 get_fifo_index(struct ViBuf *fifo, s32 dma_address) __asm__("FUN_0023baf8");

u32 get_fifo_index(struct ViBuf *fifo, s32 dma_address) {
    if (dma_address == (((fifo->unk8 * 0x10) + fifo->unk4 + 0x10) & 0x0FFFFFFF)) {
        return 0U;
    }
    return (u32)(dma_address - fifo->unk0) >> 0xB;
}
