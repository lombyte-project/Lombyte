#include "types.h"
extern u8 D_001E8A08[];
extern u8 D_001E8A20[];
extern u8 D_001E8A38[];
extern s32 DebugPrint();
void print_register_values_and_halt(void) __asm__("FUN_00233f00");

void print_register_values_and_halt(void) {
    volatile u32 *p1 = (volatile u32 *)0x10009000;
    volatile u32 *p2 = (volatile u32 *)0x10009030;
    s32 dma1_chcr_value = *p1;
    s32 dma1_tadr_value = *p2;

    DebugPrint(D_001E8A08);
    DebugPrint(D_001E8A20, dma1_chcr_value, dma1_tadr_value);
    DebugPrint(D_001E8A38);
    for (;;) {
    }
}

extern __typeof__(print_register_values_and_halt) func_00233F00
    __attribute__((alias("FUN_00233f00")));
