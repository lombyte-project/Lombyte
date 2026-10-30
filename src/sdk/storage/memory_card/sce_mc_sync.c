#include "types.h"
#include "rnc/sdk/libmc.h"

extern s32 SceSifCheckStatRpc();
extern s32 SignalSema();
extern s32 mcDelayThread();

s32 sceMcSync(s32 poll_mode, s32 *active_code_out, s32 *completion_status_out) {
    s32           busy;
    register s32  wait_flag = poll_mode;
    register s32 *code_out = active_code_out;
    register s32 *status_out = completion_status_out;

    if (mc_current_func == 0) {
        return -1;
    }
    busy = SceSifCheckStatRpc(&mc_rpc_client);
    if ((wait_flag == 0) && (busy != 0)) {
        while (SceSifCheckStatRpc(&mc_rpc_client) != 0) {
            mcDelayThread(0x3C);
        }
        busy = 0;
    }
    busy = (busy == 0);
    if (code_out != NULL) {
        *code_out = mc_current_func;
    }
    if (busy != 0) {
        mc_current_func = 0;
        if (status_out != NULL) {
            *status_out = mc_rpc_result[0];
        }
        SignalSema(mc_sema);
    }
    return busy;
}
