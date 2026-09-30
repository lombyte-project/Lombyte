#include "types.h"
#include "sda.h"
extern s32 *D_00160F00 MACRO_ADDR;
extern u8 D_001DEE00[];
void vu1_tex_flush(void) __asm__("FUN_00233b68");

void vu1_tex_flush(void)
{
    *D_00160F00 = 0x30000003;
    *(s32 *)((u32)D_00160F00 + 4) = (s32)D_001DEE00;
    *(s32 *)((u32)D_00160F00 + 8) = 0;
    *(s32 *)((u32)D_00160F00 + 12) = 0x50000003;
    D_00160F00 += 4;
}

extern __typeof__(vu1_tex_flush) func_00233B68 __attribute__((alias("FUN_00233b68")));
