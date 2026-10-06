#include "types.h"
struct _reent {
    s32 _errno;
};
extern s32 D_0015EC10[];
extern s32 func_00118E08(s32, s32, s32);
s32 reentrant_syscall_with_three_arguments(struct _reent *ptr, s32 a, s32 b,
                                           s32 c) __asm__("FUN_00114518");

s32 reentrant_syscall_with_three_arguments(struct _reent *ptr, s32 a, s32 b, s32 c) {
    s32 ret;

    D_0015EC10[0] = 0;
    if ((ret = func_00118E08(a, b, c)) == -1 && D_0015EC10[0] != 0) {
        ptr->_errno = D_0015EC10[0];
    }
    return ret;
}

extern __typeof__(reentrant_syscall_with_three_arguments) func_00114518
    __attribute__((alias("FUN_00114518")));
