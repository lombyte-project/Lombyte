#include "types.h"
#include "rnc/audio/sound_read_work.h"
extern s32 D_0015EC8C __attribute__((sda));
extern s32 D_0015EC94 __attribute__((sda));
extern s32 D_0015EC98 __attribute__((sda));
extern s32 sceCdRead();
extern s32 snd_stream_safe_cd_sync() __asm__("func_0012EE08");
extern s32 snd_send_iop_command_no_wait() __asm__("func_0012E6E0");

s32 snd_stream_safe_cd_read(s32 lsn, s32 sector_count, s32 dst) __asm__("FUN_0012ed58");

s32 snd_stream_safe_cd_read(s32 lsn, s32 sector_count, s32 dst) {
    s32 buf[3];

    if (D_0015EC8C == 0) {
        return sceCdRead(lsn, sector_count, dst);
    }
    if (snd_stream_safe_cd_sync(1) == 1) {
        return 0;
    }
    buf[0] = lsn;
    sound_read_work.read_active = 1;
    sound_read_work.read_error = 0;
    buf[1] = sector_count;
    buf[2] = dst;
    snd_send_iop_command_no_wait(0x38, 0xC, buf, 0, 0);
    D_0015EC94 = 1;
    D_0015EC98 = 0;
    return 1;
}

extern s32 func_0012ED58(s32 lsn, s32 sector_count, s32 dst) __attribute__((alias("FUN_0012ed58")));
extern s32 snd_StreamSafeCdRead(s32 lsn, s32 sector_count, s32 dst)
    __attribute__((alias("FUN_0012ed58")));
