#include "types.h"
struct _reent {
    s32 _errno;
};
extern s32 D_0015EC10[];
extern s32 write(s32, s32, s32);
s32 reentrant_write(struct _reent *ptr, s32 a, s32 b, s32 c) __asm__("FUN_001185d0");

s32 reentrant_write(struct _reent *ptr, s32 a, s32 b, s32 c) {
    s32 ret;

    D_0015EC10[0] = 0;
    if ((ret = write(a, b, c)) == -1 && D_0015EC10[0] != 0) {
        ptr->_errno = D_0015EC10[0];
    }
    return ret;
}

extern __typeof__(reentrant_write) func_001185D0 __attribute__((alias("FUN_001185d0")));
