#include "types.h"
#include "kernel.h"

extern s32 DIntr();
extern s32 EnableInterrupts();

void VSync(void) {
    s32 interrupts_were_enabled;

    interrupts_were_enabled = DIntr();
    *(volatile u32 *)0x1000F000 = 4;
    CpuSync();
    if (interrupts_were_enabled != 0) {
        EnableInterrupts();
    }
    while ((*(volatile u32 *)0x1000F000 & 4) == 0) {
    }
    interrupts_were_enabled = DIntr();
    *(volatile u32 *)0x1000F000 = 4;
    CpuSync();
    if (interrupts_were_enabled != 0) {
        EnableInterrupts();
    }
}
