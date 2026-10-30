#include "types.h"

extern u8  dmac_bus_error_message[] __asm__("D_001E8A08");
extern u8  dma1_register_values_format[] __asm__("D_001E8A20");
extern u8  dma_halt_question_message[] __asm__("D_001E8A38");
extern s32 DebugPrint();
void       print_register_values_and_halt(void) __asm__("FUN_00233f00");

void print_register_values_and_halt(void) {
    volatile u32 *dma1_chcr_register = (volatile u32 *)0x10009000;
    volatile u32 *dma1_tadr_register = (volatile u32 *)0x10009030;
    s32           dma1_chcr_value = *dma1_chcr_register;
    s32           dma1_tadr_value = *dma1_tadr_register;

    DebugPrint(dmac_bus_error_message);
    DebugPrint(dma1_register_values_format, dma1_chcr_value, dma1_tadr_value);
    DebugPrint(dma_halt_question_message);
    for (;;) {
    }
}

extern __typeof__(print_register_values_and_halt) func_00233F00
    __attribute__((alias("FUN_00233f00")));
