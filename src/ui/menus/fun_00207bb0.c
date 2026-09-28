#include "types.h"
extern s32 D_001A0118 __attribute__((section(".data")));
extern s32 func_00207C28();
extern s32 func_00207E58();
extern void func_00208810();
void FUN_00207bb0(s32 arg0, u8 *arg1, s32 arg2) {
    func_00208810();
    if (D_001A0118 != 0) {
        if (*arg1 & 1) {
            func_00207C28(arg0, arg1, arg2);
        } else {
            func_00207E58(arg0, arg1);
        }
    }
}
