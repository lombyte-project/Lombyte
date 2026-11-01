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
    u32 status;
    s32 intc_enable_result;

    status = CpuReadStatus();
    status &= 0x10000;
    if (status != EE_INTERRUPT_ENABLE_BIT_CLEAR) {
        DIntr();
    }
    intc_enable_result = _EnableIntc(cause_index);
    CpuSync();
    if (status != EE_INTERRUPT_ENABLE_BIT_CLEAR) {
        EnableInterrupts();
    }
    return intc_enable_result;
}

/* Recovered original symbol name. */
extern __typeof__(enable_intc) EnableIntc __attribute__((alias("FUN_00119090")));
