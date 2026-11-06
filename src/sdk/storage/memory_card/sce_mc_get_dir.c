#include "types.h"
#include "rnc/sdk/sifrpc.h"
#include "rnc/data_labels.h"
#include "rnc/sdk/libmc.h"
#include "rnc/sdk/rpc/rpc.h"

/* RPC parameters of a name call (mcNameParam in the open-source PS2 SDK). */
struct McGetDirRequest {
    s32 port;
    s32 slot;
    s32 mode;
    s32 maxent;
    s32 table;
    s8  name[0x400];
};

extern struct McGetDirRequest D_00159AB0;
extern s32                    PollSema();
extern s32                    SignalSema();
extern s32                    sceSifWriteBackDCache();
extern s32                    strncpy(s8 *, s8 *, s32);

s32 sceMcGetDir(s32 port, s32 slot, s8 *name, s32 mode, s32 maxent, s32 table) {
    enum {
        RPC_SUBMISSION_SUCCESS = 0
    };
    s32          rpc_submission_status;
    s32          request_status;
    register s32 path_arg = slot;

    if (mc_rpc_client.serve == 0) {
        return -0x64;
    }
    request_status = -0xC8;
    if (PollSema(mc_sema) >= 0) {
        if ((name == NULL) || (*name == 0)) {
            SignalSema(mc_sema);
            return -0xD2;
        }
        D_00159AB0.port = port;
        D_00159AB0.slot = path_arg;
        D_00159AB0.mode = mode;
        D_00159AB0.maxent = maxent;
        D_00159AB0.table = table;
        strncpy(D_00159AB0.name, name, 0x3FF);
        D_00159AB0.name[0x3FF] = 0;
        if (maxent >= 0) {
            sceSifWriteBackDCache(table, maxent << 6);
        }
        rpc_submission_status = sceSifCallRpc(
            &mc_rpc_client, MC_FUNC_GET_DIR, 1, &D_00159AB0, 0x414, mc_rpc_result, 4, 0, 0);
        if (rpc_submission_status == RPC_SUBMISSION_SUCCESS) {
            mc_current_func = MC_FUNC_GET_DIR;
        } else {
            SignalSema(mc_sema);
        }
        request_status = rpc_submission_status;
        /* Duplicate return node #12. Try simplifying control flow for better match */
        return request_status;
    }
    return request_status;
}
