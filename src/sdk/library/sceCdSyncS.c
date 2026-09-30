#include "types.h"
extern s32 D_001312D0[];
extern u8 D_00132D08[];
extern u8 D_00152F00[];
extern s32 SceSifCheckStatRpc();
extern s32 sceCdDelayThread();
extern s32 scePrintf();

s32 sceCdSyncS(s32 mode) {
    if (mode == 0) {
        if (D_001312D0[0] > 0) {
            scePrintf(D_00152F00);
        }
        while (SceSifCheckStatRpc(D_00132D08) != 0) {
            sceCdDelayThread(0x3C);
        }
        return 0;
    }
    return SceSifCheckStatRpc(D_00132D08);
}
