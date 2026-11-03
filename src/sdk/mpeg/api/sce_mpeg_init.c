#include "types.h"
#include "rnc/sdk/interrupts.h"

extern s32 DIntr();

void sceMpegInit(void) {
    s32 interrupts_were_enabled;

    interrupts_were_enabled = DIntr();
    *(volatile u32 *)0x1000F590 = *(volatile u32 *)0x1000F520 | 0x10000;
    *(volatile u32 *)0x1000B000 &= 0xFFFFFEFF;
    *(volatile u32 *)0x1000B400 &= 0xFFFFFEFF;
    *(volatile u32 *)0x1000F590 = *(volatile u32 *)0x1000F520 & 0xFFFEFFFF;
    if (interrupts_were_enabled != FALSE) {
        EnableInterrupts();
    }
    *(volatile u32 *)0x1000B020 = 0;
    *(volatile u32 *)0x1000B420 = 0;
    sceIpuInit();
}
