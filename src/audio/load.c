#include "types.h"

#include "rnc/audio/music/music_stream_state.h"

extern void ReadGlobalTableEntry(void);
extern s32 snd_flush_sound_commands() __asm__("func_0012DC80");
extern s32 snd_reset_state_and_flush_commands() __asm__("func_0012EB00");
extern s32 music_update() __asm__("func_00216290");
extern s32 start_audio_stream_read(s32 dst, s32 sector, s32 sector_count) __asm__("FUN_00216788");
extern s32 sceGsSyncV();

s32 load(s32 dst, s32 sector, s32 sector_count) __asm__("FUN_00216828");

s32 load(s32 dst, s32 sector, s32 sector_count) {
    s32 result = start_audio_stream_read(dst, sector, sector_count);

    if (result != 0 && music_stream_state.read_state != 0) {
        do {
            sceGsSyncV(0);
            music_update();
            snd_reset_state_and_flush_commands();
            snd_flush_sound_commands();
            ReadGlobalTableEntry();
        } while (music_stream_state.read_state != 0);
    }
    return result;
}

extern s32 func_00216828(s32 dst, s32 sector, s32 sector_count) __attribute__((alias("FUN_00216828")));
