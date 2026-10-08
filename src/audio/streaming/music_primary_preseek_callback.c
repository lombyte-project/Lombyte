#include "types.h"
#include "rnc/audio/music/music_stream_state.h"
extern void music_preseek_track(s32, s32, s32) __asm__("func_00215B68");
void music_primary_preseek_callback(u32 handle, s64 context) __asm__("FUN_00216a20");

void music_primary_preseek_callback(u32 handle, s64 context) {
    struct MusicStreamChannel *channel = (struct MusicStreamChannel *)(s32)context;

    if (channel != 0) {
        channel->handle = handle;
        if (handle != 0) {
            if (channel->state == 1) {
                channel->state = 2;
            }
        } else {
            music_preseek_track(music_stream_state.primary.track, music_stream_state.primary.flags,
                                music_stream_state.primary.volume);
        }
    }
}

extern __typeof__(music_primary_preseek_callback) func_00216A20
    __attribute__((alias("FUN_00216a20")));
