#include "types.h"
#include "rnc/data_labels.h"

extern s32  AddDmacHandler();
extern s32  enable_dmac() __asm__("func_00119160");
extern s32  vif1_dma_interrupt_handler(s32 cause) __asm__("func_00233E00");
extern void print_register_values_and_halt() __asm__("func_00233F00");

void dmac_vif1_enable(void) __asm__("FUN_00233d00");

void dmac_vif1_enable(void) {
    s32 *handler_id_or_dmac_status_ptr;
    s32  masked;

    if (D_00160F18 == 0) {
        handler_id_or_dmac_status_ptr = &D_00160F1C;
        if (*handler_id_or_dmac_status_ptr == 0) {
            masked = *(volatile u32 *)0x1000E010 & 0x20000;
            if (!masked) {
                handler_id_or_dmac_status_ptr = (s32 *)0x1000E010;
                *handler_id_or_dmac_status_ptr = 0x20000;
            }
            D_00160F18 = AddDmacHandler(1, &vif1_dma_interrupt_handler, 0);
            D_00160F1C = AddDmacHandler(0xF, &print_register_values_and_halt, 0);
            enable_dmac(1);
        }
    }
}

extern __typeof__(dmac_vif1_enable) func_00233D00 __attribute__((alias("FUN_00233d00")));
