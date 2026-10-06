#include "types.h"
extern s32 D_00160F18;
extern s32 D_00160F1C;
extern s32 AddDmacHandler();
extern s32 enable_dmac() __asm__("func_00119160");
extern void func_00233E00();
extern void print_register_values_and_halt() __asm__("func_00233F00");

void dmac_vif1_enable(void) __asm__("FUN_00233d00");

void dmac_vif1_enable(void) {
    s32 *p;
    s32 masked;

    if (D_00160F18 == 0) {
        p = &D_00160F1C;
        if (*p == 0) {
            masked = *(volatile u32 *)0x1000E010 & 0x20000;
            if (!masked) {
                p = (s32 *)0x1000E010;
                *p = 0x20000;
            }
            D_00160F18 = AddDmacHandler(1, &func_00233E00, 0);
            D_00160F1C = AddDmacHandler(0xF, &print_register_values_and_halt, 0);
            enable_dmac(1);
        }
    }
}

extern __typeof__(dmac_vif1_enable) func_00233D00 __attribute__((alias("FUN_00233d00")));
