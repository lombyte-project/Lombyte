#include "types.h"
struct sceSifClientData {
    u8 reserved_00[0x24];
    s32 is_initialized;
};

extern u32 D_00132DA8[];
extern u32 D_00132DAC[];
extern struct sceSifClientData D_00159A00;
extern u8 D_00159A80[];
extern u8 D_0015AFC0[];
extern s32 PollSema();
extern s32 SignalSema();
extern s32 sceSifCallRpc();
s32 sceMcClose(s32 file_descriptor) {
    s32 rpc_result;

    if (D_00159A00.is_initialized == 0) {
        return -0x64;
    }
    if (PollSema(D_00132DAC[0]) < 0) {
        return -0xC8;
    }
    *(s32 *)D_00159A80 = file_descriptor;
    rpc_result = sceSifCallRpc(&D_00159A00, 3, 1, D_00159A80, 0x30, D_0015AFC0, 4, 0, 0);
    if (rpc_result == 0) {
        D_00132DA8[0] = 3;
    } else {
        SignalSema(D_00132DAC[0]);
    }
    return rpc_result;
}
