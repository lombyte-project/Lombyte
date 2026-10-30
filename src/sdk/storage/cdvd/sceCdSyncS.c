#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/sdk/libcdvd.h"

extern u8  cd_s_command_wait_message[] __asm__("D_00152F00");
extern s32 SceSifCheckStatRpc();
extern s32 sceCdDelayThread();
extern s32 scePrintf();

s32 sceCdSyncS(s32 mode) {
    if (mode == 0) {
        if (cd_debug > 0) {
            scePrintf(cd_s_command_wait_message);
        }
        while (SceSifCheckStatRpc(&cd_scmd_client) != 0) {
            sceCdDelayThread(0x3C);
        }
        return 0;
    }
    return SceSifCheckStatRpc(&cd_scmd_client);
}
