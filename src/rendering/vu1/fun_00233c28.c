#include "types.h"
#include "sda.h"
extern s32 *D_00160F00 MACRO_ADDR;
extern u8 D_001DE3F0[];


void FUN_00233c28(void)
{
    *D_00160F00 = 0x30000003;
    *(s32 *)((u32)D_00160F00 + 4) = (s32)D_001DE3F0;
    *(s32 *)((u32)D_00160F00 + 8) = 0;
    *(s32 *)((u32)D_00160F00 + 12) = 0x50000003;
    D_00160F00 += 4;
}
extern __typeof__(FUN_00233c28) func_00233C28 __attribute__((alias("FUN_00233c28")));
