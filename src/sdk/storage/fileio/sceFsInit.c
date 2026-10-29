/* Ported from rac1-decomp (src/core/00119D88.c: func_0011BCB0). */
#include "types.h"
#include "sda.h"
#include "sifrpc.h"
#include "rnc/data_labels.h"
#include "rnc/sdk/library/sdk_state.h"
#include "rnc/sdk/library/sif_file_slot.h"

typedef struct SifFileCommandCallbackRecord {
    void (*callback)(void *);
    void *argument;
} SifFileCommandCallbackRecord;

/* Four bytes copied without an alignment assumption (lwl/lwr). */
struct UnalignedWord {
    char bytes[4];
};

extern SifFileCommandCallbackRecord D_00158000 NOT_SDA;
extern char  fs_receive_interrupt_handler_data[] __asm__("D_00157FC0");
extern char *fs_init_completion_buffer_addresses[] __asm__("D_00156840");
extern u8    FsResetBuffer[] __asm__("D_00157FA8");
extern u8    D_00157500[];
extern s32   D_0012FC98[];
extern void  _sceFs_Rcv_Intr();
extern void  invoke_sif_file_command_callback(
     void *packet, SifFileCommandCallbackRecord *callback_record) __asm__("func_0011B980");
extern void sceSifInitRpc(u32 mode);
extern s32  DIntr();
extern s32  EnableInterrupts();
extern s32  SignalSema();
extern s32  WaitSema();
extern s32  _sceFsIobSemaMK();
extern void sceSifAddCmdHandler(s32 command, u32 handler, u32 data);
extern s32  sceSifBindRpc();
extern s32  sceSifCallRpc();

s32 sceFsInit(void) {
    s32                  initialization_call_status, delay;
    struct SifFileSlot  *slot;
    struct UnalignedWord reply_word;

    SifFileCommandCallbackRecord *callback_record = &D_00158000;
    sceSifInitRpc(0);
    D_00158000.callback = 0;
    callback_record->argument = 0;
    initialization_call_status = DIntr();
    sceSifAddCmdHandler(0x80000011, (u32)_sceFs_Rcv_Intr, (u32)fs_receive_interrupt_handler_data);
    sceSifAddCmdHandler(0x80000013, (u32)invoke_sif_file_command_callback, (u32)callback_record);
    if (initialization_call_status != 0) {
        EnableInterrupts();
    }
    for (;;) {
        initialization_call_status = sceSifBindRpc(&fio_rpc_client, 0x80000001, 0);
        if (initialization_call_status < 0) {
            return -1;
        }
        if (fio_rpc_client.serve != 0) {
            break;
        }
        for (delay = 0x100000; delay != -1; delay--) {
        }
    }
    _sceFsIobSemaMK();
    WaitSema(fio_iob_sema);
    for (slot = fio_iob; slot < fio_iob + SIF_FILE_SLOT_COUNT; slot++) {
        slot->flags = 0;
    }
    SignalSema(fio_iob_sema);
    fs_init_completion_buffer_addresses[0] = D_00157500;
    fs_init_completion_buffer_addresses[1] = D_00157500 + 0x440;
    initialization_call_status = sceSifCallRpc(
        &fio_rpc_client, 0xFF, 0, fs_init_completion_buffer_addresses, 8, fio_rpc_result, 8, 0, 0);
    if (initialization_call_status < 0) {
        return (s32)0xFFFEFFFF;
    }
    *(struct UnalignedWord *)FsResetBuffer =
        *(struct UnalignedWord *)((u32)fio_rpc_result | 0x20000000);
    reply_word = *(struct UnalignedWord *)((u32)(fio_rpc_result + 4) | 0x20000000);
    D_0012FC98[0] = (*(s32 *)&reply_word == 2);
    FsResetState = 1;
    return 0;
}
