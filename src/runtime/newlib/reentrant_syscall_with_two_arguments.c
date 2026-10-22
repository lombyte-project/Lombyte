#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/sdk/library/sdk_globals.h"
struct _reent {
    s32 _errno;
};
extern s32 func_00118EC8(s32, s32);
s32 reentrant_syscall_with_two_arguments(struct _reent *ptr, s32 a, s32 b) __asm__("FUN_00114000");

s32 reentrant_syscall_with_two_arguments(struct _reent *reentrancy_context, s32 file_descriptor, s32 stat_buffer) {
    s32 syscall_result;

    SbrkErrno[0] = 0;
    if ((syscall_result = func_00118EC8(file_descriptor, stat_buffer)) == -1 && SbrkErrno[0] != 0) {
        reentrancy_context->_errno = SbrkErrno[0];
    }
    return syscall_result;
}

extern __typeof__(reentrant_syscall_with_two_arguments) func_00114000
    __attribute__((alias("FUN_00114000")));
