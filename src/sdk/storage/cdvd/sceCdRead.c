#include "types.h"
#include "rnc/sdk/libcdvd.h"

struct CdReadCommand {
    u32               sector;
    u32               count;
    void             *buffer;
    struct sceCdRMode mode;
    void             *rpc_buffer;
    s32              *status;
};

extern volatile u32         D_001312F4[];
extern volatile u32         D_00131314[];
extern struct CdReadCommand native_cd_read_request __asm__("D_001313C0");
extern u8                   native_cd_read_completion_buffer[] __asm__("D_001323C0");
extern u8                   native_cd_read_response[] __asm__("D_00132480");
extern u8                   native_cd_read_submit_debug_message[] __asm__("D_00152FC8");
extern u8                   native_cd_read_queued_debug_message[] __asm__("D_00152FE0");
extern void                 process_native_cd_read_completion(void *) __asm__("func_00120788");
extern s32                  SignalSema(u32 semaphore);
extern s32                  cd_check_ncmd() __asm__("func_00120A28");
extern s32                  sceCdNcmdDiskReady();
extern s32                  scePrintf();
extern s32                  sceSifCallRpc();
extern void                 sceSifWriteBackDCache(void *ptr, s32 size);

s32 sceCdRead(u32 sector, u32 count, void *buffer, struct sceCdRMode *mode) {
    enum {
        CD_READ_PATTERN_2048_BYTES_PER_SECTOR = 0,
        CD_READ_PATTERN_2328_BYTES_PER_SECTOR = 1,
        CD_READ_PATTERN_2340_BYTES_PER_SECTOR = 2
    };
    struct CdReadCommand *command = &native_cd_read_request;
    s32                   size;

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
    command->rpc_buffer = native_cd_read_completion_buffer;
    command->status = (s32 *)native_cd_read_response;
    switch (mode->datapattern) {
    case CD_READ_PATTERN_2328_BYTES_PER_SECTOR:
        size = count * 2328;
        break;
    case CD_READ_PATTERN_2340_BYTES_PER_SECTOR:
        size = count * 2340;
        break;
    case CD_READ_PATTERN_2048_BYTES_PER_SECTOR:
    default:
        size = count * 2048;
        break;
    }
    *(s32 *)native_cd_read_response = 0;
    if (!(D_001312F4[0] & 2)) {
        sceSifWriteBackDCache(buffer, size);
    }
    sceSifWriteBackDCache(native_cd_read_completion_buffer, 0x90);
    sceSifWriteBackDCache(command, sizeof(*command));
    sceSifWriteBackDCache(native_cd_read_response, 4);
    if (cd_debug > 0) {
        scePrintf(native_cd_read_submit_debug_message);
    }
    D_00131314[0] = 1;
    cd_read_pending = 1;
    if (sceSifCallRpc(&cd_ncmd_client,
                      1,
                      1,
                      command,
                      sizeof(*command),
                      0,
                      0,
                      process_native_cd_read_completion,
                      native_cd_read_completion_buffer) < 0) {
        D_00131314[0] = 0;
        cd_read_pending = 0;
        SignalSema(cd_ncmd_sema);
        return 0;
    }
    if (cd_debug > 0) {
        scePrintf(native_cd_read_queued_debug_message);
    }
    return 1;
}
