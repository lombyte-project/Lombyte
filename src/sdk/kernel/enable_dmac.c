#include "types.h"
#include "rnc/sdk/kernel.h"
#include "rnc/sdk/interrupts.h"

extern s32 _EnableDmac();
extern s32 DIntr();

s32 enable_dmac(s32 channel_index) __asm__("FUN_00119160");

s32 enable_dmac(s32 channel_index) {
    enum {
        INTERRUPT_ENABLE_BIT_CLEAR = 0
    };
    u32 saved_interrupt_enable_bit;
    s32 dmac_enable_result;

    saved_interrupt_enable_bit = CpuReadStatus();
    saved_interrupt_enable_bit &= 0x10000;
    if (saved_interrupt_enable_bit != INTERRUPT_ENABLE_BIT_CLEAR) {
        DIntr();
    }
    dmac_enable_result = _EnableDmac(channel_index);
    CpuSync();
    if (saved_interrupt_enable_bit != INTERRUPT_ENABLE_BIT_CLEAR) {
        EnableInterrupts();
    }
    return dmac_enable_result;
}

/* Recovered original symbol name. */
extern __typeof__(enable_dmac) EnableDmac __attribute__((alias("FUN_00119160")));
