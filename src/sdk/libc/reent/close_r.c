/*
 * Derived from newlib, which is distributed under permissive BSD-style
 * terms. See THIRD_PARTY_NOTICES.md and licenses/COPYING.NEWLIB.txt.
 */

/* Source: newlib. */

#include "types.h"
#include "rnc/sdk/library/sdk_globals.h"

extern s32 _close() __asm__("func_00118E00");

s32 _close_r(s32 *reentrancy_error, s32 file_descriptor) {
    s32 close_result;

    SbrkErrno[0] = 0;
    close_result = _close(file_descriptor);
    if (close_result == -1) {
        if (SbrkErrno[0] != 0) {
            *reentrancy_error = SbrkErrno[0];
        }
    }
    return close_result;
}
