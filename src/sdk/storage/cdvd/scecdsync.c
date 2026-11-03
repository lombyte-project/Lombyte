#include "types.h"
#include "rnc/sdk/libcdvd.h"
#include "rnc/sdk/deci2.h"
#include "rnc/sdk/rpc/rpc.h"
#include "rnc/sdk/storage/cdvd.h"

const char cd_sync_wait_debug_message[] __asm__("D_00152EF0") = "N cmd wait\n";

s32 sceCdSync(s32 mode) {
    enum {
        CD_SYNC_BLOCKING = 0
    };
    if (mode == CD_SYNC_BLOCKING) {
        if (cd_debug > 0) {
            scePrintf(cd_sync_wait_debug_message);
        }
        while (cd_read_pending != 0 || SceSifCheckStatRpc(&cd_ncmd_client) != 0) {
            SceCdDelayThread(0x3C);
        }
        return 0;
    }
    if (cd_read_pending != 0 || SceSifCheckStatRpc(&cd_ncmd_client) != 0) {
        return 1;
    }
    return 0;
}
