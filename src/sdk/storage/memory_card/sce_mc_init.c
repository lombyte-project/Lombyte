#include "types.h"
#include "kernel.h"
#include "rnc/sdk/libmc.h"

extern char mc_rpc_bind_error_message[] __asm__("D_00153450");
extern char mcserv_old_release_message[] __asm__("D_00153468");
extern char mcman_old_release_message[] __asm__("D_00153490");
extern u8   D_00159A80[];
extern s32  CreateSema(struct SemaParam *);
extern s32  WaitSema(s32);
extern s32  SignalSema(s32);
extern s32  sceMcSync(s32, s32 *, s32 *);
extern void sceSifInitRpc(u32);
extern s32  sceSifBindRpc(struct sceSifClientData *, u32, u32);
extern s32
sceSifCallRpc(struct sceSifClientData *, u32, u32, void *, s32, void *, s32, void *, void *);
extern s32 scePrintf(const char *, ...);

s32 sceMcInit(void) {
    struct SemaParam param;
    s32              rpc_call_result;
    s32              i;

    if (mc_sema < 0) {
        param.initCount = 1;
        param.maxCount = 1;
        param.option = 0;
        mc_sema = CreateSema(&param);
    }
    sceMcSync(0, 0, 0);
    WaitSema(mc_sema);
    sceSifInitRpc(0);
    while (1) {
        if (sceSifBindRpc(&mc_rpc_client, 0x80000400, 0) < 0) {
            scePrintf(mc_rpc_bind_error_message);
            while (1) {
            }
        }
        if (mc_rpc_client.serve != 0) {
            break;
        }
        i = 0x100000;
        while (--i != 0) {
        }
    }
    rpc_call_result =
        sceSifCallRpc(&mc_rpc_client, MC_FUNC_INIT, 0, D_00159A80, 48, mc_rpc_result, 12, 0, 0);
    SignalSema(mc_sema);
    if (rpc_call_result < 0) {
        mc_rpc_client.serve = 0;
        return rpc_call_result - 100;
    }
    if (mc_rpc_result[1] < 522) {
        scePrintf(mcserv_old_release_message);
        mc_rpc_client.serve = 0;
        return -120;
    }
    if (mc_rpc_result[2] < 526) {
        scePrintf(mcman_old_release_message);
        mc_rpc_client.serve = 0;
        return -121;
    }
    return mc_rpc_result[0];
}
