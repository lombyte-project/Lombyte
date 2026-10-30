#include "types.h"
#include "sifrpc.h"
#include "rnc/sdk/libmc.h"

struct MemoryCardWriteRequest {
    s32 file_descriptor;
    s32 port;
    s32 slot;
    s32 size;
    s32 offset;
    u32 origin;
    s32 buffer;
    s32 reserved_1c;
    u8  data[0x10];
};

extern struct MemoryCardWriteRequest D_00159A80;
extern s32                           FlushCache();
extern s32                           PollSema();
extern s32                           SignalSema();
extern s32                           sceSifCallRpc();

s32 sceMcWrite(s32 fd, u8 *buffer, s32 size) {
    s32 rpc_result;
    u32 i;
    s32 root_off;
    s32 buffer_alignment_reference;

    if (mc_rpc_client.serve == 0) {
        return -0x64;
    }
    if (PollSema(mc_sema) < 0) {
        return -0xC8;
    }
    D_00159A80.file_descriptor = fd;
    if (size < 0x11) {
        D_00159A80.origin = size;
        D_00159A80.size = 0;
        D_00159A80.buffer = 0;
    } else {
        buffer_alignment_reference = (s32)buffer - 0x10;
        root_off = ((s32)(buffer - 1) & 0xFFFFFFF0) - buffer_alignment_reference;
        D_00159A80.size = size - root_off;
        D_00159A80.origin = root_off;
        D_00159A80.buffer = (s32)(buffer + root_off);
    }
    for (i = 0; i < D_00159A80.origin; i++) {
        D_00159A80.data[i] = buffer[i];
    }
    FlushCache(0);
    rpc_result =
        sceSifCallRpc(&mc_rpc_client, MC_FUNC_WRITE, 1, &D_00159A80, 0x30, mc_rpc_result, 4, 0, 0);
    if (rpc_result == 0) {
        mc_current_func = MC_FUNC_WRITE;
    } else {
        SignalSema(mc_sema);
    }
    return rpc_result;
}
