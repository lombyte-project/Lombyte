#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
/* Exact SDK/library unit sceFsInit; symbolic expected assembly retained pending source recovery. */
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/library/sceFsInit/sceFsInit.s", sceFsInit);
#else
#include "types.h"

struct FsRequest {
    s32 field0;
    s32 field4;
};

struct FsQueue {
    u8 pad0[0x24];
    s32 reply_ready;
};

struct FsSemaSlot {
    u8 pad0[4];
    s32 value;
    u8 pad1[8];
};

struct FsWord4 {
    u8 bytes[4];
};

/* Incomplete arrays keep these small globals on the absolute-address path. */
extern u8 D_0011B980[];
extern s32 D_0012FC94[];
extern s32 D_0012FC98[];
extern s32 D_0012FCA0[];
extern struct FsRequest D_00156840[];
extern u8 D_001574C0[];
extern u8 D_00157500[];
extern u8 D_00157D80[];
extern struct FsQueue D_00157F80;
extern struct FsWord4 D_00157FA8[];
extern u8 D_00157FC0[];
extern struct FsRequest D_00158000[];

extern s32 DIntr();
extern s32 EnableInterrupts();
extern s32 SignalSema();
extern s32 WaitSema();
extern s32 _sceFsIobSemaMK();
extern s32 sceSifAddCmdHandler();
extern s32 sceSifBindRpc();
extern s32 sceSifCallRpc();
extern s32 sceSifInitRpc();
extern void _sceFs_Rcv_Intr();

s32 sceFsInit(void) {
    s32 irq_state;
    s32 countdown;
    struct FsRequest *init_request;
    struct FsRequest *rpc_command;
    struct FsSemaSlot *sema_slot;
    u8 *sema_end;
    struct FsWord4 response_word;

    init_request = &D_00158000[0];
    sceSifInitRpc(0);
    init_request->field0 = 0;
    init_request->field4 = 0;
    irq_state = DIntr();
    sceSifAddCmdHandler(0x80000011, &_sceFs_Rcv_Intr, D_00157FC0);
    sceSifAddCmdHandler(0x80000013, D_0011B980, init_request);

    if (irq_state != 0) {
        EnableInterrupts();
    }

    for (;;) {
        if (sceSifBindRpc(&D_00157F80, 0x80000001, 0) < 0) {
            return -1;
        }
        if (D_00157F80.reply_ready != 0) {
            break;
        }
        countdown = 0x100000;
        do {
            countdown--;
        } while (countdown != -1);
    }

    _sceFsIobSemaMK();
    WaitSema(D_0012FCA0[0]);
    sema_slot = (struct FsSemaSlot *)D_00157D80;
    sema_end = &D_00157D80[0x200];
    while ((u8 *)sema_slot < sema_end) {
        sema_slot->value = 0;
        sema_slot++;
    }
    SignalSema(D_0012FCA0[0]);

    rpc_command = &D_00156840[0];
    rpc_command->field0 = (s32)&D_00157500[0];
    rpc_command->field4 = (s32)&D_00157500[0x440];
    if (sceSifCallRpc(&D_00157F80, 0xFF, 0, rpc_command, 8,
                      (s32)D_001574C0, 8, 0, 0) < 0) {
        return 0xFFFEFFFF;
    }

    D_00157FA8[0] = *(struct FsWord4 *)((s32)D_001574C0 | 0x20000000);
    response_word = *(struct FsWord4 *)(((s32)D_001574C0 + 4) | 0x20000000);
    D_0012FC98[0] = (*(s32 *)&response_word == 2);
    D_0012FC94[0] = 1;
    return 0;
}
#endif /* NON_MATCHING */
