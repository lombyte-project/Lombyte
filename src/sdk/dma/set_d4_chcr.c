#include "types.h"

extern s32 DIntr();
extern s32 EnableInterrupts();

void SetD4Chcr(s32 channel_control) {
    DIntr();
    *(volatile u32 *)0x1000F590 = *(volatile u32 *)0x1000F520 | 0x10000;
    *(volatile u32 *)0x1000B400 = channel_control;
    *(volatile u32 *)0x1000F590 = *(volatile u32 *)0x1000F520 & 0xFFFEFFFF;
    EnableInterrupts();
}
