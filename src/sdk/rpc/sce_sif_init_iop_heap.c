/* sceSifInitIopHeap: SDK compiler, built with the legacy per-unit
 * -fno-schedule-insns entry in configure.py (SDK_COMPILER_FLAG_UNITS). */

#include "types.h"
struct SifClient { u8 pad_0[0x24]; s32 unk24; };
extern u8 D_0012FCAC[];
extern struct SifClient D_00158040;
extern s32 sceSifBindRpc();
s32 sceSifInitIopHeap(void) {
    s32 delay;
    goto bind;
retry:
    delay = 0x100000;
    do {
        delay -= 1;
    } while (delay != -1);
bind:
    if (sceSifBindRpc(&D_00158040, 0x80000003u, 0) < 0) {
        return -1;
    }
    if (D_00158040.unk24 == 0) {
        goto retry;
    }
    *(s32 *)D_0012FCAC = 0;
    return 0;
}