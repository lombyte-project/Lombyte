#include "types.h"
#include "kernel.h"

extern s32 _DisableIntc();
extern s32 DIntr();
extern s32 EnableInterrupts();

s32 disable_intc(s32 intc_cause) __asm__("FUN_00119028");

s32 disable_intc(s32 intc_cause) {
    u32 status;
    s32 result;

    status = CpuReadStatus();
    status &= 0x10000;
    if (status != 0) {
        DIntr();
    }
    result = _DisableIntc(intc_cause);
    CpuSync();
    if (status != 0) {
        EnableInterrupts();
    }
    return result;
}

/* Recovered original symbol name. */
extern __typeof__(disable_intc) DisableIntc __attribute__((alias("FUN_00119028")));
