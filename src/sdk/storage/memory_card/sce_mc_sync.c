#include "types.h"
extern u32 D_00132DA8[];
extern u32 D_00132DAC[];
extern u8 D_00159A00[];
extern u32 D_0015AFC0[];
extern s32 SceSifCheckStatRpc();
extern s32 SignalSema();
extern s32 mcDelayThread();
s32 sceMcSync(s32 arg0, s32 *arg1, s32 *arg2) {
    s32 var_16_24;
    register s32 wait_flag = arg0;
    register s32 *code_out = arg1;
    register s32 *status_out = arg2;

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
