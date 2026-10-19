#include "types.h"
#include "rnc/sdk/libcdvd.h"

struct CdReadCommand {
    u32 sector;
    u32 count;
    void *buffer;
    struct sceCdRMode mode;
    void *rpc_buffer;
    s32 *status;
};

extern s32 D_001312D0[];
extern volatile u32 D_001312E8[];
extern volatile u32 D_001312F0[];
extern volatile u32 D_001312F4[];
extern volatile u32 D_00131314[];
extern struct CdReadCommand D_001313C0;
extern u8 D_001323C0[];
extern u8 D_00132480[];
extern u8 D_00132490[];
extern u8 D_00152FC8[];
extern u8 D_00152FE0[];
extern void process_native_cd_read_completion(void *) __asm__("func_00120788");
extern s32 SignalSema(u32 semaphore);
extern s32 cd_check_ncmd() __asm__("func_00120A28");
extern s32 sceCdNcmdDiskReady();
extern s32 scePrintf();
extern s32 sceSifCallRpc();
extern void sceSifWriteBackDCache(void *ptr, s32 size);

s32 sceCdRead(u32 sector, u32 count, void *buffer, struct sceCdRMode *mode) {
    struct CdReadCommand *command = &D_001313C0;
    s32 size;

    if (!(D_001312F4[0] & 1) && sceCdNcmdDiskReady() == 6) {
        return 0;
    }
    if (cd_check_ncmd(4) == 0) {
        return 0;
    }
    command->sector = sector;
    command->count = count;
    command->buffer = buffer;
    command->mode.trycount = mode->trycount;
    command->mode.spindlctrl = mode->spindlctrl;
    command->mode.datapattern = mode->datapattern;
    command->rpc_buffer = D_001323C0;
    command->status = (s32 *)D_00132480;
    switch (mode->datapattern) {
    case 1:
        size = count * 2328;
        break;
    case 2:
        size = count * 2340;
        break;
    case 0:
    default:
        size = count * 2048;
        break;
    }
    *(s32 *)D_00132480 = 0;
    if (!(D_001312F4[0] & 2)) {
        sceSifWriteBackDCache(buffer, size);
    }
    sceSifWriteBackDCache(D_001323C0, 0x90);
    sceSifWriteBackDCache(command, sizeof(*command));
    sceSifWriteBackDCache(D_00132480, 4);
    if (D_001312D0[0] > 0) {
        scePrintf(D_00152FC8);
    }
    D_00131314[0] = 1;
    D_001312F0[0] = 1;
    if (sceSifCallRpc(D_00132490, 1, 1, command, sizeof(*command), 0, 0, process_native_cd_read_completion, D_001323C0) < 0) {
        D_00131314[0] = 0;
        D_001312F0[0] = 0;
        SignalSema(D_001312E8[0]);
        return 0;
    }
    if (D_001312D0[0] > 0) {
        scePrintf(D_00152FE0);
    }
    return 1;
}
