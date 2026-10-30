#include "types.h"

struct sceSifDmaData {
    u32 source;
    u32 destination;
    u32 size;
    u32 attr;
};

extern s32  sceSifAllocIopHeap(s32 module_id);
extern s32  sceSifSetDma(struct sceSifDmaData *transfer, s32 priority);
extern s32  sceSifDmaStat(s32 dma_id);
extern s32  sceSifLoadModuleBuffer(s32 iop_address,
                                   s32 arguments_length,
                                   s32 arguments) __asm__("InvokeIopStatus");
extern void invoke_state_initializer(s32 module_id) __asm__("InvokeStateInitializer");

s32 load_irx_module(s32 irx_image, s32 size_bytes) __asm__("FUN_00201520");

s32 load_irx_module(s32 irx_image, s32 size_bytes) {
    struct sceSifDmaData transfer;
    s32                  module_id;
    s32                  dma_id;
    s32                  result;

    module_id = sceSifAllocIopHeap(size_bytes);
    transfer.size = size_bytes;
    transfer.source = irx_image;
    transfer.destination = module_id;
    transfer.attr = 0;
    dma_id = sceSifSetDma(&transfer, 1);
    result = 1;
    if (dma_id != 0) {
        while (sceSifDmaStat(dma_id) >= 0) {
        }
        if (sceSifLoadModuleBuffer(module_id, 0, 0) <= -1) {
            result = 0;
        }
        invoke_state_initializer(module_id);
    }
    return result;
}

extern __typeof__(load_irx_module) func_00201520 __attribute__((alias("FUN_00201520")));
