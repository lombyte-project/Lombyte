#include "types.h"
#include "kernel.h"

extern s32 _DisableDmac();
extern s32 DIntr();
extern s32 EnableInterrupts();

s32 disable_dmac(s32 channel_index) __asm__("FUN_001190f8");

s32 disable_dmac(s32 channel_index) {
    u32 status;
    s32 dmac_disable_result;

    status = CpuReadStatus();
    status &= 0x10000;
    if (status != 0) {
        DIntr();
    }
    dmac_disable_result = _DisableDmac(channel_index);
    CpuSync();
    if (status != 0) {
        EnableInterrupts();
    }
    return dmac_disable_result;
}

/* Recovered original symbol name. */
extern __typeof__(disable_dmac) DisableDmac __attribute__((alias("FUN_001190f8")));
