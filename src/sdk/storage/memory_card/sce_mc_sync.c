#include "types.h"
extern u32 D_00132DA8[];
extern s32 D_00132DAC[];
extern u8 D_00159A00[];
extern u32 D_0015AFC0[];
extern s32 SceSifCheckStatRpc();
extern s32 SignalSema();
extern s32 mcDelayThread();
s32 sceMcSync(s32 poll_mode, s32 *active_code_out, s32 *completion_status_out) {
    s32 var_16_24;
    register s32 wait_flag = poll_mode;
    register s32 *code_out = active_code_out;
    register s32 *status_out = completion_status_out;

    if (D_00132DA8[0] == 0) {
        return -1;
    }
    var_16_24 = SceSifCheckStatRpc(D_00159A00);
    if ((wait_flag == 0) && (var_16_24 != 0)) {
        while (SceSifCheckStatRpc(D_00159A00) != 0) {
            mcDelayThread(0x3C);
        }
        var_16_24 = 0;
    }
    var_16_24 = (var_16_24 == 0);
    if (code_out != NULL) {
        *code_out = D_00132DA8[0];
    }
    if (var_16_24 != 0) {
        D_00132DA8[0] = 0;
        if (status_out != NULL) {
            *status_out = D_0015AFC0[0];
        }
        SignalSema(D_00132DAC[0]);
    }
    return var_16_24;
}
