#include "types.h"

extern s32 _EnableDmac();
extern s32 DIntr();
extern s32 EnableInterrupts();

s32 enable_dmac(s32 arg0) __asm__("FUN_00119160");

s32 enable_dmac(s32 arg0) {
    u32 status;
    s32 result;

    __asm__ __volatile__("mfc0 %0, $12" : "=r"(status));
    status &= 0x10000;
    if (status != 0) {
        DIntr();
    }
    result = _EnableDmac(arg0);
    __asm__ __volatile__("sync");
    if (status != 0) {
        EnableInterrupts();
    }
    return result;
}

/* Recovered original symbol name. */
extern __typeof__(enable_dmac) EnableDmac __attribute__((alias("FUN_00119160")));
