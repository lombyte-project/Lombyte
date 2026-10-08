#include "types.h"
#include "rnc/audio/music/music_stream_state.h"
struct MusicChannelHandle {
    u32 handle;
    u8 pad4[6];
    s16 state;
};
extern void music_start_track_by_id(s32, s32, s32) __asm__("func_00215970");
void music_secondary_start_callback(u32 handle, s64 context) __asm__("FUN_002169c0");

void music_secondary_start_callback(u32 handle, s64 context) {
    struct MusicChannelHandle *channel = (struct MusicChannelHandle *)(s32)context;

    if (channel != 0) {
        channel->handle = handle;
        if (handle != 0) {
            if (channel->state == 1) {
                channel->state = 2;
            }
        } else {
            music_start_track_by_id(music_stream_state.secondary_track, music_stream_state.secondary_flags,
                                    music_stream_state.secondary_volume);
        }
    }
}

extern __typeof__(music_secondary_start_callback) func_002169C0
    __attribute__((alias("FUN_002169c0")));
