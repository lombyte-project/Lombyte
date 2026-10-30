#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/sdk/library/sdk_globals.h"

struct _reent {
    s32 _errno;
};

extern s32 read(s32, s32, s32);
s32        _read_r(struct _reent *ptr, s32 a, s32 b, s32 c) __asm__("FUN_00116108");

s32 _read_r(struct _reent *reent_context,
            s32            file_descriptor,
            s32            destination_buffer,

            s32 max_bytes) {
    s32 bytes_read;

    SbrkErrno[0] = 0;
    if ((bytes_read = read(file_descriptor, destination_buffer, max_bytes)) == -1 &&
        SbrkErrno[0] != 0) {
        reent_context->_errno = SbrkErrno[0];
    }
    return bytes_read;
}

extern __typeof__(_read_r) func_00116108 __attribute__((alias("FUN_00116108")));
