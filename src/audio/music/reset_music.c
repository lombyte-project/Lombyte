#include "types.h"

#include "rnc/audio/music/music_stream_state.h"

extern s32 snd_flush_sound_commands() __asm__("func_0012DC80");
extern s32 snd_init_vag_streaming_ex() __asm__("func_0012EB20");
extern s32 register_audio_stream_callback() __asm__("func_00215420");
void reset_music(void) __asm__("FUN_00215390");

void reset_music(void) {
    s32 neg = -1;

    music_stream_state.cd_mode.trycount = 0x20;
    music_stream_state.unk0 = 0;
    music_stream_state.cd_mode.spindlctrl = 0;
    music_stream_state.cd_mode.datapattern = 0;
    music_stream_state.cd_mode.pad = 0;
    music_stream_state.primary.handle = 0;
    music_stream_state.primary.state = 0;
    music_stream_state.secondary.handle = 0;
    music_stream_state.secondary.state = 0;
    music_stream_state.transition.handle = 0;
    music_stream_state.transition.state = 0;
    music_stream_state.queued_secondary_track = neg;
    music_stream_state.requested_track = neg;
    snd_init_vag_streaming_ex(4, 0xF000, 0, 1);
    while (snd_flush_sound_commands() != 0) {
    }
    register_audio_stream_callback();
}
