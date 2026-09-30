#include "types.h"
#include "sda.h"
extern s32 *D_00160F00 MACRO_ADDR;
extern u8 D_0013CF10[];


void FUN_00233c90(void)
{
    *D_00160F00 = 0x3000000B;
    *(s32 *)((u32)D_00160F00 + 4) = (s32)D_0013CF10;
    *(s32 *)((u32)D_00160F00 + 8) = 0;
    *(s32 *)((u32)D_00160F00 + 12) = 0x5000000B;
    D_00160F00 += 4;
}
extern __typeof__(FUN_00233c90) func_00233C90 __attribute__((alias("FUN_00233c90")));
