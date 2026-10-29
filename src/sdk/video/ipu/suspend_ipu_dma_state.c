#include "types.h"
#include "asm.h"

#include "types.h"

extern void SetD4Chcr(s32 value);
extern void SetD3Chcr(s32 value);

void suspend_ipu_dma_state(u32 *pSnapshot) __asm__("FUN_0012cc20");

void suspend_ipu_dma_state(u32 *pSnapshot) {
    SetD4Chcr(1);
    pSnapshot[0] = *(volatile u32 *)0x1000B410;
    pSnapshot[1] = *(volatile u32 *)0x1000B430;
    pSnapshot[2] = *(volatile u32 *)0x1000B420;
    pSnapshot[3] = *(volatile u32 *)0x1000B400;
    while ((*(volatile u32 *)0x10002010 & 0xF0) != 0) {
    }
    SetD3Chcr(0);
    pSnapshot[4] = *(volatile u32 *)0x1000B010;
    pSnapshot[5] = *(volatile u32 *)0x1000B020;
    pSnapshot[6] = *(volatile u32 *)0x1000B000;
    pSnapshot[7] = *(volatile u32 *)0x10002020;
    pSnapshot[8] = *(volatile u32 *)0x10002010;
}
