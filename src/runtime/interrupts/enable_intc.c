#include "types.h"
#include "kernel.h"

extern s32 _EnableIntc();
extern s32 DIntr();
extern s32 EnableInterrupts();

s32 enable_intc(s32 arg0) __asm__("FUN_00119090");

s32 enable_intc(s32 cause_index) {
    u32 status;
    s32 result;

    status = CpuReadStatus();
    status &= 0x10000;
    if (status != 0) {
        DIntr();
    }
    result = _EnableIntc(cause_index);
    CpuSync();
    if (status != 0) {
        EnableInterrupts();
    }
    return result;
}

/* Recovered original symbol name. */
extern __typeof__(enable_intc) EnableIntc __attribute__((alias("FUN_00119090")));
