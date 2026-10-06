#include "types.h"

#include "types.h"
extern s32 D_00160F18;
extern s32 D_00160F1C;
extern s32 RemoveDmacHandler();
extern s32 disable_dmac() __asm__("func_001190F8");
void dmac_vif1_disable(void) __asm__("FUN_00233d90");

void dmac_vif1_disable(void) {
    if (*(volatile s32 *)0x1000E010 & 0x20000) {
        *(volatile s32 *)0x1000E010 = 0x20000;
    }
    RemoveDmacHandler(1, D_00160F18);
    RemoveDmacHandler(0xF, D_00160F1C);
    disable_dmac(1);
    D_00160F18 = 0;
    D_00160F1C = 0;
}

extern void func_00233D90(void) __attribute__((alias("FUN_00233d90")));
