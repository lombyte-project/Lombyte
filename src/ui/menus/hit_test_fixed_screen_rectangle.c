#include "types.h"

extern void func_00208818(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);

void hit_test_fixed_screen_rectangle(s32 arg0, s32 arg1) __asm__("FUN_00206978");

void hit_test_fixed_screen_rectangle(s32 arg0, s32 arg1) {
    func_00208818(arg0, arg1, 0xd3, 0xdb, 0x129, 0xf9);
}
