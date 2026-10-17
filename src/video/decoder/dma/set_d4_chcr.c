#include "types.h"

extern void DIntr(void);
extern void EnableInterrupts(void);

void set_d4_chcr(s32 arg0) __asm__("FUN_0023bbb0");

void set_d4_chcr(s32 control_value) {
    DIntr();
    *(volatile s32 *)0x1000F590 = *(volatile s32 *)0x1000F520 | 0x10000;
    *(volatile s32 *)0x1000B400 = control_value;
    *(volatile s32 *)0x1000F590 = *(volatile s32 *)0x1000F520 & 0xFFFEFFFF;
    EnableInterrupts();
}
