#include "types.h"
#include "qzero.h"
struct S {
    u8 pad_0[0x140];
    float f140, f144, f148;
    u8 pad_14C[0x204];
    float f350;
    u8 pad_354[0x10];
    float f364;
    u8 pad_368[0x10];
    float f378, f37C;
};
extern struct S D_00186F40;
extern s32 D_001872A0;
extern s32 D_001872B0;
void FUN_00218d10(void) {
    D_00186F40.f140 = 256.0f;
    D_00186F40.f148 = 64.0f;
    D_00186F40.f144 = 256.0f;
    qzero(&D_00186F40.f350);
    qzero(&D_001872A0);
    qzero(&D_001872B0);
    D_00186F40.f350 = 1.0f;
    D_00186F40.f364 = 1.0f;
    D_00186F40.f378 = 1.0f;
    D_00186F40.f37C = 1.0f;
}

extern __typeof__(FUN_00218d10) func_00218D10 __attribute__((alias("FUN_00218d10")));
