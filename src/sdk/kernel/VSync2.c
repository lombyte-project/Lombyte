#include "types.h"
#include "kernel.h"

extern s32  DIntr();
extern s32  EnableInterrupts();
extern void SetVSyncFlag(s32 *flag, s64 *result);

s64 VSync2(void) {
    volatile s32 flag;
    volatile s64 gs_csr_snapshot;

    s32 interrupts_were_enabled;

    flag = 0;
    SetVSyncFlag((s32 *)&flag, &gs_csr_snapshot);
    interrupts_were_enabled = DIntr();
    *(volatile u32 *)0x1000F000 = 4;
    CpuSync();
    if (interrupts_were_enabled != 0) {
        EnableInterrupts();
    }
    while ((*(volatile u32 *)0x1000F000 & 4) == 0 && flag == 0) {
    }
    interrupts_were_enabled = DIntr();
    *(volatile u32 *)0x1000F000 = 4;
    CpuSync();
    if (interrupts_were_enabled != 0) {
        EnableInterrupts();
    }
    return gs_csr_snapshot;
}
