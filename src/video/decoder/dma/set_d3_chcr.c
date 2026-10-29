#include "types.h"

extern void DIntr(void);
extern void EnableInterrupts(void);

void set_d3_chcr(s32 chcr_value) __asm__("FUN_0023bb40");

void set_d3_chcr(s32 chcr_value) {
    DIntr();
    *(volatile s32 *)0x1000F590 = *(volatile s32 *)0x1000F520 | 0x10000;
    *(volatile s32 *)0x1000B000 = chcr_value;
    *(volatile s32 *)0x1000F590 = *(volatile s32 *)0x1000F520 & 0xFFFEFFFF;
    EnableInterrupts();
}
