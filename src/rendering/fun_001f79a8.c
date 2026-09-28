#include "types.h"
extern f32 D_0015F348 __attribute__((sda));
extern s32 D_0015F478;
extern s32 func_001F76A0();
extern s32 func_001F89A4();
extern s32 func_001F8FF0();
extern void func_00233980(s32, s64);
/* retail small-data globals, declared to GAS before the body */

void FUN_001f79a8(void) {
    if (D_0015F478 != 0) {
        func_00233980(8, 5);
        func_00233980(0x14, 0x61);
        func_00233980(0x47, 0x513F1);
        func_00233980(0x4A, 1);
        func_001F8FF0();
        D_0015F348 = -0.04f;
        func_001F76A0();
        func_001F89A4();
        D_0015F348 = 0;
        func_00233980(0x4A, 0);
    }
}

extern __typeof__(FUN_001f79a8) func_001F79A8 __attribute__((alias("FUN_001f79a8")));
