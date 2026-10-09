/*
 * Derived from newlib, which is distributed under permissive BSD-style
 * terms. See THIRD_PARTY_NOTICES.md and licenses/COPYING.NEWLIB.txt.
 */

/* Source: newlib. */

#include "types.h"
#include "rnc/runtime/core_state.h"
extern s32 sceTtyInit();
extern s32 sceTtyWrite();
s32 write(s32 fd, s32 buf, s32 len) {
    register s32 tty_arg = buf;
    if ((u32)(fd - 1) < 2U) {
        if (CoreGlobalWord == 0) {
            if (sceTtyInit() != 0) {
                CoreGlobalWord = 1;
                goto block_4;
            }
            goto block_5;
        }
    block_4:
        return sceTtyWrite(tty_arg, len);
    }
block_5:
    return -1;
}
