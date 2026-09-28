#include "types.h"

struct SifDmaTransfer {
    u32 source;
    u32 destination;
    u32 size;
    u32 attr;
};

extern s32 sceSifAllocIopHeap(s32 module_id);
extern s32 sceSifSetDma(struct SifDmaTransfer *transfer, s32 priority);
extern s32 sceSifDmaStat(s32 dma_id);
extern s32 InvokeIopStatus(s32 module_id, s32 a1, s32 a2);
extern void InvokeStateInitializer(s32 module_id);

s32 load_irx_module(s32 arg0, s32 arg1) __asm__("FUN_00201520");
s32 load_irx_module(s32 arg0, s32 arg1) {
    struct SifDmaTransfer transfer;
    s32 module_id;
    s32 dma_id;
    s32 result;

    module_id = sceSifAllocIopHeap(arg1);
    transfer.size = arg1;
    transfer.source = arg0;
    transfer.destination = module_id;
    transfer.attr = 0;
    dma_id = sceSifSetDma(&transfer, 1);
    result = 1;
    if (dma_id != 0) {
        while (sceSifDmaStat(dma_id) >= 0) {
        }
        if (InvokeIopStatus(module_id, 0, 0) <= -1) {
            result = 0;
        }
        InvokeStateInitializer(module_id);
    }
    return result;
}

extern __typeof__(load_irx_module) func_00201520 __attribute__((alias("FUN_00201520")));
