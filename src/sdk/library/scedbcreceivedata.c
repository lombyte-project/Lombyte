#include "types.h"
struct DbCState { s32 unk0; s32 unk4; s32 unk8; u8 payload[0x80]; s32 unk8C; };
extern u8 D_001536A0[]; extern u8 D_0015B008[]; extern struct DbCState D_0015B080;
extern s32 func_00124A20(); extern s32 sceSifCallRpc();
s32 sceDbcReceiveData(s32 arg0, s32 arg1, s32 *arg2, u8 *arg3) {
    struct DbCState *state = &D_0015B080;
    s32 i;

    state->unk0 = arg0;
    state->unk4 = arg1;
    state->unk8 = *arg2;
    if (sceSifCallRpc(D_0015B008, 0x8000091A, 0, state, 0x400, state, 0x400, 0, 0) < 0) {
        func_00124A20(D_001536A0);
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
