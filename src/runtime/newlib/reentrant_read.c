#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/sdk/library/sdk_globals.h"
struct _reent {
    s32 _errno;
};
extern s32 read(s32, s32, s32);
s32 reentrant_read(struct _reent *ptr, s32 a, s32 b, s32 c) __asm__("FUN_00116108");

s32 reentrant_read(struct _reent *ptr, s32 file_descriptor, s32 b, s32 max_bytes) {
    s32 ret;

    SbrkErrno[0] = 0;
    if ((ret = read(file_descriptor, b, max_bytes)) == -1 && SbrkErrno[0] != 0) {
        ptr->_errno = SbrkErrno[0];
    }
    return ret;
}

extern __typeof__(reentrant_read) func_00116108 __attribute__((alias("FUN_00116108")));
