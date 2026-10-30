#include "types.h"
#include "sifrpc.h"
#include "rnc/data_labels.h"
#include "rnc/sdk/libmc.h"

struct MemoryCardNameParam {
    s32  port;
    s32  slot;
    s32  flags;
    s32  maxent;
    s32  unk10;
    char name[0x400];
};

extern s32   PollSema();
extern s32   SignalSema();
extern s32   sceSifCallRpc();
extern char *strncpy();

s32 sceMcOpen(s32 port, s32 slot, char *name, s32 mode) {
    struct MemoryCardNameParam *request;
    s32                         rpc_result;

    if (mc_rpc_client.serve == 0) {
        return -0x64;
    }
    if (PollSema(mc_sema) < 0) {
        return -0xC8;
    }
    if (name == 0 || name[0] == 0) {
        SignalSema(mc_sema);
        return -0xD2;
    }
    strncpy(D_00159AC4, name, 0x3FF);
    request = (struct MemoryCardNameParam *)(D_00159AC4 - 0x14);
    request->port = port;
    request->flags = mode;
    request->slot = slot;
    request->name[0x3FF] = 0;
    rpc_result =
        sceSifCallRpc(&mc_rpc_client, MC_FUNC_OPEN, 1, request, 0x414, mc_rpc_result, 4, 0, 0);
    if (rpc_result == 0) {
        mc_current_func = MC_FUNC_OPEN;
    } else {
        SignalSema(mc_sema);
    }
    return rpc_result;
}
