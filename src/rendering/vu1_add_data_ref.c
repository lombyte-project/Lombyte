#include "types.h"
#include "sda.h"
extern s32 *D_00160F00 MACRO_ADDR;
void vu1_add_data_ref(s32 arg0, s32 arg1) __asm__("FUN_00233830");

void vu1_add_data_ref(s32 arg0, s32 arg1)
{
    *D_00160F00 = arg1 | 0x30000000;
    *(s32 *)((u32)D_00160F00 + 4) = arg0;
    *(s32 *)((u32)D_00160F00 + 8) = 0;
    *(s32 *)((u32)D_00160F00 + 12) = 0;
    D_00160F00 += 4;
}

extern __typeof__(vu1_add_data_ref) func_00233830 __attribute__((alias("FUN_00233830")));
