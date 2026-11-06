#include "types.h"
#include "rnc/sdk/sifrpc.h"
#include "rnc/data_labels.h"
#include "rnc/sdk/libmc.h"
#include "rnc/sdk/rpc/rpc.h"
#include "rnc/sdk/storage/memory_card.h"

/* RPC parameters of a name call (mcNameParam in the open-source PS2 SDK). */
struct McChdirRequest {
    s32 port;
    s32 slot;
    u8  pad_8[0x8];
    s32 buffer;
    s8  name[0x400];
};

extern struct McChdirRequest D_00159AB0;
extern s32                   PollSema();
extern s32                   SignalSema();
extern s32                   sceSifWriteBackDCache();
extern s32                   strncpy(s8 *, s8 *, s32);

s32 sceMcChdir(s32 port, s32 slot, s8 *path, s32 pwd) {
    enum {
        RPC_DISPATCH_SUCCESS = 0
    };
    s32 rpc_dispatch_status;
    s32 chdir_request_status;

    if (mc_rpc_client.serve == 0) {
        return -0x64;
    }
    chdir_request_status = -0xC8;
    if (PollSema(mc_sema) >= 0) {
        if ((path == NULL) || (*path == 0)) {
            SignalSema(mc_sema);
            return -0xD2;
        }
        D_00159AB0.port = port;
        D_00159AB0.buffer = D_00159FC0;
        D_00159AB0.slot = slot;
        strncpy(D_00159AB0.name, path, 0x3FF);
        D_00159AB0.name[0x3FF] = 0;
        sceSifWriteBackDCache(D_00159FC0, 0x400);
        rpc_dispatch_status = sceSifCallRpc(&mc_rpc_client,
                                            MC_FUNC_CHDIR,
                                            1,
                                            &D_00159AB0,
                                            0x414,
                                            mc_rpc_result,
                                            4,
                                            &mceStorePwd,
                                            pwd);
        if (rpc_dispatch_status == RPC_DISPATCH_SUCCESS) {
            mc_current_func = MC_FUNC_CHDIR;
        } else {
            SignalSema(mc_sema);
        }
        chdir_request_status = rpc_dispatch_status;
        /* Duplicate return node #10. Try simplifying control flow for better match */
        return chdir_request_status;
    }
    return chdir_request_status;
}
