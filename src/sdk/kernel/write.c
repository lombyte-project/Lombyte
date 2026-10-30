/*
 * Derived from newlib, which is distributed under permissive BSD-style
 * terms. See THIRD_PARTY_NOTICES.md and licenses/COPYING.NEWLIB.txt.
 */

/* Source: newlib. */

#include "types.h"
#include "rnc/runtime/core_state.h"

extern s32 sceTtyInit();
extern s32 sceTtyWrite();

s32 write(s32 fd, s32 source_buffer, s32 byte_count) {
    register s32 tty_arg = source_buffer;
    if ((u32)(fd - 1) < 2U) {
        if (tty_initialized == 0) {
            if (sceTtyInit() != 0) {
                tty_initialized = 1;
                goto block_4;
            }
            goto block_5;
        }
    block_4:
        return sceTtyWrite(tty_arg, byte_count);
    }
block_5:
    return -1;
}
