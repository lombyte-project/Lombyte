#include "types.h"
struct M2c_arg0 {
    u8 pad_0[0xC];
    u16 unkC;
    s16 unkE;
    u8 pad_12[0x44];
    s32 unk54;
};

extern s32 reentrant_syscall_with_three_arguments() __asm__("func_00114518");
extern s32 reentrant_write() __asm__("func_001185D0");
s64 __swrite(struct M2c_arg0 *arg0, s32 arg1, s32 arg2) {
    s64 r;

    if (arg0->unkC & 0x100) {
        reentrant_syscall_with_three_arguments(arg0->unk54, arg0->unkE, 0, 2);
    }
    arg0->unkC = (u16)(arg0->unkC & 0xEFFF);
    r = reentrant_write(arg0->unk54, arg0->unkE, arg1, arg2);
    return (s64)(s32)(u32)r;
}
