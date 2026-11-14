#include "types.h"
#include "rnc/sdk/kernel.h"
#include "rnc/sdk/interrupts.h"

extern s32 _EnableIntc();
extern s32 DIntr();

s32 enable_intc(s32 cause_index) __asm__("FUN_00119090");

s32 enable_intc(s32 cause_index) {
    enum {
        EE_INTERRUPT_ENABLE_BIT_CLEAR = 0
    };
    u32 saved_interrupt_enable_bit;
    s32 intc_enable_result;

    saved_interrupt_enable_bit = CpuReadStatus();
    saved_interrupt_enable_bit &= 0x10000;
    if (saved_interrupt_enable_bit != EE_INTERRUPT_ENABLE_BIT_CLEAR) {
        DIntr();
    }
    intc_enable_result = _EnableIntc(cause_index);
    CpuSync();
    if (saved_interrupt_enable_bit != EE_INTERRUPT_ENABLE_BIT_CLEAR) {
        EnableInterrupts();
    }
    return intc_enable_result;
}

/* Recovered original symbol name. */
extern __typeof__(enable_intc) EnableIntc __attribute__((alias("FUN_00119090")));
