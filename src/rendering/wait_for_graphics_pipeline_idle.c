#include "types.h"
#include "asm.h"

#include "../../include/ee_cop2.h"

/* Spins until VIF1 and GIF DMA, VIF1, VU1 and the GIF are all idle. The original
 * two incoming argument registers are never consumed. All five sources are
 * observed on every pass, in order; the retail routine has no timeout.
 * COP2 access stays in the separately documented low-level EE accessor.
 */
s32 wait_for_graphics_pipeline_idle(s32 arg0, s32 arg1) __asm__("FUN_00120558");

s32 wait_for_graphics_pipeline_idle(s32 arg0, s32 arg1) {
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
        if ((vif1_dma & 0x100) != 0)
            busy = 1;
        else
            busy = 0;
        if (gif_dma & 0x100)
            busy |= 2;
        if (vif1_status & 3)
            busy |= 4;
        vpu_status = ee_read_vpu_stat() & 0x100;
        if (vpu_status)
            busy |= 8;
        gif_status = *(vu32 *)0x10003020;
        if (gif_status & 0xc00)
            busy |= 16;
    } while (busy != 0);
    return 0;
}

extern __typeof__(wait_for_graphics_pipeline_idle) func_00120558
    __attribute__((alias("FUN_00120558")));
