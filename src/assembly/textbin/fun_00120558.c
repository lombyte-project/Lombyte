#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00120558/FUN_00120558.s", FUN_00120558);
#else
#include "../../../include/ee_cop2.h"

/* Provisional recovery of the complete resident polling loop. The original
 * two incoming argument registers are never consumed. All five sources are
 * observed on every pass, in order; the retail routine has no timeout.
 * COP2 access stays in the separately documented low-level EE accessor.
 * The oracle above remains authoritative: no compiler matching is claimed.
 */
s32 FUN_00120558(s32 arg0, s32 arg1) {
    u32 busy;
    u32 initial_busy;
    u32 vif1_dma;
    u32 gif_dma;
    u32 vif1_status;
    u32 vpu_status;
    u32 gif_status;

    (void)arg0;
    (void)arg1;
    do {
        vif1_dma = *(vu32 *)0x10009000;
        initial_busy = 1;
        gif_dma = *(vu32 *)0x1000a000;
        vif1_status = *(vu32 *)0x10003c00;
        busy = initial_busy;
        if (!(vif1_dma & 0x100)) busy = 0;
        if (gif_dma & 0x100) busy |= 2;
        if (vif1_status & 3) busy |= 4;
        vpu_status = ee_read_vpu_stat();
        gif_status = *(vu32 *)0x10003020;
        if (vpu_status & 0x100) busy |= 8;
        if (gif_status & 0xc00) busy |= 16;
    } while (busy != 0);
    return 0;
}
#endif /* NON_MATCHING */
