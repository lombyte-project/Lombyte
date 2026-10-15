/*
 * Derived from newlib, which is distributed under permissive BSD-style
 * terms. See THIRD_PARTY_NOTICES.md and licenses/COPYING.NEWLIB.txt.
 */

/* Source: newlib. */

#include "types.h"
extern u32 D_0015EC10[];
extern s32 return_minus_one() __asm__("func_00118E00");
s32 _close_r(s32 *reentrancy_error, s32 file_descriptor) {
    s32 close_result;

    D_0015EC10[0] = 0;
    close_result = return_minus_one(file_descriptor);
    if (close_result == -1) {
        if (D_0015EC10[0] != 0) {
            *reentrancy_error = D_0015EC10[0];
        }
    }
    return close_result;
}
