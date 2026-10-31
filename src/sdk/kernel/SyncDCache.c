#include "types.h"
#include "rnc/sdk/kernel.h"
#include "rnc/sdk/interrupts.h"

extern s32 _sceSDC();
extern s32 DIntr();

void SyncDCache(s32 range_start_address, s32 range_end_address) __asm__("FUN_00118f88");

void SyncDCache(s32 range_start_address, s32 range_end_address) {
    enum {
        INTERRUPT_ENABLE_BIT_CLEAR = 0
    };
    u32 status;

    status = CpuReadStatus();
    status &= 0x10000;
    if (status != INTERRUPT_ENABLE_BIT_CLEAR) {
        DIntr();
    }
    _sceSDC(range_start_address & 0xFFFFFFC0, range_end_address & 0xFFFFFFC0);
    if (status != INTERRUPT_ENABLE_BIT_CLEAR) {
        EnableInterrupts();
    }
}
