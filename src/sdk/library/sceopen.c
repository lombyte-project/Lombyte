#include "types.h"

typedef char *va_list;

struct FsOpenRequest {
    s32 completion_semaphore;
    void *result;
    s32 result_size;
    s32 flags;
    s32 mode;
    char path[0x400];
    s32 slot;
};

struct SemaParam {
    s32 count;
    s32 max_count;
    s32 init_count;
    s32 wait_threads;
    s32 attr;
    s32 option;
};

struct SifFileSlot {
    s32 fd;
    s32 flags;
    s32 reserved8;
    s32 reservedC;
};

extern s32 D_0012FC94[];
extern s32 D_0012FCA0[];
extern struct FsOpenRequest D_00156880;
extern u8 D_001574C0[];
extern struct SifFileSlot D_00157D80[];
extern u8 D_00157F80[];
extern s32 CreateSema(struct SemaParam *);
extern s32 DeleteSema(s32);
extern s32 ReadQueueStatus(void);
extern s32 SignalSema(s32);
extern s32 WaitSema(s32);
extern s32 _sceFsWaitS(s32);
extern s32 func_0011BBB8(void);
extern struct SifFileSlot *new_iob(void);
extern s32 sceFsInit(void);
extern s32 sceSifCallRpc(void *, s32, s32, void *, s32, void *, s32, void *, void *);

s32 sceOpen(const u8 *path, s32 flags, ...) {
    struct SemaParam semaphore_parameters;
    va_list arguments;
    s32 result;
    s32 path_index;
    s32 slot_mutex;
    s32 mode;
    s32 completion_semaphore;
    s32 slot_index;
    s32 return_value;
    struct SifFileSlot *file_slot;
    struct FsOpenRequest *request;

    request = &D_00156880;
    /* The six remaining EE argument registers occupy eight bytes each. */
    arguments = __builtin_next_arg(flags) - 0x30;
    _sceFsWaitS(0);
    if (D_0012FC94[0] == 0) {
        sceFsInit();
    }
    if (func_0011BBB8() != 0) {
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
    slot_index = file_slot - D_00157D80;
    request->flags = (s32)(flags & 0x6FFFFFFF);
    request->mode = mode;
    semaphore_parameters.max_count = 1;
    request->slot = slot_index;
    semaphore_parameters.init_count = 0;
    semaphore_parameters.option = 0;
    completion_semaphore = CreateSema(&semaphore_parameters);
    request->result = &result;
    request->completion_semaphore = completion_semaphore;
    request->result_size = 4;
    if (sceSifCallRpc(D_00157F80, 0, 0, &D_00156880, 0x418, D_001574C0, 4, 0, 0) < 0) {
        DeleteSema(completion_semaphore);
        ReadQueueStatus();
        return -0xB;
    }
    return_value = *(u32 *)((u32)D_001574C0 | 0x20000000);
    ReadQueueStatus();
    if (return_value == 0) {
        DeleteSema(completion_semaphore);
        return -0xB;
    }
    WaitSema(completion_semaphore);
    DeleteSema(completion_semaphore);
    if (result < 0) {
        WaitSema(D_0012FCA0[0]);
        file_slot->flags = 0;
        SignalSema(D_0012FCA0[0]);
        return result;
    }
    return_value = slot_index;
    WaitSema(D_0012FCA0[0]);
    slot_mutex = D_0012FCA0[0];
    file_slot->fd = result;
    file_slot->flags = (s32)(file_slot->flags | flags);
    SignalSema(slot_mutex);
    return return_value;
}
