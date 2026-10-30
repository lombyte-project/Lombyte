#include "types.h"
#include "kernel.h"
#include "rnc/sdk/library/sif_file_slot.h"
#include "rnc/sdk/library/sdk_state.h"
#include "rnc/data_labels.h"

typedef char *va_list;

struct FsOpenRequest {
    s32   completion_semaphore;
    void *result;
    s32   result_size;
    s32   flags;
    s32   mode;
    char  path[0x400];
    s32   slot;
};

extern struct FsOpenRequest D_00156880;
extern s32                  CreateSema(struct SemaParam *);
extern s32                  DeleteSema(s32);
extern s32                  ReadQueueStatus(void);
extern s32                  SignalSema(s32);
extern s32                  WaitSema(s32);
extern s32                  _sceFsWaitS(s32);
extern s32                  sif_file_server_changed(void) __asm__("func_0011BBB8");
extern struct SifFileSlot  *new_iob(void);
extern s32                  sceFsInit(void);
extern s32 sceSifCallRpc(void *, s32, s32, void *, s32, void *, s32, void *, void *);

s32 sceOpen(const u8 *path, s32 flags, ...) {
    struct SemaParam      semaphore_parameters;
    va_list               arguments;
    s32                   result;
    s32                   path_index;
    s32                   slot_mutex;
    s32                   mode;
    s32                   completion_semaphore;
    s32                   slot_index;
    s32                   return_value;
    struct SifFileSlot   *file_slot;
    struct FsOpenRequest *request;

    request = &D_00156880;
    /* The six remaining EE argument registers occupy eight bytes each. */
    arguments = __builtin_next_arg(flags) - 0x30;
    _sceFsWaitS(0);
    if (FsResetState == 0) {
        sceFsInit();
    }
    if (sif_file_server_changed() != 0) {
        ReadQueueStatus();
        return -0x10004;
    }
    file_slot = new_iob();
    if (file_slot == NULL) {
        ReadQueueStatus();
        return -0x13;
    }
    mode = *(s32 *)arguments;
    for (path_index = 0; path_index < 0x400; path_index++) {
        if ((request->path[path_index] = path[path_index]) == 0) {
            break;
        }
    }
    if (path_index == 0x400) {
        request->path[0x3FF] = 0;
    }
    slot_index = file_slot - fio_iob;
    request->flags = (s32)(flags & 0x6FFFFFFF);
    request->mode = mode;
    semaphore_parameters.maxCount = 1;
    request->slot = slot_index;
    semaphore_parameters.initCount = 0;
    semaphore_parameters.option = 0;
    completion_semaphore = CreateSema(&semaphore_parameters);
    request->result = &result;
    request->completion_semaphore = completion_semaphore;
    request->result_size = 4;
    if (sceSifCallRpc(&fio_rpc_client, 0, 0, &D_00156880, 0x418, fio_rpc_result, 4, 0, 0) < 0) {
        DeleteSema(completion_semaphore);
        ReadQueueStatus();
        return -0xB;
    }
    return_value = *(u32 *)((u32)fio_rpc_result | 0x20000000);
    ReadQueueStatus();
    if (return_value == 0) {
        DeleteSema(completion_semaphore);
        return -0xB;
    }
    WaitSema(completion_semaphore);
    DeleteSema(completion_semaphore);
    if (result < 0) {
        WaitSema(fio_iob_sema);
        file_slot->flags = 0;
        SignalSema(fio_iob_sema);
        return result;
    }
    return_value = slot_index;
    WaitSema(fio_iob_sema);
    slot_mutex = fio_iob_sema;
    file_slot->fd = result;
    file_slot->flags = (s32)(file_slot->flags | flags);
    SignalSema(slot_mutex);
    return return_value;
}
