#include "types.h"
#include "rnc/sdk/kernel.h"
#include "rnc/sdk/interrupts.h"

extern s32 _EnableDmac();
extern s32 DIntr();

s32 enable_dmac(s32 channel_index) __asm__("FUN_00119160");

s32 enable_dmac(s32 channel_index) {
    u32 status;
    s32 dmac_enable_result;

    status = CpuReadStatus();
    status &= 0x10000;
    if (status != 0) {
        DIntr();
    }
    dmac_enable_result = _EnableDmac(channel_index);
    CpuSync();
    if (status != 0) {
        EnableInterrupts();
    }
    return dmac_enable_result;
}

/* Recovered original symbol name. */
extern __typeof__(enable_dmac) EnableDmac __attribute__((alias("FUN_00119160")));
