#include "types.h"
#include "rnc/sdk/interrupts/enable_interrupts.h"

extern s32 DIntr();

void SetD4ChcrVariant(s32 chcr_value) {
    DIntr();
    *(volatile u32 *)0x1000F590 = *(volatile u32 *)0x1000F520 | 0x10000;
    *(volatile u32 *)0x1000B400 = chcr_value;
    *(volatile u32 *)0x1000F590 = *(volatile u32 *)0x1000F520 & 0xFFFEFFFF;
    EnableInterrupts();
}
