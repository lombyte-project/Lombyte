#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/sdk/library/sdk_globals.h"
struct DbCState {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    u8 payload[0x80];
    s32 unk8C;
};
extern u8 D_001536A0[];
extern s32 debug_print_stub() __asm__("func_00124A20");
extern s32 sceSifCallRpc();
extern struct DbCState RpcReceiveData __asm__("D_0015B080");
s32 sceDbcReceiveData(s32 arg0, s32 arg1, s32 *arg2, u8 *arg3) {
    struct DbCState *state = &RpcReceiveData;
    s32 i;

    state->unk0 = arg0;
    state->unk4 = arg1;
    state->unk8 = *arg2;
    if (sceSifCallRpc(RpcQueueData, 0x8000091A, 0, state, 0x400, state, 0x400, 0, 0) < 0) {
        debug_print_stub(D_001536A0);
        return 0;
    }
    if (state->unk8C < 0) {
        return state->unk8C;
    }
    *arg2 = state->unk8;
    i = 0;
    if (state->unk8 > 0) {
        do {
            arg3[i] = state->payload[i];
            i++;
        } while (i < state->unk8);
        return state->unk8C;
    }
    return *(volatile s32 *)&state->unk8C;
}
