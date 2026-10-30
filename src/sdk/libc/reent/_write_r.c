#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/sdk/library/sdk_globals.h"

struct _reent {
    s32 _errno;
};

extern s32 write(s32, s32, s32);
s32        _write_r(struct _reent *ptr, s32 a, s32 b, s32 c) __asm__("FUN_001185d0");

s32 _write_r(struct _reent *reent_context, s32 file_descriptor, s32 buffer, s32 byte_count) {
    s32 write_result;

    SbrkErrno[0] = 0;
    if ((write_result = write(file_descriptor, buffer, byte_count)) == -1 && SbrkErrno[0] != 0) {
        reent_context->_errno = SbrkErrno[0];
    }
    return write_result;
}

extern __typeof__(_write_r) func_001185D0 __attribute__((alias("FUN_001185d0")));
