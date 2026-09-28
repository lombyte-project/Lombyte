#include "types.h"
extern void func_00115248(void *, const void *, s32);
extern s32 func_0023B810(s32, s32, s32, s32, s32, s32, s32, s32) __attribute__((alias("FUN_0023b810")));

s32 copy_video_buffer_region(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) __asm__("FUN_0023b810");

s32 copy_video_buffer_region(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    if (arg1 + arg3 < arg5 + arg7) {
        return 0;
    }
    if (arg5 >= arg1) {
        func_00115248(arg0, arg4, arg1);
        func_00115248(arg2, arg4 + arg1, arg5 - arg1);
        func_00115248((arg2 + arg5) - arg1, arg6, arg7);
    } else if (arg7 >= arg1 - arg5) {
        func_00115248(arg0, arg4, arg5);
        func_00115248(arg0 + arg5, arg6, arg1 - arg5);
        func_00115248(arg2, (arg6 + arg1) - arg5, arg7 - (arg1 - arg5));
    } else {
        func_00115248(arg0, arg4, arg5);
        func_00115248(arg0 + arg5, arg6, arg7);
    }
    return arg5 + arg7;
}
