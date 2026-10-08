#include "types.h"

#include "rnc/audio/music/music_stream_state.h"

extern void ReadGlobalTableEntry(void);
extern s32 snd_flush_sound_commands() __asm__("func_0012DC80");
extern s32 snd_reset_state_and_flush_commands() __asm__("func_0012EB00");
extern s32 music_update() __asm__("func_00216290");
extern s32 sceGsSyncV();
s16 update_audio_stream_until_idle(s32 arg0) __asm__("FUN_002168a8");

s16 update_audio_stream_until_idle(s32 arg0) {
    if (arg0 != 0) {
        if (music_stream_state.read_state != 0) {
            do {
                sceGsSyncV(0);
                music_update();
                snd_reset_state_and_flush_commands();
                snd_flush_sound_commands();
                ReadGlobalTableEntry();
            } while (music_stream_state.read_state != 0);
        }
    } else {
        music_update();
        snd_reset_state_and_flush_commands();
        snd_flush_sound_commands();
        ReadGlobalTableEntry();
    }
    return music_stream_state.read_state;
}
