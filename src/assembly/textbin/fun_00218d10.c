#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00218d10/FUN_00218d10.s", FUN_00218d10);
#else
#include "rnc/fun_00218d10_types.h"
#include "types.h"


extern struct M2c_D_00186F40 D_00186F40;
extern s32 D_001872A0;
extern s32 D_001872B0;
void FUN_00218d10(void) {
    D_00186F40.unk144 = 256.0f;
    D_00186F40.unk148 = 64.0f;
    D_00186F40.unk140 = 256.0f;
    D_00186F40.unk350 = 0;
    D_001872A0 = 0;
    D_001872B0 = 0;
    D_00186F40.unk37C = 1.0f;
    D_00186F40.unk350 = 1.0f;
    D_00186F40.unk364 = 1.0f;
    D_00186F40.unk378 = 1.0f;
}

extern void func_00218D10(void) __attribute__((alias("FUN_00218d10")));
#endif /* NON_MATCHING */
