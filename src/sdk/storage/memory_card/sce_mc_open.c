#include "types.h"
#include "sifrpc.h"
struct MemoryCardNameParam {
    s32 port;
    s32 slot;
    s32 flags;
    s32 maxent;
    s32 unk10;
    char name[0x400];
};
extern u32 D_00132DA8[];
extern u32 D_00132DAC[];
extern struct sceSifClientData D_00159A00;
extern u8 D_00159AC4[];
extern u8 D_0015AFC0[];
extern s32 PollSema();
extern s32 SignalSema();
extern s32 sceSifCallRpc();
extern char *strncpy();
s32 sceMcOpen(s32 port, s32 slot, char *name, s32 mode) {
    struct MemoryCardNameParam *request;
    s32 rpc_result;

    if (D_00159A00.serve == 0) {
        return -0x64;
    }
    if (PollSema(D_00132DAC[0]) < 0) {
        return -0xC8;
    }
    if (name == 0 || name[0] == 0) {
        SignalSema(D_00132DAC[0]);
        return -0xD2;
    }
    strncpy(D_00159AC4, name, 0x3FF);
    request = (struct MemoryCardNameParam *)(D_00159AC4 - 0x14);
    request->port = port;
    request->flags = mode;
    request->slot = slot;
    request->name[0x3FF] = 0;
    rpc_result = sceSifCallRpc(&D_00159A00, 2, 1, request, 0x414, D_0015AFC0, 4, 0, 0);
    if (rpc_result == 0) {
        D_00132DA8[0] = 2;
    } else {
        SignalSema(D_00132DAC[0]);
    }
    return rpc_result;
}
