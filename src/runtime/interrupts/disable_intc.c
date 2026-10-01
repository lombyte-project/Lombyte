#include "types.h"

extern s32 _DisableIntc();
extern s32 DIntr();
extern s32 EnableInterrupts();

s32 disable_intc(s32 arg0) __asm__("FUN_00119028");

s32 disable_intc(s32 arg0) {
    u32 status;
    s32 result;

    __asm__ __volatile__("mfc0 %0, $12" : "=r"(status));
    status &= 0x10000;
    if (status != 0) {
        DIntr();
    }
    result = _DisableIntc(arg0);
    __asm__ __volatile__("sync");
    if (status != 0) {
        EnableInterrupts();
    }
    return result;
}

/* Recovered original symbol name. */
extern __typeof__(disable_intc) DisableIntc __attribute__((alias("FUN_00119028")));
