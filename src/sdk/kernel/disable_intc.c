#include "types.h"
#include "rnc/sdk/kernel.h"
#include "rnc/sdk/interrupts.h"

extern s32 _DisableIntc();
extern s32 DIntr();

s32 disable_intc(s32 intc_cause) __asm__("FUN_00119028");

s32 disable_intc(s32 intc_cause) {
    enum {
        EE_INTERRUPT_ENABLE_CLEAR = 0
    };
    u32 saved_interrupt_enable_bit;
    s32 result;

    saved_interrupt_enable_bit = CpuReadStatus();
    saved_interrupt_enable_bit &= 0x10000;
    if (saved_interrupt_enable_bit != EE_INTERRUPT_ENABLE_CLEAR) {
        DIntr();
    }
    result = _DisableIntc(intc_cause);
    CpuSync();
    if (saved_interrupt_enable_bit != EE_INTERRUPT_ENABLE_CLEAR) {
        EnableInterrupts();
    }
    return result;
}

/* Recovered original symbol name. */
extern __typeof__(disable_intc) DisableIntc __attribute__((alias("FUN_00119028")));
