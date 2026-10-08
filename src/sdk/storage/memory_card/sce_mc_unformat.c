#include "types.h"
#include "sifrpc.h"
struct MemoryCardUnformatRequest {
    u8 reserved_00[0x4];
    s32 port;
    s32 slot;
};

extern u32 D_00132DA8[];
extern u32 D_00132DAC[];
extern struct sceSifClientData D_00159A00;
extern struct MemoryCardUnformatRequest D_00159A80;
extern u8 D_0015AFC0[];
extern s32 PollSema();
extern s32 SignalSema();
extern s32 sceSifCallRpc();
s32 SceMcUnformat(s32 port, s32 slot) {
    s32 rpc_result;

    if (D_00159A00.serve == 0) {
        return -0x64;
    }
    if (PollSema(D_00132DAC[0]) < 0) {
        return -0xC8;
    }
    D_00159A80.port = port;
    D_00159A80.slot = slot;
    rpc_result = sceSifCallRpc(&D_00159A00, 0x11, 1, &D_00159A80, 0x30, D_0015AFC0, 4, 0, 0);
    if (rpc_result == 0) {
        D_00132DA8[0] = 0x11;
    } else {
        SignalSema(D_00132DAC[0]);
    }
    return rpc_result;
}
