#include "types.h"
#include "rnc/sdk/interrupts/enable_interrupts.h"

extern s32 DIntr();

void SetD3Chcr(s32 channel_control) {
    DIntr();
    *(volatile u32 *)0x1000F590 = *(volatile u32 *)0x1000F520 | 0x10000;
    *(volatile u32 *)0x1000B000 = channel_control;
    *(volatile u32 *)0x1000F590 = *(volatile u32 *)0x1000F520 & 0xFFFEFFFF;
    EnableInterrupts();
}
