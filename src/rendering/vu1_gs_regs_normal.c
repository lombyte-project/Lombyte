#include "types.h"
#include "sda.h"
extern s32 *D_00160F00 MACRO_ADDR;
extern u8 D_001DE3C0[];
void vu1_gs_regs_normal(void) __asm__("FUN_00233bc8");

void vu1_gs_regs_normal(void) {
    *D_00160F00 = 0x30000003;
    *(s32 *)((u32)D_00160F00 + 4) = (s32)D_001DE3C0;
    *(s32 *)((u32)D_00160F00 + 8) = 0;
    *(s32 *)((u32)D_00160F00 + 12) = 0x50000003;
    D_00160F00 += 4;
}
extern __typeof__(vu1_gs_regs_normal) func_00233BC8 __attribute__((alias("FUN_00233bc8")));
