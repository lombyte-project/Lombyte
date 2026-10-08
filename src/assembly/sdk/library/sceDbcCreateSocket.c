#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/library/sceDbcCreateSocket/sceDbcCreateSocket.s",
            sceDbcCreateSocket);
#else
#include "types.h"
struct DbcRpcBuffer {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    u8 pad_14[0x10];
    s32 unk24;
    s32 unk28;
    s32 unk2C;
};
struct DbcSocketParams {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    u8 name[0x10];
};
extern u8 D_00153578[];
extern u8 D_0015B008[];
extern struct DbcRpcBuffer D_0015B080;
extern s32 sceSifCallRpc();
extern s32 debug_print_stub() __asm__("func_00124A20");
s32 sceDbcCreateSocket(struct DbcSocketParams *arg0, s32 arg1, s32 arg2) {
    struct DbcRpcBuffer *state = &D_0015B080;
    s32 i;
    s32 even;
    state->unk28 = arg1;
    even = arg0->unk0;
    i = 0;
    state->unk2C = arg2;
    state->unk0 = even;
    even = arg0->unk4;
    state->unk4 = even;
    state->unk8 = arg0->unk8;
    state->unkC = arg0->unkC;
    state->unk10 = arg0->unk10;
    do {
        state->pad_14[i] = arg0->name[i];
        i++;
    } while (i < 0x10);
    if (sceSifCallRpc(D_0015B008, 0x80000901, 0, &D_0015B080, 0x400, &D_0015B080, 0x400, 0, 0) <
        0) {
        debug_print_stub(D_00153578);
        return 0;
    }
    return state->unk24;
}

#endif /* NON_MATCHING */
