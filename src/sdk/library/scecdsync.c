#include "types.h"
extern s32 D_001312D0[];
extern u32 D_001312F0[];
extern u8 D_00132490[];
extern u8 D_00152EF0[];
extern s32 SceSifCheckStatRpc();
extern s32 sceCdDelayThread();
extern s32 scePrintf();

s32 sceCdSync(s32 mode) {
    if (mode == 0) {
        if (D_001312D0[0] > 0) {
            scePrintf(D_00152EF0);
        }
        while (D_001312F0[0] != 0 || SceSifCheckStatRpc(D_00132490) != 0) {
            sceCdDelayThread(0x3C);
        }
        return 0;
    }
    if (D_001312F0[0] != 0 || SceSifCheckStatRpc(D_00132490) != 0) {
        return 1;
    }
    return 0;
}
