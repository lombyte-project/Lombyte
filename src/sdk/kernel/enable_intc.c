#include "types.h"
#include "kernel.h"

extern s32 _EnableIntc();
extern s32 DIntr();
extern s32 EnableInterrupts();

s32 enable_intc(s32 cause_index) __asm__("FUN_00119090");

s32 enable_intc(s32 cause_index) {
    u32 status;
    s32 intc_enable_result;

    status = CpuReadStatus();
    status &= 0x10000;
    if (status != 0) {
        DIntr();
    }
    intc_enable_result = _EnableIntc(cause_index);
    CpuSync();
    if (status != 0) {
        EnableInterrupts();
    }
    return intc_enable_result;
}

/* Recovered original symbol name. */
extern __typeof__(enable_intc) EnableIntc __attribute__((alias("FUN_00119090")));
