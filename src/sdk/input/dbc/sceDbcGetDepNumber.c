#include "types.h"
#include "rnc/sdk/library/sdk_globals.h"

struct DbCBlob {
    u8 data[0x40];
};

struct DbCState {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    u8  pad_14[0x10];
    s32 unk24;
    s32 unk28;
    s32 unk2C;
};

extern u8              dbc_get_dep_number_rpc_error_message[] __asm__("D_001535B8");
extern u8              D_0015B480[];
extern u8              D_0015B500[];
extern s32             DIntr();
extern s32             EnableInterrupts();
extern s32             SyncDCache() __asm__("func_00118F88");
extern s32             debug_print_stub() __asm__("func_00124A20");
extern s32             sceSifCallRpc();
extern struct DbCState RpcReceiveData __asm__("D_0015B080");

s32 sceDbcGetDepNumber(s32 dependency_index) {
    s32 result;

    SyncDCache(D_0015B480, D_0015B480 + 0x80);
    DIntr();
    *(struct DbCBlob *)D_0015B500 = *(struct DbCBlob *)D_0015B480;
    EnableInterrupts();
    if (((s32 *)D_0015B500)[dependency_index] == 1) {
        RpcReceiveData.unk0 = dependency_index;
        if (sceSifCallRpc(&RpcQueueData,
                          0x80000903,
                          0,
                          &RpcReceiveData,
                          0x400,
                          &RpcReceiveData,
                          0x400,
                          0,
                          0) < 0) {
            debug_print_stub(dbc_get_dep_number_rpc_error_message);
            return 0;
        }
        return RpcReceiveData.unk4;
    }
    return -0xC;
}
