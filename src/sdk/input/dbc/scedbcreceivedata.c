#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/sdk/library/sdk_globals.h"
#include "rnc/sdk/rpc/rpc.h"

struct DbCState {
    s32 unk0;
    s32 unk4;
    s32 byte_count;
    u8  payload[0x80];
    s32 result;
};

const char dbc_receive_rpc_error_message[] __asm__("D_001536A0") = "sceDbcReceiveData: rpc error\n";
extern s32 debug_print_stub() __asm__("func_00124A20");
extern struct DbCState RpcReceiveData __asm__("D_0015B080");
s32                    sceDbcReceiveData(s32  rpc_request_word_0,
                                         s32  rpc_request_word_1,
                                         s32 *byte_count,

                                         u8 *destination) {
    struct DbCState *rpc_transfer_buffer = &RpcReceiveData;
    s32              i;

    rpc_transfer_buffer->unk0 = rpc_request_word_0;
    rpc_transfer_buffer->unk4 = rpc_request_word_1;
    rpc_transfer_buffer->byte_count = *byte_count;
    if (sceSifCallRpc(&RpcQueueData,
                      0x8000091A,
                      0,
                      rpc_transfer_buffer,
                      0x400,
                      rpc_transfer_buffer,
                      0x400,
                      0,
                      0) < 0) {
        debug_print_stub(dbc_receive_rpc_error_message);
        return 0;
    }
    if (rpc_transfer_buffer->result < 0) {
        return rpc_transfer_buffer->result;
    }
    *byte_count = rpc_transfer_buffer->byte_count;
    i = 0;
    if (rpc_transfer_buffer->byte_count > 0) {
        do {
            destination[i] = rpc_transfer_buffer->payload[i];
            i++;
        } while (i < rpc_transfer_buffer->byte_count);
        return rpc_transfer_buffer->result;
    }
    return *(volatile s32 *)&rpc_transfer_buffer->result;
}
