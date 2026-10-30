#include "types.h"
#include "sifrpc.h"
#include "rnc/sdk/libmc.h"

struct MemoryCardSeekRequest {
    s32 file_descriptor;
    u8  reserved_04[0xC];
    s32 offset;
    s32 origin;
};

extern struct MemoryCardSeekRequest D_00159A80;
extern s32                          PollSema();
extern s32                          SignalSema();
extern s32                          sceSifCallRpc();

s32 sceMcSeek(s32 file_descriptor, s32 offset, s32 origin) {
    s32 rpc_result;

    if (mc_rpc_client.serve == 0) {
        return -0x64;
    }
    if (PollSema(mc_sema) < 0) {
        return -0xC8;
    }
    D_00159A80.file_descriptor = file_descriptor;
    D_00159A80.offset = offset;
    D_00159A80.origin = origin;
    rpc_result =
        sceSifCallRpc(&mc_rpc_client, MC_FUNC_SEEK, 1, &D_00159A80, 0x30, mc_rpc_result, 4, 0, 0);
    if (rpc_result == 0) {
        mc_current_func = MC_FUNC_SEEK;
    } else {
        SignalSema(mc_sema);
    }
    return rpc_result;
}
