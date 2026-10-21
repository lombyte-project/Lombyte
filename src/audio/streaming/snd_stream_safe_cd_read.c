#include "types.h"
#include "rnc/audio/sound_read_work.h"
#include "rnc/data_labels.h"
#include "rnc/audio/sound_state.h"
extern s32 sceCdRead();
extern s32 snd_stream_safe_cd_sync() __asm__("func_0012EE08");
extern s32 snd_send_iop_command_no_wait() __asm__("func_0012E6E0");

s32 snd_stream_safe_cd_read(s32 lsn, s32 sector_count, s32 dst) __asm__("FUN_0012ed58");

s32 snd_stream_safe_cd_read(s32 lsn, s32 sector_count, s32 read_destination) {
    s32 cd_read_command_payload[3];

    if (snd_cd_stream_active == 0) {
        return sceCdRead(lsn, sector_count, read_destination);
    }
    if (snd_stream_safe_cd_sync(1) == 1) {
        return 0;
    }
    cd_read_command_payload[0] = lsn;
    sound_read_work.read_active = 1;
    sound_read_work.read_error = 0;
    cd_read_command_payload[1] = sector_count;
    cd_read_command_payload[2] = read_destination;
    snd_send_iop_command_no_wait(0x38, 0xC, cd_read_command_payload, 0, 0);
    snd_cd_sync_pending = 1;
    snd_cd_stream_end_pending = 0;
    return 1;
}

extern s32 func_0012ED58(s32 lsn, s32 sector_count, s32 dst) __attribute__((alias("FUN_0012ed58")));
extern s32 snd_StreamSafeCdRead(s32 lsn, s32 sector_count, s32 dst)
    __attribute__((alias("FUN_0012ed58")));
