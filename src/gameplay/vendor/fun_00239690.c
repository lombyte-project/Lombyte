#include "types.h"
extern void func_001FB440(s32, s32, s32);
extern void func_001F33B8(s32, s32, f32, f32, f32, f32, f32);
extern void func_00233980(s32, u64);
void FUN_00239690(s32 arg0, s32 arg1, f32 fparg0) {
    s32 sum;
    sum = arg0 + arg1;
    sum = (sum < 17) ? sum : 16;
    func_001FB440(arg0, arg1, ((0x3FF000 - (4 << sum)) >> 13) << 13);
    func_001F33B8(1 << arg0, 1 << arg1, fparg0, 0.0f, 524288.0f, 255.0f, 0.0f);
    func_00233980(0x47, 0x30000);
    func_00233980(0x42, (((u64)0x8000) << 24) | 0x44);
}
