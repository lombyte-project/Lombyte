#include "types.h"
struct MusicChannelHandle {
    u32 handle;
    u8 pad4[6];
    s16 state;
};
struct MusicTrackParameters {
    s16 track;
    s16 volume;
    s16 flags;
};
struct MusicStartParameters {
    u8 pad0[0x38];
    struct MusicTrackParameters primary;
    u8 pad3e[0x16];
    struct MusicTrackParameters secondary;
};
extern struct MusicStartParameters D_001516D0;
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
            music_start_track_by_id(D_001516D0.secondary.track, D_001516D0.secondary.flags,
                                    D_001516D0.secondary.volume);
        }
    }
}

extern __typeof__(music_secondary_start_callback) func_002169C0
    __attribute__((alias("FUN_002169c0")));
