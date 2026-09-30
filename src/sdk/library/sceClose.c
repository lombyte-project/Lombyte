#include "types.h"
struct M2c_D_00156880 { s32 unk0; s32 unk4; s32 unk8; s32 unkC; s32 unk10; };
struct M2c_temp_16_13 { s32 unk0; s32 unk4; };
struct Sema { s32 count; s32 max_count; s32 init_count; s32 wait_threads; s32 attr; s32 option; };
extern u32 D_0012FC94[];
extern struct M2c_D_00156880 D_00156880;
extern u8 D_001574C0[];
extern u8 D_00157D80[];
extern u8 D_00157F80[];
extern struct M2c_temp_16_13 *get_iob(s32);
extern s32 _sceFsWaitS(s32 arg0);
extern s32 ReadQueueStatus(void);
extern s32 CreateSema(void *param);
extern s32 DeleteSema(s32 id);
extern s32 WaitSema(s32 id);
extern s32 sceSifCallRpc(void *a0, s32 a1, s32 a2, void *a3, s32 a4, void *a5, s32 a6, void *a7, void *a8);
s32 sceClose(s32 fd) {
    struct Sema sema;
    s32 result;
    s32 ok;
    s32 sid;
    struct M2c_temp_16_13 *io;
    struct M2c_D_00156880 *state = &D_00156880;

    io = get_iob(fd);
    _sceFsWaitS(1);
    if (D_0012FC94[0] == 0) {
        ReadQueueStatus();
        return -1;
    }
    if (io == NULL || io->unk4 == 0) {
        ReadQueueStatus();
        return -9;
    }
    state->unkC = io->unk0;
    sema.max_count = 1;
    state->unk10 = ((u8 *)io - (u8 *)D_00157D80) >> 4;
    sema.init_count = 0;
    sema.option = 0;
    sid = CreateSema(&sema);
    state->unk0 = sid;
    state->unk4 = (s32)&result;
    state->unk8 = 4;
    if (sceSifCallRpc(D_00157F80, 1, 0, state, 0x14, D_001574C0, 4, 0, 0) < 0) {
        DeleteSema(sid);
        ReadQueueStatus();
        return -0xB;
    }
    io->unk4 = 0;
    ok = *(u32 *)((u32)D_001574C0 | 0x20000000);
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
