#include "types.h"
#include "kernel.h"
#include "rnc/sdk/library/sif_file_slot.h"
#include "sifrpc.h"
#include "rnc/sdk/library/sdk_state.h"
#include "rnc/data_labels.h"

struct FsReadRequest {
    s32   completion_semaphore;
    void *result;
    s32   result_size;
    s32   fd;
    void *buffer;
    s32   length;
    s32   reserved18;
    s32   slot;
};

extern struct FsReadRequest D_00156880;
/* Also accessed by the asynchronous completion interrupt handler. */
extern struct SifFileSlot *get_iob(s32 fd);
extern s32                 _sceFsWaitS(s32);
extern s32                 ReadQueueStatus(void);
extern s32                 CreateSema(struct SemaParam *);
extern s32                 DeleteSema(s32);
extern s32                 WaitSema(s32);
extern s32                 SignalSema(s32);
extern void                sceSifWriteBackDCache(void *, s32);
extern s32 sceSifCallRpc(void *, s32, s32, void *, s32, void *, s32, void *, void *);

s32 sceRead(s32 fd, void *buffer, s32 length) {
    struct FsReadRequest *request;
    struct SifFileSlot   *file_slot;
    struct SemaParam      semaphore_parameters;
    s32                   result;
    s32                   flags;
    s32                   completion_semaphore;
    s32                   async_index;
    volatile s32         *async_slot;
    s32                   rpc_result;

    request = &D_00156880;
    file_slot = get_iob(fd);
    _sceFsWaitS(2);
    if (FsResetState == 0) {
        ReadQueueStatus();
        return -1;
    }
    if (file_slot == 0 || (flags = file_slot->flags) == 0) {
        ReadQueueStatus();
        return -9;
    }
    request->fd = file_slot->fd;
    semaphore_parameters.maxCount = 1;
    request->slot = file_slot - fio_iob;
    request->buffer = buffer;
    request->length = length;
    semaphore_parameters.initCount = 0;
    semaphore_parameters.option = 0;
    completion_semaphore = CreateSema(&semaphore_parameters);
    request->result = &result;
    request->result_size = 4;
    D_00156880.completion_semaphore = completion_semaphore;
    if ((s16)flags & 0x8000) {
        WaitSema(fio_async_table_sema);
        for (async_index = 0; async_index < 32; async_index++) {
            volatile s32 *async_semaphores = fio_async_semas;

            async_slot = async_semaphores + async_index;
            if (*async_slot == -1) {
                *async_slot = request->completion_semaphore;
                request->completion_semaphore = -request->completion_semaphore;
                break;
            }
        }
        SignalSema(fio_async_table_sema);
    }
    if (!(flags & 0x20000000)) {
        sceSifWriteBackDCache(buffer, length);
    }
    sceSifWriteBackDCache(request, 0x20);
    if (sceSifCallRpc(&fio_rpc_client, 2, 0, &D_00156880, 0x20, fio_rpc_result, 4, 0, 0) < 0) {
        DeleteSema(completion_semaphore);
        ReadQueueStatus();
        return -11;
    }
    rpc_result = *(s32 *)((u32)fio_rpc_result | 0x20000000);
    ReadQueueStatus();
    if (rpc_result == 0) {
        DeleteSema(completion_semaphore);
        return -11;
    }
    if (flags & 0x8000) {
        DeleteSema(completion_semaphore);
        return 0;
    }
    WaitSema(completion_semaphore);
    DeleteSema(completion_semaphore);
    return result;
}
