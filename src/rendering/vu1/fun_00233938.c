#include "types.h"
#include "sda.h"
extern s32 *D_00160F00 MACRO_ADDR;

void FUN_00233938(s32 arg0)
{
    *D_00160F00 = 0x10000000;
    *(s32 *)((u32)D_00160F00 + 4) = 0;
    *(s32 *)((u32)D_00160F00 + 8) = 0;
    *(s32 *)((u32)D_00160F00 + 12) = arg0;
    D_00160F00 += 4;
}

extern __typeof__(FUN_00233938) func_00233938 __attribute__((alias("FUN_00233938")));
