#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/sdk/library/sdk_globals.h"

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

extern u8              dbc_set_work_address_rpc_error_message[] __asm__("D_00153558");
extern s32             debug_print_stub() __asm__("func_00124A20");
extern s32             sceSifCallRpc();
extern struct DbCState RpcReceiveData __asm__("D_0015B080");

s32 sceDbcSetWorkAddr(s32 work_address) {
    RpcReceiveData.unk4 = work_address;
    if (sceSifCallRpc(
            &RpcQueueData, 0x80000904, 0, &RpcReceiveData, 0x400, &RpcReceiveData, 0x400, 0, 0) <
        0) {
        debug_print_stub(dbc_set_work_address_rpc_error_message);
        return 0;
    }
    return RpcReceiveData.unk0;
}
