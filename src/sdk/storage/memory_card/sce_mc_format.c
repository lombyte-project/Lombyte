#include "types.h"
#include "sifrpc.h"
#include "rnc/sdk/libmc.h"

struct MemoryCardFormatRequest {
    u8  reserved_00[0x4];
    s32 port;
    s32 slot;
};

extern struct MemoryCardFormatRequest D_00159A80;
extern s32                            PollSema();
extern s32                            SignalSema();
extern s32                            sceSifCallRpc();

s32 SceMcFormat(s32 port, s32 slot) {
    s32 rpc_result;

    if (mc_rpc_client.serve == 0) {
        return -0x64;
    }
    if (PollSema(mc_sema) < 0) {
        return -0xC8;
    }
    D_00159A80.port = port;
    D_00159A80.slot = slot;
    rpc_result =
        sceSifCallRpc(&mc_rpc_client, MC_FUNC_FORMAT, 1, &D_00159A80, 0x30, mc_rpc_result, 4, 0, 0);
    if (rpc_result == 0) {
        mc_current_func = MC_FUNC_FORMAT;
    } else {
        SignalSema(mc_sema);
    }
    return rpc_result;
}
