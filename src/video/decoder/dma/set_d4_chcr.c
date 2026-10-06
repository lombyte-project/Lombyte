#include "types.h"

extern void DIntr(void);
extern void EnableInterrupts(void);

void set_d4_chcr(s32 arg0) __asm__("FUN_0023bbb0");

void set_d4_chcr(s32 arg0) {
    DIntr();
    *(volatile s32 *)0x1000F590 = *(volatile s32 *)0x1000F520 | 0x10000;
    *(volatile s32 *)0x1000B400 = arg0;
    *(volatile s32 *)0x1000F590 = *(volatile s32 *)0x1000F520 & 0xFFFEFFFF;
    EnableInterrupts();
}
