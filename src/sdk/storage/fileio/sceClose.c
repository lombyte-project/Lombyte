#include "types.h"
#include "kernel.h"
#include "rnc/sdk/library/sdk_state.h"
#include "rnc/data_labels.h"
#include "rnc/sdk/library/sif_file_slot.h"

struct SifFileState {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
};

extern struct SifFileState D_00156880;
extern struct SifFileSlot *get_iob(s32);
extern s32                 _sceFsWaitS(s32 arg0);
extern s32                 ReadQueueStatus(void);
extern s32                 CreateSema(void *param);
extern s32                 DeleteSema(s32 id);
extern s32                 WaitSema(s32 id);
extern s32
sceSifCallRpc(void *a0, s32 a1, s32 a2, void *a3, s32 a4, void *a5, s32 a6, void *a7, void *a8);

s32 sceClose(s32 fd) {
    struct SemaParam     sema;
    s32                  result;
    s32                  ok;
    s32                  sid;
    struct SifFileSlot  *io;
    struct SifFileState *state = &D_00156880;

    io = get_iob(fd);
    _sceFsWaitS(1);
    if (FsResetState == 0) {
        ReadQueueStatus();
        return -1;
    }
    if (io == NULL || io->flags == 0) {
        ReadQueueStatus();
        return -9;
    }
    state->unkC = io->fd;
    sema.maxCount = 1;
    state->unk10 = io - fio_iob;
    sema.initCount = 0;
    sema.option = 0;
    sid = CreateSema(&sema);
    state->unk0 = sid;
    state->unk4 = (s32)&result;
    state->unk8 = 4;
    if (sceSifCallRpc(&fio_rpc_client, 1, 0, state, 0x14, fio_rpc_result, 4, 0, 0) < 0) {
        DeleteSema(sid);
        ReadQueueStatus();
        return -0xB;
    }
    io->flags = 0;
    ok = *(u32 *)((u32)fio_rpc_result | 0x20000000);
    ReadQueueStatus();
    if (ok == 0) {
        DeleteSema(sid);
        return -0xB;
    }
    WaitSema(sid);
    DeleteSema(sid);
    if (result < 0) {
        return result;
    }
    return 0;
}
