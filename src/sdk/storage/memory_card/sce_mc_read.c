#include "types.h"
struct sceSifClientData {
    u8 reserved_00[0x24];
    s32 is_initialized;
};

struct MemoryCardReadRequest {
    s32 file_descriptor;
    u8 reserved_04[0x8];
    s32 byte_count;
    u8 reserved_10[0x8];
    s32 destination_buffer;
    s32 transfer_buffer;
};

extern void unpack_spr_descriptor(void *) __asm__("FUN_001238a0");
extern u32 D_00132DA8[];
extern u32 D_00132DAC[];
extern struct sceSifClientData D_00159A00;
extern struct MemoryCardReadRequest D_00159A80;
extern u8 D_00159F00[];
extern u8 D_0015AFC0[];
extern s32 PollSema();
extern s32 SignalSema();
extern s32 sceSifCallRpc();
extern s32 sceSifWriteBackDCache();
s32 sceMcRead(s32 file_descriptor, s32 *destination_buffer, s32 byte_count) {
    s32 rpc_result;

    if (D_00159A00.is_initialized == 0) {
        return -0x64;
    }
    if (PollSema(D_00132DAC[0]) < 0) {
        return -0xC8;
    }
    D_00159A80.file_descriptor = file_descriptor;
    D_00159A80.transfer_buffer = D_00159F00;
    D_00159A80.destination_buffer = destination_buffer;
    D_00159A80.byte_count = byte_count;
    sceSifWriteBackDCache(destination_buffer, byte_count);
    sceSifWriteBackDCache(D_00159F00, 0xC0);
    rpc_result = sceSifCallRpc(&D_00159A00, 5, 1, &D_00159A80, 0x30, D_0015AFC0, 4,
                               unpack_spr_descriptor, D_00159F00);
    if (rpc_result == 0) {
        D_00132DA8[0] = 5;
    } else {
        SignalSema(D_00132DAC[0]);
    }
    return rpc_result;
}
