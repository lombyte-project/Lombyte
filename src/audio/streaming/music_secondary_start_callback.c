#include "types.h"
#include "rnc/audio/music/music_stream_state.h"
extern void music_start_track_by_id(s32, s32, s32) __asm__("func_00215970");
void music_secondary_start_callback(u32 handle, s64 context) __asm__("FUN_002169c0");

void music_secondary_start_callback(u32 handle, s64 context) {
    struct MusicStreamChannel *channel = (struct MusicStreamChannel *)(s32)context;

    if (channel != 0) {
        channel->handle = handle;
        if (handle != 0) {
            if (channel->state == 1) {
                channel->state = 2;
            }
        } else {
            music_start_track_by_id(music_stream_state.secondary.track, music_stream_state.secondary.flags,
                                    music_stream_state.secondary.volume);
        }
    }
}

extern __typeof__(music_secondary_start_callback) func_002169C0
    __attribute__((alias("FUN_002169c0")));
