#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/sdk/libcdvd.h"
#include "rnc/sdk/deci2.h"
#include "rnc/sdk/rpc/rpc.h"
#include "rnc/sdk/storage/cdvd.h"

const char cd_s_command_wait_message[] __asm__("D_00152F00") = "S cmd wait\n";

s32 sceCdSyncS(s32 mode) {
    enum {
        CD_SYNC_S_WAIT = 0
    };
    if (mode == CD_SYNC_S_WAIT) {
        if (cd_debug > 0) {
            scePrintf(cd_s_command_wait_message);
        }
        while (SceSifCheckStatRpc(&cd_scmd_client) != 0) {
            SceCdDelayThread(0x3C);
        }
        return 0;
    }
    return SceSifCheckStatRpc(&cd_scmd_client);
}
