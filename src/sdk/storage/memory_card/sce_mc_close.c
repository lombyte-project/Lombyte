#include "types.h"
#include "sifrpc.h"
#include "rnc/data_labels.h"
#include "rnc/sdk/libmc.h"

extern s32 PollSema();
extern s32 SignalSema();
extern s32 sceSifCallRpc();

s32 sceMcClose(s32 file_descriptor) {
    s32 rpc_result;

    if (mc_rpc_client.serve == 0) {
        return -0x64;
    }
    if (PollSema(mc_sema) < 0) {
        return -0xC8;
    }
    *(s32 *)D_00159A80 = file_descriptor;
    rpc_result =
        sceSifCallRpc(&mc_rpc_client, MC_FUNC_CLOSE, 1, D_00159A80, 0x30, mc_rpc_result, 4, 0, 0);
    if (rpc_result == 0) {
        mc_current_func = MC_FUNC_CLOSE;
    } else {
        SignalSema(mc_sema);
    }
    return rpc_result;
}
