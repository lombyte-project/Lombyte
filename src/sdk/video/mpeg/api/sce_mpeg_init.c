#include "types.h"
extern s32 DIntr();
extern s32 EnableInterrupts();
void sceMpegInit(void) {
    s32 state;

    state = DIntr();
    *(volatile u32 *)0x1000F590 = *(volatile u32 *)0x1000F520 | 0x10000;
    *(volatile u32 *)0x1000B000 &= 0xFFFFFEFF;
    *(volatile u32 *)0x1000B400 &= 0xFFFFFEFF;
    *(volatile u32 *)0x1000F590 = *(volatile u32 *)0x1000F520 & 0xFFFEFFFF;
    if (state != 0) {
        EnableInterrupts();
    }
    *(volatile u32 *)0x1000B020 = 0;
    *(volatile u32 *)0x1000B420 = 0;
    sceIpuInit();
}
