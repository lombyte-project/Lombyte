#include "types.h"
#include "sda.h"
extern s32 *D_00160F00 MACRO_ADDR;

void vu1_add_vif_code(s32 arg0) __asm__("FUN_00233938");

void vu1_add_vif_code(s32 arg0) {
    *D_00160F00 = 0x10000000;
    *(s32 *)((u32)D_00160F00 + 4) = 0;
    *(s32 *)((u32)D_00160F00 + 8) = 0;
    *(s32 *)((u32)D_00160F00 + 12) = arg0;
    D_00160F00 += 4;
}

extern __typeof__(vu1_add_vif_code) func_00233938 __attribute__((alias("FUN_00233938")));
