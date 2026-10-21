#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/sdk/library/sdk_globals.h"
struct _reent {
    s32 _errno;
};
extern s32 func_00118E08(s32, s32, s32);
s32 reentrant_syscall_with_three_arguments(struct _reent *ptr, s32 a, s32 b,
                                           s32 c) __asm__("FUN_00114518");

s32 reentrant_syscall_with_three_arguments(struct _reent *reentrancy_context, s32 file_descriptor, s32 seek_offset, s32 seek_origin) {
    s32 seek_result;

    SbrkErrno[0] = 0;
    if ((seek_result = func_00118E08(file_descriptor, seek_offset, seek_origin)) == -1 && SbrkErrno[0] != 0) {
        reentrancy_context->_errno = SbrkErrno[0];
    }
    return seek_result;
}

extern __typeof__(reentrant_syscall_with_three_arguments) func_00114518
    __attribute__((alias("FUN_00114518")));
