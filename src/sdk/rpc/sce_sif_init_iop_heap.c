/* C_EXACT - promoted 2026-09-09 (per-unit -fno-schedule-insns routing)
 * SYMBOL: sceSifInitIopHeap
 * COMPILER: EE-GCC 2.9-991111-01 -O2 -g2 -gstabs -fno-schedule-insns
 *   (per-unit flag via configure.py SDK_COMPILER_FLAG_UNITS; the flag is NOT
 *   production-default because it regresses the sibling scePad2Read)
 * SCORE: code=100 functions=100 data=100 complete_data=100
 * DECISION: promoted (staged object + full-ELF SHA gate PASS e05058...c0c9)
 * BLOCKER: none
 */

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