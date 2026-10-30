#include "types.h"
#include "sifrpc.h"
#include "rnc/data_labels.h"
#include "rnc/sdk/libmc.h"

struct McChdirRequest {
    s32 unk0;
    s32 unk4;
    u8  pad_8[0x8];
    s32 unk10;
    u8  pad_14[0x3FF];
    s32 unk413;
};

extern struct McChdirRequest D_00159AB0;
extern s32                   PollSema();
extern s32                   SignalSema();
extern s32                   sceSifCallRpc();
extern s32                   sceSifWriteBackDCache();
extern s32                   strncpy(s8 *, s8 *, s32);
extern void                  mceStorePwd();

s32 sceMcChdir(s32 port, s32 slot, s8 *path, s32 pwd) {
    s32 result;
    s32 ret;

    if (mc_rpc_client.serve == 0) {
        return -0x64;
    }
    ret = -0xC8;
    if (PollSema(mc_sema) >= 0) {
        if ((path == NULL) || (*path == 0)) {
            SignalSema(mc_sema);
            return -0xD2;
        }
        D_00159AB0.unk0 = port;
        D_00159AB0.unk10 = D_00159FC0;
        D_00159AB0.unk4 = slot;
        strncpy((s8 *)((u8 *)&D_00159AB0 + 0x14), path, 0x3FF);
        *(u8 *)((u8 *)&D_00159AB0 + 0x413) = 0;
        sceSifWriteBackDCache(D_00159FC0, 0x400);
        result = sceSifCallRpc(&mc_rpc_client,
                               MC_FUNC_CHDIR,
                               1,
                               &D_00159AB0,
                               0x414,
                               mc_rpc_result,
                               4,
                               &mceStorePwd,
                               pwd);
        if (result == 0) {
            mc_current_func = MC_FUNC_CHDIR;
        } else {
            SignalSema(mc_sema);
        }
        ret = result;
        /* Duplicate return node #10. Try simplifying control flow for better match */
        return ret;
    }
    return ret;
}
