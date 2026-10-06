#include "types.h"
struct sceSifClientData {
    u8 reserved_00[0x24];
    s32 is_initialized;
};
struct MemoryCardWriteRequest {
    s32 file_descriptor;
    s32 port;
    s32 slot;
    s32 size;
    s32 offset;
    u32 origin;
    s32 buffer;
    s32 reserved_1c;
    u8 data[0x10];
};
extern u32 D_00132DA8[];
extern u32 D_00132DAC[];
extern struct sceSifClientData D_00159A00;
extern struct MemoryCardWriteRequest D_00159A80;
extern u8 D_0015AFC0[];
extern s32 FlushCache();
extern s32 PollSema();
extern s32 SignalSema();
extern s32 sceSifCallRpc();
s32 sceMcWrite(s32 fd, u8 *buffer, s32 size) {
    s32 rpc_result;
    u32 i;
    s32 root_off;
    s32 base;

    if (D_00159A00.is_initialized == 0) {
        return -0x64;
    }
    if (PollSema(D_00132DAC[0]) < 0) {
        return -0xC8;
    }
    D_00159A80.file_descriptor = fd;
    if (size < 0x11) {
        D_00159A80.origin = size;
        D_00159A80.size = 0;
        D_00159A80.buffer = 0;
    } else {
        base = (s32)buffer - 0x10;
        root_off = ((s32)(buffer - 1) & 0xFFFFFFF0) - base;
        D_00159A80.size = size - root_off;
        D_00159A80.origin = root_off;
        D_00159A80.buffer = (s32)(buffer + root_off);
    }
    for (i = 0; i < D_00159A80.origin; i++) {
        D_00159A80.data[i] = buffer[i];
    }
    FlushCache(0);
    rpc_result = sceSifCallRpc(&D_00159A00, 6, 1, &D_00159A80, 0x30, D_0015AFC0, 4, 0, 0);
    if (rpc_result == 0) {
        D_00132DA8[0] = 6;
    } else {
        SignalSema(D_00132DAC[0]);
    }
    return rpc_result;
}
