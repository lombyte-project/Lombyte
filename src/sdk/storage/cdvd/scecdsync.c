#include "types.h"
#include "rnc/sdk/libcdvd.h"

extern u8  cd_sync_wait_debug_message[] __asm__("D_00152EF0");
extern s32 SceSifCheckStatRpc();
extern s32 sceCdDelayThread();
extern s32 scePrintf();

s32 sceCdSync(s32 mode) {
    if (mode == 0) {
        if (cd_debug > 0) {
            scePrintf(cd_sync_wait_debug_message);
        }
        while (cd_read_pending != 0 || SceSifCheckStatRpc(&cd_ncmd_client) != 0) {
            sceCdDelayThread(0x3C);
        }
        return 0;
    }
    if (cd_read_pending != 0 || SceSifCheckStatRpc(&cd_ncmd_client) != 0) {
        return 1;
    }
    return 0;
}
