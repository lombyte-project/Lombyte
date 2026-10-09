/*
 * Derived from newlib, which is distributed under permissive BSD-style
 * terms. See THIRD_PARTY_NOTICES.md and licenses/COPYING.NEWLIB.txt.
 */

/* Source: newlib. */

#include "types.h"
#include "rnc/runtime/core_state.h"

extern s32 sceTtyInit(void);
extern s32 sceTtyRead();
s32 read(s32 arg0, s32 arg1, s32 arg2) {
    if (arg0 == 0) {
        if (CoreGlobalWord == 0) {
            if (sceTtyInit() != 0) {
                do {
                    CoreGlobalWord = 1;
                } while (0);
                goto block_4;
            }
            goto block_5;
        }
    block_4:
        return sceTtyRead(arg1, arg2);
    }
block_5:
    return -1;
}
