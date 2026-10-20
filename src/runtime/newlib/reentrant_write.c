#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/sdk/library/sdk_globals.h"
struct _reent {
    s32 _errno;
};
extern s32 write(s32, s32, s32);
s32 reentrant_write(struct _reent *ptr, s32 a, s32 b, s32 c) __asm__("FUN_001185d0");

s32 reentrant_write(struct _reent *ptr, s32 file_descriptor, s32 b, s32 byte_count) {
    s32 ret;

    SbrkErrno[0] = 0;
    if ((ret = write(file_descriptor, b, byte_count)) == -1 && SbrkErrno[0] != 0) {
        ptr->_errno = SbrkErrno[0];
    }
    return ret;
}

extern __typeof__(reentrant_write) func_001185D0 __attribute__((alias("FUN_001185d0")));
