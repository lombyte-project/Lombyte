#include "types.h"
extern s32 D_0013E504[];
extern s64 D_00160588;
extern void func_001F5450(s32, s32, s32, s32, s32, s32, s32, s32, s64, s64);
extern void func_00233980(s32, s64);
void FUN_0022ea08(s32 arg0) {
    func_00233980(0x47, 0x31801);
    func_00233980(0x42, (((s64)0x8000) << 24) | 0x44);
    func_001F5450(0x20, D_0013E504[0] - 0x58, 0x100, 0x20, 0, 0, 0x100, 0x20, (s64)((arg0 << 24) | 0x808080), D_00160588);
    func_00233980(0x47, 0x5360B);
}
