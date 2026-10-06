#include "types.h"

struct SifFileSlot {
    s32 fd;
    s32 flags;
    s32 reserved8;
    s32 reservedC;
};

struct FsSeekRequest {
    s32 completion_semaphore;
    void *result;
    s32 result_size;
    s32 fd;
    s32 offset;
    s32 origin;
    s32 slot;
};

struct SemaParam {
    s32 count;
    s32 max_count;
    s32 init_count;
    s32 wait_threads;
    u32 attr;
    u32 option;
};

extern struct FsSeekRequest D_00156880;
/* Also accessed by the asynchronous completion interrupt handler. */
extern volatile s32 D_0012FC10[];
extern s32 D_0012FC94[];
extern s32 D_0012FCA4[];
extern u8 D_001574C0[];
extern struct SifFileSlot D_00157D80[];
struct sceSifClientData {
    u8 pad[0x28];
};

extern struct sceSifClientData D_00157F80;
extern struct SifFileSlot *get_iob(s32 fd);
extern s32 _sceFsWaitS(s32);
extern s32 ReadQueueStatus(void);
extern s32 CreateSema(struct SemaParam *);
extern s32 DeleteSema(s32);
extern s32 WaitSema(s32);
extern s32 SignalSema(s32);
extern s32 sceSifCallRpc(void *, s32, s32, void *, s32, void *, s32, void *, void *);

s32 sceLseek(s32 fd, s32 offset, s32 origin) {
    struct FsSeekRequest *request;
    struct SifFileSlot *file_slot;
    struct SemaParam semaphore_parameters;
    s32 result;
    s32 flags;
    s32 completion_semaphore;
    s32 async_index;
    volatile s32 *async_slot;
    s32 rpc_result;

    request = &D_00156880;
    file_slot = get_iob(fd);
    _sceFsWaitS(4);
    if (D_0012FC94[0] == 0) {
        ReadQueueStatus();
        return -1;
    }
    if (file_slot == 0 || (flags = file_slot->flags) == 0) {
        ReadQueueStatus();
        return -9;
    }
    request->fd = file_slot->fd;
    request->offset = offset;
    request->origin = origin;
    request->slot = file_slot - D_00157D80;
    semaphore_parameters.max_count = 1;
    semaphore_parameters.init_count = 0;
    semaphore_parameters.option = 0;
    completion_semaphore = CreateSema(&semaphore_parameters);
    request->result = &result;
    request->result_size = 4;
    D_00156880.completion_semaphore = completion_semaphore;
    if ((s16)flags & 0x8000) {
        WaitSema(D_0012FCA4[0]);
        for (async_index = 0; async_index < 32; async_index++) {
            volatile s32 *async_semaphores = D_0012FC10;

            async_slot = async_semaphores + async_index;
            if (*async_slot == -1) {
                *async_slot = request->completion_semaphore;
                request->completion_semaphore = -request->completion_semaphore;
                break;
            }
        }
        SignalSema(D_0012FCA4[0]);
    }
    if (sceSifCallRpc(&D_00157F80, 4, 0, &D_00156880, 0x1C, D_001574C0, 4, 0, 0) < 0) {
        DeleteSema(completion_semaphore);
        ReadQueueStatus();
        return -11;
    }
    rpc_result = *(s32 *)((u32)D_001574C0 | 0x20000000);
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
