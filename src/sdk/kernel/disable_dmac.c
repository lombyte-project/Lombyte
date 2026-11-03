#include "types.h"
#include "rnc/sdk/kernel.h"
#include "rnc/sdk/interrupts.h"

extern s32 _DisableDmac();
extern s32 DIntr();

s32 disable_dmac(s32 channel_index) __asm__("FUN_001190f8");

s32 disable_dmac(s32 channel_index) {
    enum {
        EE_INTERRUPT_ENABLE_CLEAR = 0
    };
    u32 saved_interrupt_enable_bit;
    s32 dmac_disable_result;

    saved_interrupt_enable_bit = CpuReadStatus();
    saved_interrupt_enable_bit &= 0x10000;
    if (saved_interrupt_enable_bit != EE_INTERRUPT_ENABLE_CLEAR) {
        DIntr();
    }
    dmac_disable_result = _DisableDmac(channel_index);
    CpuSync();
    if (saved_interrupt_enable_bit != EE_INTERRUPT_ENABLE_CLEAR) {
        EnableInterrupts();
    }
    return dmac_disable_result;
}

/* Recovered original symbol name. */
extern __typeof__(disable_dmac) DisableDmac __attribute__((alias("FUN_001190f8")));
