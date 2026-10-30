#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/sdk/library/sdk_globals.h"

struct DbCState {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    u8  payload[0x80];
    s32 unk8C;
};

extern u8              dbc_receive_rpc_error_message[] __asm__("D_001536A0");
extern s32             debug_print_stub() __asm__("func_00124A20");
extern s32             sceSifCallRpc();
extern struct DbCState RpcReceiveData __asm__("D_0015B080");
s32                    sceDbcReceiveData(s32  rpc_request_word_0,
                                         s32  rpc_request_word_1,
                                         s32 *byte_count,

                                         u8 *destination) {
    struct DbCState *state = &RpcReceiveData;
    s32              i;

    state->unk0 = rpc_request_word_0;
    state->unk4 = rpc_request_word_1;
    state->unk8 = *byte_count;
    if (sceSifCallRpc(&RpcQueueData, 0x8000091A, 0, state, 0x400, state, 0x400, 0, 0) < 0) {
        debug_print_stub(dbc_receive_rpc_error_message);
        return 0;
    }
    if (state->unk8C < 0) {
        return state->unk8C;
    }
    *byte_count = state->unk8;
    i = 0;
    if (state->unk8 > 0) {
        do {
            destination[i] = state->payload[i];
            i++;
        } while (i < state->unk8);
        return state->unk8C;
    }
    return *(volatile s32 *)&state->unk8C;
}
